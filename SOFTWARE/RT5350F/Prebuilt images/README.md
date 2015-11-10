# RT5350F Prebuild images

## Changes

### 10 NOV 2015

* Fixed issue with MAC address. They were hard-coded in the 
factory partition. Now on the first boot a random one is 
generated and stored.

### 24 AUG 2015

* Now modules for USB storage are [*] instead of [M]

### 17 AUG 2015

* Changed OpenWrt repository url from snapshots to Chaos Calmer 15.05-rc3
* Updated WiFi led polarity
* Added separate images for OLinuXino-RT5350F and EVB. Difference is that on
the EVB the button and the two relays are exported on boot time.
