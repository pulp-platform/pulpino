CC=arm-xilinx-linux-gnueabi-gcc


all: spiload

spiload: main.c arg_parsing.c console_read.c
	$(CC) -pthread -o $@ $^


push: spiload
	scp ./spiload $(FPGA_HOSTNAME):/root/
