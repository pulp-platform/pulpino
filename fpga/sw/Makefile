PWD=$(CURDIR)
NCORES=4
ARM_TOOLCHAIN_PATH := $(shell which arm-xilinx-linux-gnueabi-gcc)
ARM_TOOLCHAIN_PATH := $(shell dirname $(ARM_TOOLCHAIN_PATH))
ARM_TOOLCHAIN_PATH := $(shell dirname $(ARM_TOOLCHAIN_PATH))


all: u-boot linux rootfs devtree fsbl sd_image/BOOT.BIN


################################################################################
# linux
################################################################################

linux-xlnx:
	git clone git://github.com/Xilinx/linux-xlnx.git
	cd linux-xlnx && git checkout tags/xilinx-v2015.1

linux-xlnx/.config: $(filter-out $(wildcard linux-xlnx), linux-xlnx)
	cp ./kernel.config ./linux-xlnx/.config

linux-xlnx/arch/arm/boot/uImage: linux-xlnx/.config
	cd linux-xlnx && make ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- UIMAGE_LOADADDR=0x8000 uImage PATH=$(shell pwd)/u-boot-xlnx/tools/:${PATH} -j${NCORES}

sd_image/uImage: linux-xlnx/arch/arm/boot/uImage
	mkdir -p sd_image
	cp linux-xlnx/arch/arm/boot/uImage ./sd_image/uImage

.PHONY: linux
linux: sd_image/uImage


################################################################################
# u-boot
################################################################################

u-boot-xlnx:
	git clone git://github.com/Xilinx/u-boot-xlnx.git
ifeq ($(BOARD),zybo)
	cd u-boot-xlnx && git checkout 84f81e5b8950748fd7ca967ecf0428affef7ecea
else
	cd u-boot-xlnx && git checkout tags/xilinx-v2015.1
endif
	cd u-boot-xlnx && patch -p1 < ../u-boot-xlnx-def-env.patch

u-boot-xlnx/.config: $(filter-out $(wildcard u-boot-xlnx), u-boot-xlnx)
ifeq ($(BOARD),zybo)
	cd u-boot-xlnx && make CROSS_COMPILE=arm-xilinx-linux-gnueabi- zynq_zybo_config
	cp ./u-boot.config.zybo ./u-boot-xlnx/.config
else
	cd u-boot-xlnx && make CROSS_COMPILE=arm-xilinx-linux-gnueabi- zynq_zed_config
	cp ./u-boot.config ./u-boot-xlnx/.config
endif

u-boot-xlnx/u-boot: u-boot-xlnx/.config
	cd u-boot-xlnx && make CROSS_COMPILE=arm-xilinx-linux-gnueabi-

sd_image/u-boot.elf: u-boot-xlnx/u-boot
	mkdir -p sd_image
	cp u-boot-xlnx/u-boot ./sd_image/u-boot.elf

.PHONY: u-boot
u-boot: sd_image/u-boot.elf


################################################################################
# buildroot
################################################################################

buildroot:
	git clone git://git.buildroot.net/buildroot.git
	cd buildroot && git checkout tags/2015.05

buildroot/.config: $(filter-out $(wildcard buildroot), buildroot)
	cat ./buildroot.config | sed 's#BR2_TOOLCHAIN_EXTERNAL_PATH="/to/be/defined/"#BR2_TOOLCHAIN_EXTERNAL_PATH="$(ARM_TOOLCHAIN_PATH)"#g' > ./buildroot/.config

buildroot/output/images/rootfs.tar: buildroot/.config
	cd buildroot && make ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- busybox-source
	cp busybox.config buildroot/output/build/busybox-1.23.2/.config
	cd buildroot && make ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- -j${NCORES}

sd_image/rootfs.tar: buildroot/output/images/rootfs.tar
	mkdir -p sd_image
	cp buildroot/output/images/rootfs.tar sd_image/rootfs.tar

.PHONY: rootfs
rootfs: sd_image/rootfs.tar


################################################################################
# device tree blob
################################################################################

sd_image/devicetree.dtb: device-tree/*.dts* $(filter-out $(wildcard linux-xlnx), linux-xlnx)
ifeq ($(BOARD),zybo)
	cd device-tree/zybo/ && $(PWD)/linux-xlnx/scripts/dtc/dtc -I dts -O dtb -o $(PWD)/sd_image/devicetree.dtb system.dts
else
	cd device-tree && ../linux-xlnx/scripts/dtc/dtc -I dts -O dtb -o ../sd_image/devicetree.dtb system.dts
endif

.PHONY: devtree
devtree: sd_image/devicetree.dtb

################################################################################
# fsbl
################################################################################

hsi/pulpemu_top.sysdef: ../pulpemu/pulpemu.runs/impl_1/pulpemu_top.sysdef
	cp $< $@

hsi/fsbl/executable.elf: hsi/pulpemu_top.sysdef
	cd hsi && hsi -mode tcl -source ./fsbl.tcl

sd_image/fsbl.elf: hsi/fsbl/executable.elf
	cp $< $@

.PHONY: fsbl
fsbl: sd_image/fsbl.elf

################################################################################
# get bitstream
################################################################################

sd_image/pulpemu_top.bit: ../pulpemu/pulpemu.runs/impl_1/pulpemu_top.bit
	cp $< $@

################################################################################
# boot.bin
################################################################################

sd_image/BOOT.BIN: sd_image/u-boot.elf sd_image/pulpemu_top.bit boot.bif sd_image/fsbl.elf
	bootgen -image ./boot.bif -o i ./sd_image/BOOT.BIN -w on

.PHONY: clean
clean:
	rm -rf u-boot-xlnx
	make -C linux-xlnx clean
	rm -f linux-xlnx/.config
	rm -f buildroot/.config
	make -C buildroot clean
	rm -f hsi/pulpemu_top.sysdef
	rm -f sd_image/*
