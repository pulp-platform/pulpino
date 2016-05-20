# General Overview

2D-Convolution application with a kernel window 3x3.
Run on `octave` the script gen_stimuli.m to generate samples from a new random image or
use your image to generate samples.
A new file `data_image.h` is generated. It contains image samples, the used kernel and the
golden output value.

## Setup

Modify `COL` and `ROW` to change the random image size.
Set `WRITE_FILE` equal to 1 to generate the `data_image.h` header file for RISC-V.