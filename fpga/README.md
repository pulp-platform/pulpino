# FPGA

This folder contains everything needed to synthesize and run PULPino on a ZedBoard.


## Requirements

This synthesis flow has been tested with `Vivado 2015.1`, there is no guarantee
that it is going to work with any other version without modifications to the
scripts.


## Get Started

1. Make sure you have the vivado toolchain in your path before continuing.

2. Type `make all` in the fpga directory
   This builds the FPGA bitstream for the ZedBoard, downloads and compiles linux
   and u-boot, prepares the fsbl and devicetree, downloads and compiles buildroot
   and builds the boot.bin image for booting the ZYNQ.

3. Prepare the SD card and the ZedBoard for booting via SD card.
   To prepare the card, follow the Xilinx guide [1].

4. Copy the boot.bin file to the first partition of the SD card.

5. Extract the content of the rootfs.tar archive and put it on the second
   partition of the SD card.
   You are ready now

6. Put the SD card into the ZedBoard and boot the system.
   You can use minicom or any other terminal emulator tool to communicate with
   the UART of the ZedBoard.

7. You should now be able to login to the ZYNQ and have a fully working Linux
   running on it.


The boot.bin and rootfs.tar files can be found under the folder sw/sd_image.


## Running applications on PULPino

1. Make sure you have a fully working Linux running on the ZYNQ.
   Otherwise see section "get started" above.

2. Currently the only method to load a program into the PULPino system is via
   SPI. Linux uses its SPI master to communicate with PULPinos SPI slave and
   writes directly into the instruction and data memory of PULPino.
   The spiload program which can be found under sw/spi_loader takes care of
   this.

3. Compile the spi_loader application for the ZYNQ.
   Just type `make` inside the sw/spi_loader folder.

4. Transfer this program to the ZYNQ. We suggest using scp, but any other
   method works as well of course.

5. Now you need to compile the program you want to run on PULPino.
   Please take a look at the README in pulpino/sw directory which explains how
   applications can be compiled using cmake.
   Use this flow to compile your application. We need the spi_stim.txt file
   from the applications slm_files subfolder.

6. Transfer the spi_stim.txt file to the ZYNQ.

7. Run the spiload application on the ZYNQ like this

    ./spiload ./spi_stim.txt

   This resets PULPino, transfers the application to the memories of PULPino
   and starts it.


## Connected peripherlas & communication with PULPino

PULPino includes a set of built-in peripherals like SPI, UART and GPIOs.
The SPI slave peripheral is connected to the SPI master of the ZYNQ, thus it is
possible to directly write to any memory address of PULPino from outside.

UART is connected to UART0 of the ZYNQ and is available under /dev/ttyPS0 in
linux.


Some of GPIO pins are connected to LEDs, switches and buttons on the ZedBoard.

Specifically the following is connected:

    PULPino GPIO pin  0: SW 0
    PULPino GPIO pin  1: SW 1
    PULPino GPIO pin  2: SW 2
    PULPino GPIO pin  3: SW 3
    PULPino GPIO pin  4: SW 4
    PULPino GPIO pin  5: SW 5
    PULPino GPIO pin  6: SW 6
    PULPino GPIO pin  7: SW 7

    PULPino GPIO pin  8: LD 0
    PULPino GPIO pin  9: LD 1
    PULPino GPIO pin 10: LD 2
    PULPino GPIO pin 11: LD 3
    PULPino GPIO pin 12: LD 4
    PULPino GPIO pin 13: LD 5
    PULPino GPIO pin 14: LD 6
    PULPino GPIO pin 15: LD 7

    PULPino GPIO pin 16: BTNC
    PULPino GPIO pin 17: BTND
    PULPino GPIO pin 18: BTNL
    PULPino GPIO pin 19: BTNR
    PULPino GPIO pin 20: BTNU




[1] http://www.wiki.xilinx.com/Prepare+Boot+Medium
