## SETTING MTD2 PARTITION (RT5350 FIRMWARE)

### Did you manage to overwrite the MTD2 firmware partition of RT5350F-OLinuXino? Do not worry. You can restore 
the default firmware settings using the instructions below:

1.1 Make sure that your RT5350F-OLinuXino board is capable of booting successfully. Else you need to use one of the
methods described in the wiki article to restore the board to a successfully booting state. The MTD2 partition contains 
firmware constants and calibration variables - these depend on the hardware revision. 

The fix is suitable for RT5350F-OLinuXino boards with the following hardware characteristics:

- quartz Q1: 20MHz/9pF/10ppm in (5x3.2)mm package 
- capacitor C49: NA (Not Assembled) - the component is not placed
- capacitor C50: 22pF in 0603 package

If your board has different components please either do not use this guide or adjust the hardware accordingly.

1.2 Make sure that you are connected to the command line interface of the board and that that your RT5350F-OLinuXino
board is connected to the Internet either by using an Ethernet cable or the on-board WIFI (if possible). The Internet connection
is needed to download the three required files mentioned below with wget. If you can't access the Internet through the board you
can also manually download and copy the file to the mentioned folders (in this case you don't need Internet connection through the
RT5350 board).

The most reliable and easy setup: connect a serial cable for debugging at the UART0 connector; start terminal software
at 57600 baud; connect Ethernet cable to LAN1 connector; provide 5V at the power jack; observe boot messages in the terminal
and press enter. The boot is successfull when you get root@RT5350F-OLinuXino:/

If you decide to use the WIFI - note that it is typically disabled by default. You can check its current status with:

```shell
cat /etc/config/wireless
```

then you can enable it with:

```shell
uci set wireless.@wifi-device[0].disabled=0; uci commit wireless; wifi
```

### Once you have connection to internet please do the following:

2.1 We have to change the default locked image. It is not possible to change MTD2 partition with the original image, due
to security reasons. Type the following commands (some of them take more time to complete, be patient):

```shell
opkg update
```

```shell
opkg install ca-certificates
```

The two commands above update the SSL certifactes that allow us to reach the files below over https with wget

```shell
cd /tmp
```

```shell
wget ftp://staging.olimex.com/RT5350/openwrt-ramips-rt305x-rt5350f-olinuxino-evb-squashfs-sysupgrade-unlocked.bin
```


```shell
mtd write openwrt-ramips-rt305x-rt5350f-olinuxino-evb-squashfs-sysupgrade-unlocked.bin firmware
```

This installs temorary unlocked image that allows the modificiation of MTD2 partition.

```shell
reboot
```

2.2 The board now runs the modified unlocked image and we can update the MTD2 region. Type the following commands
(some of them take more time to complete, be patient)

```shell
opkg update
```

```shell
opkg install ca-certificates
```

Since it is a new image we need to get the SSL certifactes that allow us to reach the files below over https AGAIN.

```shell
cd /tmp
```

```shell
wget ftp://staging.olimex.com/RT5350/rt5350f_factory.eeprom
```

```shell
mtd write rt5350f_factory.eeprom factory
```

```shell
reboot
```

2.3 We have succesfully modified the MTD2 partition with the firmware, and now we want to get rid of the unlocked
image and revert to the official locked one. Type the following commands (some of them take more time to complete,
be patient) - note that the sysupgrade file below should be the latest official one released by Olimex (replace
the link below with the latest file if there is newer)

```shell
cd /tmp
```

```shell
wget ftp://staging.olimex.com/RT5350/openwrt-ramips-rt305x-olinuxino-rt5350f-squashfs-sysupgrade.bin
```

```shell
mtd write openwrt-ramips-rt305x-olinuxino-rt5350f-squashfs-sysupgrade.bin firmware
```

```shell
reboot
```


