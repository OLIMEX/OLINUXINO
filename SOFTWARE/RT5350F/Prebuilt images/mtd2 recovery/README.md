## SETTING MTD2 PARTITION (RT5350 FIRMWARE) 

### Did you manage to overwrite the MTD2 firmware partition of RT5350F-OLinuXino? Do not worry. You can restre the default firmware settings using the instructions below:

1.1 Make sure that your RT5350F-OLinuXino board is capable of booting successfully. Else you need to use one of the
methods described in the wiki article to restore the board to a successfully booting state.

1.2 Make sure that you are connected to the command line interface of the board and that that your RT5350F-OLinuXino
board is connected to the Internet either by using an Ethernet cable or the on-board WIFI. The Internet connection is
needed to download the three required files mentioned below with wget. If you can't access the Internet through the
board you can also manually download and copy the file to the mentioned folders (in this case you don't need Internet
connection through the RT5350 board)

The most reliable and easy setup: connect a serial cable for debugging at the UART0 connector; start terminal software
at 57600 baud; connect lan cable to LAN1 connector; provide 5V at the power jack; observe boot messages in the terminal
and press enter. The boot is successfull when you get root@RT5350F-OLinuXino:/

If you decide to use the WIFI server - note that it is disabled by default. You can check its current status with:

``` cat /etc/config/wireless ```

then you can enable it with:

``` uci set wireless.@wifi-device[0].disabled=0; uci commit wireless; wifi ```

### Once you have connection to internet please do the following:

2.1 Type the following commands (some of them take more time to complete, be patient)

``` cd /tmp ```

``` wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/RT5350F/Prebuilt%20images/mtd2%20recovery/openwrt-ramips-rt305x-rt5350f-olinuxino-evb-squashfs-sysupgrade-unlocked.bin ```

``` mtd write openwrt-ramips-rt305x-rt5350f-olinuxino-evb-squashfs-sysupgrade-unlocked.bin firmware ```

``` reboot ```

2.2 Type the following commands (some of them take more time to complete, be patient)

``` cd /tmp ```

``` wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/RT5350F/Prebuilt%20images/mtd2%20recovery/rt5350f_factory.eeprom ```

``` mtd write rt5350f_factory.eeprom factory ```

``` reboot ```

2.3 Type the following commands (some of them take more time to complete, be patient) - note that the sysupgrade file
below should be the latest official one released by Olimex (replace the link below with the latest file if there is newer)

``` cd /tmp ```

``` wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/RT5350F/Prebuilt%20images/mtd2%20recovery/openwrt-ramips-rt305x-olinuxino-rt5350f-squashfs-sysupgrade.bin ```

``` mtd write openwrt-ramips-rt305x-olinuxino-rt5350f-squashfs-sysupgrade.bin firmware ```

``` reboot ```
