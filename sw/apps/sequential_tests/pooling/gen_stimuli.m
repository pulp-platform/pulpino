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

DATA_WIDTH = 16;
WIN        = 2;

COL          = 32;
ROW          = 32;
WRITE_FILE   = 1;
SHOW_FIGURES = 1;

format long

image         = int32(255*rand(ROW,COL));
R_img         = ROW;
C_img         = COL;
R_pool        = int32(ROW/2);
C_pool        = int32(COL/2);
max_pool      = rand(R_pool,C_pool);

k = 0;

imshow(double(image)/256)

for R = 1:R_pool
    for C = 1:C_pool
        rup = 2*(R-1) + 1;
        rdw = 2*(R-1) + 2;
        cup = 2*(C-1) + 1;
        cdw = 2*(C-1) + 2;
        window = [image(rup,cup) image(rup,cdw) image(rdw,cup) image(rdw,cdw)];
        max_pool(R,C) = max(window);
    end
end
figure,
imshow(double(max_pool)/256)

if WRITE_FILE == 1
    config_file = 'config.h';
    fileID = fopen(config_file ,'w');
    fprintf(fileID,'#ifndef _CONFIG_CONV_\n#define _CONFIG_CONV_\n\n');
    fprintf(fileID,'#define DATA_WIDTH %d\n', DATA_WIDTH);
    fprintf(fileID,'#define DATA_TYPE %d\n',16);
    fprintf(fileID,'#define IMG_ROW %d\n', R_img);
    fprintf(fileID,'#define IMG_COL %d\n', C_img);
    fprintf(fileID,'#define IMG_DIM IMG_ROW*IMG_COL\n\n');
    fprintf(fileID,'#define OUT_DIM IMG_ROW*IMG_COL/4\n\n');
    fprintf(fileID,'#endif');
    fclose(fileID);

    imgage_file = ['data.h'];
    fileID = fopen(imgage_file,'w');

    fprintf(fileID,'#ifndef _INPUT_IMAGE_\n#define _INPUT_IMAGE_\n\n');

    fprintf(fileID,'static Pixel In_Img[%d] = { ', R_img*C_img);
    for R = 1:R_img
        for C = 1:C_img
            fprintf(fileID,'%d, ', image(R,C));
        end
    end
    fprintf(fileID,'};\n\n');

    fprintf(fileID,'static Pixel Gold_Out_Img[%d] = { ', R_img*C_img/4);
    for R = 1:R_pool
        for C = 1:C_pool
           fprintf(fileID,'%d, ', max_pool(R,C));
        end
    end
    fprintf(fileID,'};\n\n');



    fprintf(fileID,'#endif');
    fclose(fileID);
end