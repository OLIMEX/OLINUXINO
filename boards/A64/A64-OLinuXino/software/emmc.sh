#!/bin/bash
#
# This is script could be used to transfer filesystem from MMC to eMMC.
#
# Copyright (C) 2017 Stefan Mavrodiev, OLIMEX LTD.
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program. If not, see http://www.gnu.org/licenses/.

# Release 0.0.1 / 6 APR 2016
# Release 0.1.0 / 25 MAY 2017
#

# Set default values
MMC_DEVICE=${MMC:="/dev/mmcblk1"}
EMMC_DEVICE=${EMMC:="/dev/mmcblk0"}


function check_mounted() {
    [ -z $1 ] && return

    line=$(df | grep "$1")
    [ -z "$line" ] && return

    echo $(echo $line | awk '{ print $6 }')
}

function print_with_color() {
    echo -e "\e[33m"$1"\e[0m"
}


# Check for root
print_with_color "Checking permissions..."
[ $(id -u) -ne 0 ] && echo "This script must be run as root!" && exit 1

# Check if MMC and eMMC are present
print_with_color "Checking MMC device..."
[ ! -e $MMC_DEVICE ] &&
    echo "MMC device \"$MMC_DEVICE\" is missing!" &&
    exit 1

print_with_color "Checking eMMC device..."
[ ! -e $EMMC_DEVICE ] &&
    echo "eMMC device \"$EMMC_DEVICE\" is missing!" &&
    exit 1


# Read format table of the MMC
print_with_color "Reading partition table.."
fdisk_output=$(fdisk $MMC_DEVICE -l | grep "^$MMC_DEVICE")
partitions_count=$(echo "$fdisk_output" | wc -l)
[ -z $partitions_count ] && echo "Failed to get MMC partition table!" && exit 1

# Make sure partition table on eMMC is not mounted and erased
print_with_color "Erasing eMMC partition table..."
for p in $(ls $EMMC_DEVICE*); do
    umount $p > /dev/null 2>&1
done
dd if="/dev/zero" of=$EMMC_DEVICE bs=1M count=20 > /dev/null 2>&1

# Create partitions and copy files
for part in $(seq $partitions_count); do
    # Read the first partition parameters
    partition=$(echo "$fdisk_output" | head -n $part | tail -n 1)

    # Check target fs
    fs=$(fsck -NT $MMC_DEVICE"p"$part | awk '{ print $5}' | \
        awk -F'.' '{ print $2 }')
    [ -z $fs ] && echo "Unknown target filesystem!" && exit 1

    # First read partiton start/end sector
    start=$(echo $partition | awk '{print $2}')
    end=$(echo $partition | awk '{print $3}')
    type=$(echo $partition | awk '{print $6}')

    [ $part -eq $partitions_count ] && end=""

    # If this is the first partition copy bootloader first
    if [ $part -eq 1 ]; then
        print_with_color "Copying bootloader..."
        dd if=$MMC_DEVICE of=$EMMC_DEVICE bs=512 skip=1 seek=1 count=$(($start-2)) > /dev/null 2>&1
    fi

    # Format
    print_with_color "Creating partition: $part"
    fdisk $EMMC_DEVICE > /dev/null 2>&1 << __EOF__
n
p
$part
$start
$end
t

$type
w
__EOF__

    # Make file system
    print_with_color "Formating partition $part to $fs..."
    mkfs.$fs $EMMC_DEVICE"p"$part > /dev/null 2>&1

    # Create mount points
    mkdir ./mmc > /dev/null 2>&1
    mkdir ./emmc > /dev/null 2>&1

    mount_point=$(check_mounted $MMC_DEVICE"p"$p)

     if [ -z "$mount_point" ]; then
        mount_point="./mmc"
        mount $MMC_DEVICE"p"$part $mount_point
     fi

    mount $EMMC_DEVICE"p"$part ./emmc

    # Copy files
    print_with_color "Copying files..."
    rsync -ah --info=progress2 $mount_point/* ./emmc

    umount $EMMC_DEVICE"p"$part > /dev/null 2>&1
    umount $MMC_DEVICE"p"$part > /dev/null 2>&1

    rm -rf ./mmc > /dev/null 2>&1
    rm -rf ./emmc > /dev/null 2>&1


done

print_with_color "Finished!"

exit 0
