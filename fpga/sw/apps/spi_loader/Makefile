CC=arm-xilinx-linux-gnueabi-gcc


all: spiload

spiload: main.c
	$(CC) -o $@ $^


push: spiload
	scp ./spiload root@129.132.157.51:/root/
