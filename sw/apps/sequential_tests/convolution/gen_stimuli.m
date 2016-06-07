clear all
close all

DATA_WIDTH = 8;
COL        = 32;
ROW        = 32;
RANDOM     = 1;
WRITE_FILE = 1;
FILT       = 0;
WIN        = 5;


format long

path_img = 'INSERT YOUR PATH HERE';

int_GaussianBlur_3 = [1 2 1; 2 4 2; 1 2 1];
int_GaussianBlur_5 = [1 4 7 4 1; 4 16 26 16 4; 7 26 41 26 7; 4 16 26 16 4; 1 4 7 4 1];

int_Sobelx_3 = [1 0 -1; 0 0 0; -1 0 1];
int_Sobelx_5 = [1 2 0 -2 -1; 4 8 0 -8 -4; 6 12 0 -12 -6; 4 8 0 -8 -4; 1 2 0 -2 -1];


%NORMALIZED BY A POWER OF TWO IN ORDER TO USE SHIFT

GaussianBlur_3 = int_GaussianBlur_3./16;
GaussianBlur_5 = int_GaussianBlur_5./273;
Sobelx_3       = int_Sobelx_3./abs((max(max(int_Sobelx_3)))+2^-DATA_WIDTH);
Sobelx_5       = int_Sobelx_5./abs((max(max(int_Sobelx_5)))+2^-DATA_WIDTH);

if RANDOM == 1
    image         = rand(ROW,COL);
    R_img         = ROW
    C_img         = COL
else
    image_rbg     = imread(path_img);
    image         = double(rgb2gray(image_rbg))./256;
    clearvars     image_rbg
    [R_img,C_img] = size(image)
end

%FIXED POINT TRANSFORMATION

fxp_signed     = 1; %signed
wordlenght     = DATA_WIDTH;
fractionalPart = DATA_WIDTH-1; %Q1.7 or Q1.15

fxp_GaussianBlur_3 = fi(GaussianBlur_3,  fxp_signed, wordlenght, fractionalPart) ;
fxp_GaussianBlur_5 = fi(GaussianBlur_5,  fxp_signed, wordlenght, fractionalPart) ;
fxp_Sobelx_3       = fi(Sobelx_3      ,  fxp_signed, wordlenght, fractionalPart) ;
fxp_Sobelx_5       = fi(Sobelx_5      ,  fxp_signed, wordlenght, fractionalPart) ;
image_fxp          = fi(image, fxp_signed, wordlenght, fractionalPart);

sat      = fi(1,fxp_signed,wordlenght,fractionalPart);

if WIN == 3
    offset = 1;
    if FILT == 0
        Filter     = Sobelx_3;
        fxp_Filter = fxp_Sobelx_3;
    else
        Filter     = GaussianBlur_3;
        fxp_Filter = fxp_GaussianBlur_3;
    end
else
    offset = 2;
    if FILT == 0
        Filter     = Sobelx_5;
        fxp_Filter = fxp_Sobelx_5;
    else
        Filter     = GaussianBlur_5;
        fxp_Filter = fxp_GaussianBlur_5;
    end
end

clearvars fxp_GaussianBlur_3 fxp_GaussianBlur_5 fxp_Sobelx_3 fxp_Sobelx_5 int_GaussianBlur_3 int_GaussianBlur_5 int_Sobelx_3 int_Sobelx_5 GaussianBlur_3 GaussianBlur_5 Sobelx_3 Sobelx_5
image_conv      = conv2(image, Filter);
image_conv_appr = zeros(R_img,C_img);

fxp_Filter_data    = fxp_Filter.double;%it contains real values from fixed point rapresentation
image_fxp_data     = image_fxp.double; %it contains real values from fixed point rapresentation

%Compute input error due to quantization
err2_img = sum(sum((image - image_fxp_data).*(image - image_fxp_data)))

for R = 1+offset:R_img-offset
    for C = 1+offset:C_img-offset
        %MATRIX WINDOW CONV
        elem = 0;
        for i=-offset:offset
            for j=-offset:offset
                koeff = fxp_Filter_data(i+1+offset,j+1+offset);
                data  = image_fxp_data(R+i,C+j);
                elem  = elem + data * koeff; %fi(data * koeff, 1, 2*DATA_WIDTH,2*DATA_WIDTH-2);
            end
        end
        if elem < 0
           elem = 0;
        end
        if elem >= 1
            elem = sat.double;
        end
        image_conv_appr(R,C) = elem; 
    end
end

image_conv_appr_fxp = fi(image_conv_appr, 1, DATA_WIDTH,DATA_WIDTH-1);

figure(1), imshow( image );
figure(2), imshow( image_conv);
figure(3), imshow( image_conv_appr_fxp.double);

if WRITE_FILE == 1

    fileID = fopen('config.h','w');
    fprintf(fileID,'#ifndef _CONFIG_CONV_\n#define _CONFIG_CONV_\n\n');
    fprintf(fileID,'#define DATA_WIDTH %d\n', DATA_WIDTH);
    fprintf(fileID,'#define IMG_ROW %d\n', R_img);
    fprintf(fileID,'#define IMG_COL %d\n', C_img);
    fprintf(fileID,'#define IMG_DIM IMG_ROW*IMG_COL\n\n');

    fprintf(fileID,'#define FILT_WIN %d\n', WIN);
    %fprintf(fileID,'#define FILT_SHIFT %d\n', shift);
    fprintf(fileID,'#define FILT_DIM FILT_WIN*FILT_WIN\n\n');

    fprintf(fileID,'#endif');
    fclose(fileID);

    fileID = fopen('data_image.h','w');

    fprintf(fileID,'#ifndef _INPUT_IMAGE_\n#define _INPUT_IMAGE_\n\n');

    fprintf(fileID,'static Pixel In_Img[%d] = { ', R_img*C_img);
    for R = 1:R_img
        for C = 1:C_img
            sample = image_fxp(R,C);
            fprintf(fileID,'0x%s, ', sample.hex);
        end
    end
    fprintf(fileID,'};\n\n');

    fprintf(fileID,'static Filtc Filter_Kern[%d] = { ', WIN*WIN);
    for R = 1:WIN
        for C = 1:WIN
            sample = fxp_Filter(R,C);
            fprintf(fileID,'0x%s, ', sample.hex);
        end
    end
    fprintf(fileID,'};\n\n');


    fprintf(fileID,'static Pixel Gold_Out_Img[%d] = { ', R_img*C_img);
    for R = 1:R_img
        for C = 1:C_img
           sample = image_conv_appr_fxp(R,C);
           fprintf(fileID,'0x%s, ', sample.hex);
        end
    end
    fprintf(fileID,'};\n\n');



    fprintf(fileID,'#endif');
    fclose(fileID);
end