#!/bin/bash
# We need bash for $EUID, =~, arrays and trap ERR
#
# First and only parameter gives the block device to be prepared.
# If not given then /dev/mmcblk0 is used.
#
# The environment variable MOUNT_POINT_BASE can be set to provide
# the path to the base name of the mount point.
# The default is /mnt/pulpino_

# Minimum SD card size is 128 MiB
# The script creates a small fixed-sized boot partition and uses the remainder for /.
BOOT_BLOCKS=131072 # 64 MiB

ROOTTAR="rootfs.tar"
SOURCE_DIR="sd_image/"
BOOTFILES=(BOOT.BIN devicetree.dtb uImage)

# Check input files
for f in ${BOOTFILES[*]} $ROOTTAR; do
    if [ ! -r "${SOURCE_DIR}${f}" ] || [ ! -s "${SOURCE_DIR}${f}" ]; then
        echo "\"${SOURCE_DIR}${f}\" is missing/not readable/empty.">&2
        exit 1
    fi
done

# Sanity check for block device
sd_dev="${1:-/dev/mmcblk0}"
if [ ! -b "$sd_dev" ]; then
    echo "Path \"$sd_dev\" does not point to a block device.">&2
    exit 1
fi

# Check size of target
minsize=$((2*${BOOT_BLOCKS}))
if [ $(blockdev --getsz "${sd_dev}") -lt $minsize ]; then
    echo "${sd_dev} is too small. Where did you find such a small card!?">&2
    exit 1
fi

# Let the user confirm
echo "This will re-partition \"$sd_dev\" and void all data on it."
read -r -p "Are you sure? [y/N] " rep
if [ "$rep" != "y" ]; then
    echo "Aborted.">&2
    exit 1
fi

# Test for root rights
if [ $EUID -ne 0 ]; then
    echo "This script must be run as root.">&2
    exit 1
fi

# Set up clean up hooks
return_value=0
cleanup () {
    rm -f "$sfdisk_file"
    exit $return_value
}
bailout () {
    local cmd="$BASH_COMMAND" rc=$?
    echo "Command \"$cmd\" exited with code \"$rc\".">&2
    return_value=1
    cleanup
}
trap bailout ERR SIGINT SIGTERM
trap cleanup EXIT

# Cleanup old partitions
oldparts=( $(lsblk -po KNAME -n "${sd_dev}" | grep -v "^${sd_dev}$" || true) )
for p in ${oldparts[*]} ; do
    umount "$p" 2>/dev/null || true
    wipefs -a "$p" >/dev/null 2>&1 || true
done
wipefs -a "${sd_dev}" >/dev/null 2>&1 || true

# Create temporary files and directories
sfdisk_file="$(mktemp pulpino_sfdisk_XXX)"
rootdir="$(mktemp -d ${MOUNT_POINT_BASE:-/mnt/pulpino_}XXX)"
bootdir="$(mktemp -d ${MOUNT_POINT_BASE:-/mnt/pulpino_}XXX)"

# Partition, format and mount target partitions
cat >"$sfdisk_file" <<EOF
label: dos
device: ${sd_dev}
unit: sectors

type=b, start=2048, size=${BOOT_BLOCKS}, bootable
type=83
EOF
sfdisk -q "${sd_dev}" <"$sfdisk_file"
parts=($(lsblk -po KNAME -n "${sd_dev}" | grep -v "^${sd_dev}$"))
mkfs.vfat -F32 -n BOOT "${parts[0]}" >/dev/null
mkfs.ext4 -q -F -L root -E nodiscard "${parts[1]}"
mount "${parts[1]}" "${rootdir}"
mount "${parts[0]}" "${bootdir}"

# Copy files to target
for f in ${BOOTFILES[*]} ; do
    cp --preserve=timestamps "${SOURCE_DIR}${f}" "${bootdir}"
done
tar --one-top-level="${rootdir}" -xf "${SOURCE_DIR}${ROOTTAR}"
sync -f "${rootdir}/bin" "${bootdir}/${BOOTFILES[0]}"

echo "Done. Check out ${bootdir} (boot) and ${rootdir} (root)."
echo "Execute \"make umount\" before removing the card."
