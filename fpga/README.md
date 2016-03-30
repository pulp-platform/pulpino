# FPGA

This folder contains everything needed to synthesize and run PULPino on a ZedBoard.


## Requirements

This synthesis flow has been tested with `Vivado 2015.1`, there is no guarantee
that it is going to work with any other version without modifications to the
scripts.

For convenience reasons it is best to connect the ZedBoard to your local
network. This way you can easily transfer files from your host computer to the
Linux running on the ARM cores of the ZYNQ.


## Get Started

1. Make sure you have the Vivado toolchain and the Xilinx SDK toolchain in your
   PATH before continuing. The Vivado toolchain is required to generate the
   bitstream, while the SDK contains the ARM compiler that is used for
   cross-compiling linux and applications.

2. Type `make all` in the fpga directory
   This builds the FPGA bitstream for the ZedBoard, downloads and compiles linux
   and u-boot, prepares the fsbl and devicetree, downloads and compiles buildroot
   and builds the boot.bin image for booting the ZYNQ.

3. Prepare the SD card and the ZedBoard for booting via SD card.
   To prepare the card, follow the Xilinx guide [1].

4. Copy the BOOT.BIN, uImage and devicetree.dtb files to the first partition of the SD card.
   Those files can be found inside the `fpga/sw/sd_image` folder.

5. Extract the content of the rootfs.tar archive and put it on the second
   partition of the SD card.
   You are ready now

6. Put the SD card into the ZedBoard and boot the system.
   You can use minicom or any other terminal emulator tool to communicate with
   the UART of the ZedBoard.

7. You should now be able to login to the ZYNQ and have a fully working Linux
   running on it.

8. To be able to login to Linux via ssh, you have to make sure that Linux is
   able to access the local network. By default it will try to get an IP
   address via dhcp. You can check with `ifconfig` and friends if your device
   has gotten an IP address and use it to connect to it via a host.


The boot.bin and rootfs.tar files can be found under the folder sw/sd_image.


## Running applications on PULPino

1. Make sure you have a fully working Linux running on the ZYNQ.
   Otherwise see section "get started" above.

2. Currently the only method to load a program into the PULPino system is via
   SPI. Linux uses its SPI master to communicate with PULPino's SPI slave and
   writes directly into the instruction and data memory of PULPino.
   The spiload program which can be found under sw/apps/spiload takes care of
   this.

3. Compile the spiload application for the ZYNQ.
   Just type `make` inside the sw/apps/spiload folder.

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


As an alternative, there is a cmake target for running applications on fpga
directly. Just call

    make applicationName.fpga

You need to be able to ssh into the Linux running on the ZYNQ fpga (e.g. using
public keys) and you need to setup the environment variable `$FPGA_HOSTNAME`.
Take a look at the script `./sw/utils/run-on-fpga.sh` to understand how it
works.


## stdout via printf on PULPino

When PULPino is run on the FPGA, it transfers all output via printf via UART to
the ARM host processor in the ZYNQ. To display it, either use a console program
like minicom to read directly from the serial port, or specify a timeout when
using `spiload`. `spiload` will connect to the serial port and display
everything that PULPino sends via UART until the timeout expires.

## Connected peripherals & communication with PULPino

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
