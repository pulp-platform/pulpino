PWD=$(CURDIR)
NCORES=4
BOARD ?= zedboard
VIVADO_VERSION ?= 2018.3

# Fall back to ancient Xilinx GCC due to outdated Linux sources that can't handle modern ones
ifeq ($(VIVADO_VERSION),2015.1)
  ARMTRIPLE:=arm-xilinx-linux-gnueabi
else
  ARMTRIPLE:=arm-linux-gnueabihf
endif

ARMCC:=$(ARMTRIPLE)-gcc
export ARMCC

ARCH=$(firstword $(subst -, ,$(ARMTRIPLE)))
ARMCC_PATH := $(shell which $(ARMCC))
ifeq ($(ARMCC_PATH),)
  $(error Path to $(ARMCC) unknown. PATH is "$(PATH)")
endif
ARM_TOOLCHAIN_PATH := $(realpath $(dir $(ARMCC_PATH))..)

all: u-boot linux rootfs devtree fsbl sd_image/BOOT.BIN

# To make sure a destination directory exists simply include the 
# directory as order-only target with a trailing /
.PRECIOUS: %/
%/:
	@mkdir -p $@

################################################################################
# apps
################################################################################

apps/spiload/spiload:
	$(MAKE) -C apps/spiload

.PHONY: spiload
spiload: apps/spiload/spiload

################################################################################
# linux
################################################################################

linux-xlnx:
	git clone git://github.com/Xilinx/linux-xlnx.git

linux-xlnx/.config: config/kernel.config.$(VIVADO_VERSION) | linux-xlnx
	cd linux-xlnx && git checkout -f tags/xilinx-v$(VIVADO_VERSION)
	cp config/kernel.config.$(VIVADO_VERSION) ./linux-xlnx/.config

linux-xlnx/arch/arm/boot/uImage: linux-xlnx/.config
	cd linux-xlnx && $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(ARMTRIPLE)- UIMAGE_LOADADDR=0x8000 uImage PATH=$(shell pwd)/u-boot-xlnx/tools/:${PATH} -j${NCORES}

sd_image/uImage: linux-xlnx/arch/arm/boot/uImage | sd_image/
	cp linux-xlnx/arch/arm/boot/uImage ./sd_image/uImage

.PHONY: linux
linux: sd_image/uImage


################################################################################
# u-boot
################################################################################

u-boot-xlnx:
	git clone git://github.com/Xilinx/u-boot-xlnx.git

u-boot-xlnx/.config: $(wildcard config/u-boot*) | u-boot-xlnx
ifeq ($(BOARD),zybo)
	cd u-boot-xlnx && \
		git checkout -f 84f81e5b8950748fd7ca967ecf0428affef7ecea && \
		patch -p1 < ../config/u-boot-xlnx-def-env.patch.$(VIVADO_VERSION) && \
		$(MAKE) CROSS_COMPILE=$(ARMTRIPLE)- zynq_zybo_config
else
	cd u-boot-xlnx && \
		git checkout -f tags/xilinx-v$(VIVADO_VERSION) && \
		patch -p1 < ../config/u-boot-xlnx-def-env.patch.$(VIVADO_VERSION) && \
		$(MAKE) CROSS_COMPILE=$(ARMTRIPLE)- zynq_zed_config
endif
	cp config/u-boot.config.$(BOARD).$(VIVADO_VERSION) ./u-boot-xlnx/.config

u-boot-xlnx/u-boot: u-boot-xlnx/.config
	cd u-boot-xlnx && $(MAKE) CROSS_COMPILE=$(ARMTRIPLE)- -j${NCORES}

sd_image/u-boot.elf: u-boot-xlnx/u-boot | sd_image/
	cp u-boot-xlnx/u-boot ./sd_image/u-boot.elf

.PHONY: u-boot
u-boot: sd_image/u-boot.elf


################################################################################
# buildroot
################################################################################

BUILDROOT_VERSION_2015.1 := 2015.05
BUILDROOT_VERSION_2018.3 := 2017.08
BUILDROOT_VERSION := $(BUILDROOT_VERSION_$(VIVADO_VERSION))

BUSYBOX_VERSION_2015.1 := 1.23.2
BUSYBOX_VERSION_2018.3 := 1.27.1
BUSYBOX_VERSION := $(BUSYBOX_VERSION_$(VIVADO_VERSION))

buildroot:
	git clone git://git.buildroot.net/buildroot.git

