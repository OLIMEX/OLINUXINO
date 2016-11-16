# RT5350F Prebuild images

## Description

### Flash images
Images that can be uploaded directly into flash memory
using programmer, e.g. ARM-USB-OCD:
* __FlashImage_RT5350F-OLinuXino_DIN.bin__ - For RT5350F-OLinuXino-DIN
* __FlashImage_RT5350F-OLinuXino_EVN.bin__ - For RT5350F-OLinuXino and RT5350F-OLinuXino-EVB

### Bootloader image
Image that can be uploaded using the existing bootloader via serial port or TFTP.
* __bootloader.img__

### System upgrade
These images can be uploaded using the bootloader via TFTP
* __openwrt-ramips-rt305x-olinuxino-rt5350f-evb-squashfs-sysupgrade.bin__ - For RT5350F-OLinuXino-EVB
* __openwrt-ramips-rt305x-olinuxino-rt5350f-squashfs-sysupgrade.bin__ - For RT5350F-OLinuXino
* __openwrt-ramips-rt305x-rt5350f-olinuxino-din-squashfs-sysupgrade.bin__ - For RT5350F-OLinuXino-DIN

### Factory image
If you want image without openwrt you can use:
* __uboot+factory.bin__


## Changes

### 01 NOV 2016

* Added images and instructions suitable for restoring the firmware of RT5350F-OLinuXino and RT5350F-OLinuXino-EVB (mtd2 partition restore)

### 03 MAR 2016

* Added image RT5350F-OLinuXino-DIN
* Renamed uboot+factory+openwrt.bin to FlashImage_RT5350F_OLinuXino_EVB.bin
* Added FlashImage_RT5350F_OLinuXino_DIN.bin

### 10 NOV 2015

* Fixed issue with MAC address. They were hard-coded in the 
factory partition. Now on the first boot a random one is 
generated and stored.

The updated files are:
- uboot+factory.bin
- uboot+factory+openwrt.bin

The difference is ONLY in the factory partition. OpenWRT remains the same.
The MAC address was hard-coded at offset 0x40004. We set WLAN,
MAC0 and MAC1 MAC addresses to ff:ff:ff:ff:ff. This forces the chip to
generate random one on every boot. On the first boot this MAC is 
stored on the filesystem.

### 24 AUG 2015

* Now modules for USB storage are [*] instead of [M]

### 17 AUG 2015

* Changed OpenWrt repository url from snapshots to Chaos Calmer 15.05-rc3
* Updated WiFi led polarity
* Added separate images for OLinuXino-RT5350F and EVB. Difference is that on
the EVB the button and the two relays are exported on boot time.
