ifeq ($(VIVADO_VERSION),2018.3)
	ARMCC?=arm-linux-gnueabihf-gcc
else
	ARMCC?=arm-xilinx-linux-gnueabi-gcc
endif
CC = $(ARMCC)

all: gpio_access

gpio_access: main.c
	$(CC) -o $@ $^


push: gpio_access
	scp ./gpio_access root@$(FPGA_HOSTNAME):/root/
