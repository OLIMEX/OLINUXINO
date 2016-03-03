# RT5350F Prebuild images

## Changes

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
