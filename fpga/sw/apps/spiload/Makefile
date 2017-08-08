CC     = arm-xilinx-linux-gnueabi-gcc
CFLAGS =

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
