ifeq ($(VIVADO_VERSION),2018.3)
	ARMCC?=arm-linux-gnueabihf-gcc
else
	ARMCC?=arm-xilinx-linux-gnueabi-gcc
endif
CC = $(ARMCC)
CFLAGS = -Wall

ifeq ($(BOARD),zybo)
	CFLAGS += -DZYBO
endif

all: spiload

spiload: main.c arg_parsing.c console_read.c
	$(CC) $(CFLAGS) -pthread -o $@ $^

push: spiload
	scp ./spiload root@$(FPGA_HOSTNAME):/root/

clean:
	@rm -f ./*.o spiload
