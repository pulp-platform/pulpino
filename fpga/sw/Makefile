PWD=$(CURDIR)
NCORES=4
BOARD ?= zedboard

ARM_TOOLCHAIN_PATH := $(shell which arm-xilinx-linux-gnueabi-gcc)
ARM_TOOLCHAIN_PATH := $(shell dirname $(ARM_TOOLCHAIN_PATH))
ARM_TOOLCHAIN_PATH := $(shell dirname $(ARM_TOOLCHAIN_PATH))


all: u-boot linux rootfs devtree fsbl sd_image/BOOT.BIN

# To make sure a destination directory exists simply include the 
# directory as order-only target with a trailing /
.PRECIOUS: %/
%/:
	@mkdir -p $@

################################################################################
# linux
################################################################################

linux-xlnx:
	git clone git://github.com/Xilinx/linux-xlnx.git

linux-xlnx/.config: kernel.config | linux-xlnx
	cd linux-xlnx && git checkout -f tags/xilinx-v2015.1
	cp ./kernel.config ./linux-xlnx/.config

linux-xlnx/arch/arm/boot/uImage: linux-xlnx/.config
	cd linux-xlnx && $(MAKE) ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- UIMAGE_LOADADDR=0x8000 uImage PATH=$(shell pwd)/u-boot-xlnx/tools/:${PATH} -j${NCORES}

sd_image/uImage: linux-xlnx/arch/arm/boot/uImage | sd_image/
	cp linux-xlnx/arch/arm/boot/uImage ./sd_image/uImage

.PHONY: linux
linux: sd_image/uImage


################################################################################
# u-boot
################################################################################

u-boot-xlnx:
	git clone git://github.com/Xilinx/u-boot-xlnx.git

u-boot-xlnx/.config: $(filter-out $(wildcard u-boot*), u-boot-xlnx) | u-boot-xlnx
ifeq ($(BOARD),zybo)
	cd u-boot-xlnx && \
		git checkout -f 84f81e5b8950748fd7ca967ecf0428affef7ecea && \
		patch -p1 < ../u-boot-xlnx-def-env.patch && \
		$(MAKE) CROSS_COMPILE=arm-xilinx-linux-gnueabi- zynq_zybo_config
else
	cd u-boot-xlnx && \
		git checkout -f tags/xilinx-v2015.1 && \
		patch -p1 < ../u-boot-xlnx-def-env.patch && \
		$(MAKE) CROSS_COMPILE=arm-xilinx-linux-gnueabi- zynq_zed_config
endif
	cp ./u-boot.config.$(BOARD) ./u-boot-xlnx/.config

u-boot-xlnx/u-boot: u-boot-xlnx/.config
	cd u-boot-xlnx && $(MAKE) CROSS_COMPILE=arm-xilinx-linux-gnueabi-

sd_image/u-boot.elf: u-boot-xlnx/u-boot | sd_image/
	cp u-boot-xlnx/u-boot ./sd_image/u-boot.elf

.PHONY: u-boot
u-boot: sd_image/u-boot.elf


################################################################################
# buildroot
################################################################################

buildroot:
	git clone git://git.buildroot.net/buildroot.git

buildroot/.config: | buildroot
	cd buildroot && git checkout -f tags/2015.05
	cat ./buildroot.config | sed 's#BR2_TOOLCHAIN_EXTERNAL_PATH="/to/be/defined/"#BR2_TOOLCHAIN_EXTERNAL_PATH="$(ARM_TOOLCHAIN_PATH)"#g' > ./buildroot/.config

buildroot/output/images/rootfs.tar: buildroot/.config
	cd buildroot && $(MAKE) ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- busybox-source
	cp busybox.config buildroot/output/build/busybox-1.23.2/.config
	cd buildroot && $(MAKE) ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- -j${NCORES}

sd_image/rootfs.tar: buildroot/output/images/rootfs.tar | sd_image/
	cp buildroot/output/images/rootfs.tar sd_image/rootfs.tar

.PHONY: rootfs
rootfs: sd_image/rootfs.tar


################################################################################
# device tree blob
################################################################################

# uImage is used as an order-only prerequisite because the dtc binary used below is built that way.
sd_image/devicetree.dtb: $(wildcard device-tree/$(BOARD)/*.dts*) | linux-xlnx/arch/arm/boot/uImage
	cd device-tree/$(BOARD) && ../linux-xlnx/scripts/dtc/dtc -I dts -O dtb -o $(PWD)/sd_image/devicetree.dtb system.dts

.PHONY: devtree
devtree: sd_image/devicetree.dtb

################################################################################
# fsbl
################################################################################

hsi/pulpemu_top.sysdef: ../pulpemu/pulpemu_top.sysdef
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

sd_image/pulpemu_top.bit: ../pulpemu/pulpemu_top.bit | sd_image/
	cp $< $@

################################################################################
# boot.bin
################################################################################

sd_image/BOOT.BIN: sd_image/u-boot.elf sd_image/pulpemu_top.bit boot.bif sd_image/fsbl.elf | sd_image/
	bootgen -image ./boot.bif -o i ./sd_image/BOOT.BIN -w on

################################################################################
# SD card handling/installation
################################################################################
MOUNT_POINT_BASE:=/mnt/pulpino_
export MOUNT_POINT_BASE

.PHONY: install
install: sd_image/BOOT.BIN sd_image/pulpemu_top.bit sd_image/fsbl.elf sd_image/devicetree.dtb sd_image/rootfs.tar sd_image/u-boot.elf sd_image/uImage
	@sudo ./install.sh $(SD_DEV)

.PHONY: umount
umount:
	@echo "Umounting" "$(MOUNT_POINT_BASE)"*
	@sudo umount -Rl "$(MOUNT_POINT_BASE)"* 2>/dev/null || true
	@sudo rm -rf "$(MOUNT_POINT_BASE)"*/

################################################################################
# Cleanup
################################################################################
.PHONY: clean
clean:
	$(foreach d,$(wildcard u-boot-xlnx linux-xlnx buildroot apps/spiload),\
		$(MAKE) -C $(d) clean ;)
	rm -f u-boot-xlnx/.config
	rm -f linux-xlnx/.config
	rm -f buildroot/.config
	rm -f hsi/pulpemu_top.sysdef
	rm -f sd_image/*

.PHONY: distclean
distclean:
	rm -rf u-boot-xlnx linux-xlnx buildroot apps/spiload/spiload sd_image hsi/pulpemu_top.sysdef