buildroot/.config: config/buildroot.config.$(VIVADO_VERSION) | buildroot
	cd buildroot && git checkout -f tags/$(BUILDROOT_VERSION)
	sed \
		-e 's#BR2_TOOLCHAIN_EXTERNAL_PATH="/to/be/defined/"#BR2_TOOLCHAIN_EXTERNAL_PATH="$(ARM_TOOLCHAIN_PATH)"#g' \
		-e 's#BR2_ROOTFS_POST_BUILD_SCRIPT=""#BR2_ROOTFS_POST_BUILD_SCRIPT="../buildroot-post-build.sh"#g' \
		./config/buildroot.config.$(VIVADO_VERSION) > ./buildroot/.config

buildroot/output/images/rootfs.tar: buildroot/.config buildroot-post-build.sh $(shell ./buildroot-post-build.sh DEPENDENCIES)
	cd buildroot && $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(ARMTRIPLE)- busybox-source
	cp config/busybox.config.$(VIVADO_VERSION) buildroot/output/build/busybox-$(BUSYBOX_VERSION)/.config
	cd buildroot && $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(ARMTRIPLE)- -j${NCORES}

sd_image/rootfs.tar: buildroot/output/images/rootfs.tar | sd_image/
	cp buildroot/output/images/rootfs.tar sd_image/rootfs.tar

.PHONY: rootfs
rootfs: sd_image/rootfs.tar


################################################################################
# device tree blob
################################################################################

DT_TOP_2015.1 := system.dts
DT_TOP_2018.3 := system-top.dts
DT_TOP := $(DT_TOP_$(VIVADO_VERSION))

device_tree_repo:
	git clone https://github.com/Xilinx/device-tree-xlnx.git device_tree_repo

hsi/dts_out/$(VIVADO_VERSION)-$(BOARD)/$(DT_TOP): device_tree_repo hsi/pulpemu_top.sysdef | hsi/dts_out/$(VIVADO_VERSION)-$(BOARD)/
	cd device_tree_repo && git checkout -f tags/xilinx-v$(VIVADO_VERSION)
	cd hsi && hsi -mode batch -source dt.tcl -tclargs dts_out/$(VIVADO_VERSION)-$(BOARD)

# If there is no dts available in this repo, try creating it.
# NB: The order-only prerequisites guarantee that we don't touch an existing .dts
# This has been semi-successful in the past but is a good starting point
# for further customizations (to be then committed as dts files in device-tree/).
# Additionally, the sed call below improves the output of hsi as follows
#  - fix the memory node if need be
#  - set stdout-path to UART1
#  - adapt the spi aliases to ensure /dev/spidev1.0 is the RISC-V memory mapping
#  - set the MAC address of the ethernet PHY to a valid (but random) value
#  - replace the kernel boot arguments to set the correct UART and root FS location
#
# uImage is used as an order-only prerequisite because the dtc binary used below is built that way.
device-tree/$(VIVADO_VERSION)-$(BOARD).dts: | hsi/dts_out/$(VIVADO_VERSION)-$(BOARD)/$(DT_TOP) linux-xlnx/arch/arm/boot/uImage
	$(info Generating new Device Tree for $(BOARD) with Vivado $(VIVADO_VERSION).)
	$(info Don't expect this to work out of the box.)
	cd hsi/dts_out/$(VIVADO_VERSION)-$(BOARD) && \
		cpp -nostdinc -undef -x assembler-with-cpp $(DT_TOP) | \
			$(PWD)/linux-xlnx/scripts/dtc/dtc -I dts -O dts - | \
			sed \
				-e 's|memory {|memory@0 {|' \
				-e 's|stdout-path = "serial0:|stdout-path = "serial1:|' \
				-e '/spi[01] = .*;/d' \
				-e 's|serial1 = "/amba/serial@e0001000";|serial1 = "/amba/serial@e0001000";\n\t\tspi0 = "/amba/spi@e000d000";\n\t\tspi1 = "/amba/spi@e0006000";|' \
				-e '/local-mac-address = .*;/d' \
				-e 's|ethernet@e000b000 {|ethernet@e000b000 {\n\t\t\tlocal-mac-address = [0e be 06 33 5f c6];|' \
				-e 's|bootargs = ".*";|bootargs = "console=ttyPS1,115200n8 ip=on rootfstype=ext4 root=/dev/mmcblk0p2 rw earlyprintk";|' \
			> $(PWD)/$@

# uImage is used as an order-only prerequisite because the dtc binary used below is built that way.
sd_image/devicetree.dtb: device-tree/$(VIVADO_VERSION)-$(BOARD).dts | linux-xlnx/arch/arm/boot/uImage
	linux-xlnx/scripts/dtc/dtc -I dts -O dtb -o $@ $<

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
	rm -fr hsi/dts_out/
	rm -f sd_image/*

.PHONY: distclean
distclean:
	rm -rf u-boot-xlnx linux-xlnx buildroot apps/spiload/spiload sd_image hsi/pulpemu_top.sysdef
