# RT5350F Prebuild images

## Flash images
Images that can be uploaded directly into flash memory
using programmer, e.g. ARM-USB-OCD:
* __FlashImage_RT5350F-OLinuXino_DIN.bin__ - For RT5350F-OLinuXino-DIN
* __FlashImage_RT5350F-OLinuXino_EVN.bin__ - For RT5350F-OLinuXino and RT5350F-OLinuXino-EVB

## Bootloader image
Image that can be uploaded using the existing bootloader via serial port or TFTP.
* __bootloader.img__

## System upgrade
These images can be uploaded using the bootloader via TFTP
* __openwrt-ramips-rt305x-olinuxino-rt5350f-evb-squashfs-sysupgrade.bin__ - For RT5350F-OLinuXino-EVB
* __openwrt-ramips-rt305x-olinuxino-rt5350f-squashfs-sysupgrade.bin__ - For RT5350F-OLinuXino
* __openwrt-ramips-rt305x-rt5350f-olinuxino-din-squashfs-sysupgrade.bin__ - For RT5350F-OLinuXino-DIN

## Factory image
If you want image without openwrt you can use:
* __uboot+factory.bin__

