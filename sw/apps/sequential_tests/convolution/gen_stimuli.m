clear all
close all

COL        = 64;
ROW        = 64;
RANDOM     = 1;
WRITE_FILE = 1;
FILT       = 0;
WIN        = 3;


path_img = 'INSERT YOUR PATH HERE';

%MAKE SURE FILTER VALUES ARE signed int 8 bit (char) 

int_GaussianBlur_3 = [1 2 1; 2 4 2; 1 2 1];
int_GaussianBlur_5 = [1 4 7 4 1; 4 16 26 16 4; 7 26 41 26 7; 4 16 26 16 4; 1 4 7 4 1];

int_Edge_3 = [1 0 -1; 0 0 0; -1 0 1];
int_Edge_5 = [1 2 0 -2 -1; 4 8 0 -8 -4; 6 12 0 -12 -6; 4 8 0 -8 -4; 1 2 0 -2 -1];


%NORMALIZED BY A POWER OF TWO IN ORDER TO USE SHIFT

GaussianBlur_3 = int_GaussianBlur_3./16;
GaussianBlur_5 = int_GaussianBlur_5./256;
Edge_3 = int_Edge_3;
Edge_5 = int_Edge_5./256;


if WIN == 3
    offset = 1;
    if FILT == 0
        Filter = Edge_3;
        int_Filter = int_Edge_3;
        shift = 0;
    else
        Filter = GaussianBlur_3;
        int_Filter = int_GaussianBlur_3;
        shift = 4;
    end
else
    offset = 2;
    if FILT == 0
        Filter = Edge_5;
        int_Filter = int_Edge_5;
        shift = 8;
    else
        Filter = GaussianBlur_5;
        int_Filter = int_GaussianBlur_5;
        shift = 8;
    end
end


if RANDOM == 1
    image         = round(rand(ROW,COL)*255);
    R_img         = ROW
    C_img         = COL
else
    image_rbg     = imread(path_img);
    image         = rgb2gray(image_rbg);
    [R_img,C_img] = size(image)
end


image_conv = round(conv2(image, Filter));
image_conv_appr = zeros(R_img,C_img);


for R = 1+offset:R_img-offset
    for C = 1+offset:C_img-offset
        %MATRIX WINDOW CONV
        elem = 0;
        for i=-offset:offset
            for j=-offset:offset
                koeff = int8(int_Filter(i+1+offset,j+1+offset));
                data  = uint8(image(R+i,C+j));
                elem = int32(elem) + int32(data) * int32(koeff);
            end
        end
        elem = bitsra(elem,shift); %divide by 2**shift
        if elem < 0
           elem = 0;
        end
        if elem > 255
            elem = 255;
        end
        image_conv_appr(R,C) = elem; 
    end
end

if RANDOM == 1
    figure(1), imshow( double(image)./ double(max(max(image))) );
else
    figure(1), imshow( image );
end
figure(2), imshow( double(image_conv)./ double(max(max(image_conv))) );
figure(3), imshow( double(image_conv_appr)./ double(max(max(image_conv_appr))) );


if WRITE_FILE == 1
    fileID = fopen('data_image.h','w');

    fprintf(fileID,'#ifndef _INPUT_IMAGE_\n#define _INPUT_IMAGE_\n\n');

    fprintf(fileID,'#define IMG_ROW %d\n', R_img);
    fprintf(fileID,'#define IMG_COL %d\n', C_img);
    fprintf(fileID,'#define IMG_DIM IMG_ROW*IMG_COL\n\n');

    fprintf(fileID,'#define FILT_WIN %d\n', WIN);
    fprintf(fileID,'#define FILT_SHIFT %d\n', shift);
    fprintf(fileID,'#define FILT_DIM FILT_WIN*FILT_WIN\n\n');

    fprintf(fileID,'static Pixel In_Img[%d] = { ', R_img*C_img);
    for R = 1:R_img
        for C = 1:C_img
            fprintf(fileID,'%d, ', image(R,C));
        end
    end
    fprintf(fileID,'};\n\n');

    fprintf(fileID,'static Filtc Filter_Kern[%d] = { ', WIN*WIN);
    for R = 1:WIN
        for C = 1:WIN
            fprintf(fileID,'%d, ', int_Filter(R,C));
        end
    end
    fprintf(fileID,'};\n\n');


    fprintf(fileID,'static Pixel Gold_Out_Img[%d] = { ', R_img*C_img);
    for R = 1:R_img
        for C = 1:C_img
            fprintf(fileID,'%d, ', image_conv_appr(R,C));
        end
    end
    fprintf(fileID,'};\n\n');



    fprintf(fileID,'#endif');
    fclose(fileID);
end