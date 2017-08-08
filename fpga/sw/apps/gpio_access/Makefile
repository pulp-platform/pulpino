CC=arm-xilinx-linux-gnueabi-gcc


all: gpio_access

gpio_access: main.c
	$(CC) -o $@ $^


push: gpio_access
	scp ./gpio_access root@$(FPGA_HOSTNAME):/root/
