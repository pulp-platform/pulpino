% Copyright 2017 ETH Zurich and University of Bologna.
% Copyright and related rights are licensed under the Solderpad Hardware
% License, Version 0.51 (the “License”); you may not use this file except in
% compliance with the License.  You may obtain a copy of the License at
% http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
% or agreed to in writing, software, hardware and materials distributed under
% this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
% CONDITIONS OF ANY KIND, either express or implied. See the License for the
% specific language governing permissions and limitations under the License.

clear all
close all

DATA_WIDTH = 14; %Data are Q1.(DATA_WIDTH-1)
WIN        = 5;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Choose DATA_WIDTH and WIN such that OF can never happen for this application            %
% OF can happen if 2^(2*(DATA_WIDTH-1))*WIN*WIN < 2^31 - 1                                %
% That is the case when all the data in the current window and the filter are the most    %
% negative value in CA2.                                                                  %
% eg: WIN = 5, DATA_WIDTH=14 gives                                                        %
% 2^26*25 < 2^31-1                                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

COL        = 32;
ROW        = 32;
RANDOM     = 1;
WRITE_FILE = 1;
FILT       = 0;

format long

path_img = 'INSERT YOUR PATH HERE';

int_GaussianBlur_3 = [1 2 1; 2 4 2; 1 2 1];
int_GaussianBlur_5 = [1 4 7 4 1; 4 16 26 16 4; 7 26 41 26 7; 4 16 26 16 4; 1 4 7 4 1];

int_Sobelx_3 = [1 0 -1; 0 0 0; -1 0 1];
int_Sobelx_5 = [1 2 0 -2 -1; 4 8 0 -8 -4; 6 12 0 -12 -6; 4 8 0 -8 -4; 1 2 0 -2 -1];

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
fractionalPart = DATA_WIDTH-1; %Q1.7 or Q1.13
sat      = fi(1,fxp_signed,wordlenght,fractionalPart);
sat_int = sat.double*2^fractionalPart;

if WIN == 3
    offset = 1;
    if FILT == 0
        Filter     = Sobelx_3;
    else
        Filter     = GaussianBlur_3;
    end
else
    offset = 2;
    if FILT == 0
        Filter     = Sobelx_5;
    else
        Filter     = GaussianBlur_5;
    end
end

fxp_Filter         = fi(Filter,  fxp_signed, wordlenght, fractionalPart);
fxp_Filter_int     = fxp_Filter.double.*(2^fractionalPart);

fxp_image          = fi(image, fxp_signed, wordlenght, fractionalPart) ;
fxp_image_int      = fxp_image.double.*(2^fractionalPart);

clearvars fxp_image fxp_Filter Sobelx_5 Sobelx_3 GaussianBlur_5 GaussianBlur_3

%image_conv           = conv2(image, Filter);
image_conv           = zeros(R_img,C_img);
image_conv_appr_int  = zeros(R_img,C_img);

%Compute input error due to quantization
err2_img = sum(sum((image - double(fxp_image_int)/2^fractionalPart).*(image - double(fxp_image_int)/2^fractionalPart)))

for R = 1+offset:R_img-offset
    for C = 1+offset:C_img-offset
        %MATRIX WINDOW CONV
        elem      = 0;
        elem_data = 0;
        elem_int  = 0;
        for i=-offset:offset
            for j=-offset:offset
                %real one, double values
                koeff    = Filter(i+1+offset,j+1+offset);
                data     = image(R+i,C+j);
                elem     = elem + data * koeff;

                %fixed point with int arithmetics
                koeff    = fxp_Filter_int(i+1+offset,j+1+offset);
                data     = fxp_image_int(R+i,C+j);
                elem_int = elem_int + data * koeff;

            end
        end

        image_conv(R,C)           = elem;
        image_conv_appr_int(R,C)  = elem_int;
    end
end


image_conv(image_conv < 0) = 0;
image_conv(image_conv > 1) = 1;
image_conv_appr_int = int32(image_conv_appr_int/2^fractionalPart);
image_conv_appr_int(image_conv_appr_int < 0)        = 0;
image_conv_appr_int(image_conv_appr_int > sat_int)  = sat_int;

output_err = sum(sum((image_conv - double(image_conv_appr_int)/2^fractionalPart).*(image_conv - double(image_conv_appr_int)/2^fractionalPart)))
max_err    = max(max((image_conv - double(image_conv_appr_int)/2^fractionalPart).*(image_conv - double(image_conv_appr_int)/2^fractionalPart)))

figure(1), imshow( image );
figure(2), imshow( image_conv);
figure(3), imshow( double(image_conv_appr_int)/2^fractionalPart);

if WRITE_FILE == 1

    fileID = fopen('config.h','w');
    fprintf(fileID,'#ifndef _CONFIG_CONV_\n#define _CONFIG_CONV_\n\n');
    fprintf(fileID,'#define DATA_WIDTH %d\n', DATA_WIDTH);
    if DATA_WIDTH <= 8
        fprintf(fileID,'#define DATA_TYPE %d\n',8);
    else
        fprintf(fileID,'#define DATA_TYPE %d\n',16);
    end
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
            fprintf(fileID,'%d, ', fxp_image_int(R,C));
        end
    end
    fprintf(fileID,'};\n\n');

    fprintf(fileID,'static Filtc Filter_Kern[%d] = { ', WIN*WIN);
    for R = 1:WIN
        for C = 1:WIN
            fprintf(fileID,'%d, ', fxp_Filter_int(R,C));
        end
    end
    fprintf(fileID,'};\n\n');


    fprintf(fileID,'static Pixel Gold_Out_Img[%d] = { ', R_img*C_img);
    for R = 1:R_img
        for C = 1:C_img
           fprintf(fileID,'%d, ', image_conv_appr_int(R,C));
        end
    end
    fprintf(fileID,'};\n\n');



    fprintf(fileID,'#endif');
    fclose(fileID);
end
