# General Overview

2D-Convolution application with a kernel window 3x3 or 5x5.
Run on `octave` the script gen_stimuli.m to generate samples from a new random image or
use your image to generate samples.
A new file `data_image.h` is generated. It contains image samples, the used kernel and the
golden output value.

## Setup

Set `DATA_WIDTH` equal to 16 to have "short" values or 8 to have "char" ones.
Set `RANDOM` equal to 1 to generate a random image.
Modify `COL` and `ROW` to change the image size.
Set `WRITE_FILE` equal to 1 to generate the `data_image.h` header file for RISC-V.
Set `WIN` equal to 3 or 5 to choose your kernel size.