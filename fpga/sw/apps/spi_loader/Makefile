CC=arm-xilinx-linux-gnueabi-gcc


all: spiload

spiload: main.c
	$(CC) -o $@ $^


push: spiload
	scp ./spiload $(FPGA_HOSTNAME):/root/
