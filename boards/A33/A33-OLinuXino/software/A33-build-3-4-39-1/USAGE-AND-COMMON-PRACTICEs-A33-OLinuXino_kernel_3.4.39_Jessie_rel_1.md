# Advice and common practices for A33-OLinuxino booard

## Card preparation

There are three ways to get a working Linux card.
First, you can download the official Debian image from the wiki article of the board and download it to a microSD card.
Each board has own wiki article and specific Debian image, so make sure that you are getting the proper Debian image from the correct wiki article.
Alternatively, you can purchase a card with Debian on it.
Again, make sure that the card is suitable for the board that you purchased.
If you have doubts, drop us an e-mail or post in the forums.

The third way of getting a working microSD is to prepare the image from scratch.
The step-by-step guide on how to do it can be found in the project git repository.

Note: We sell microSD cards with pre-loaded official Debian that you just plug and use.
These are 8GB class 10 fast cards.
If you are going to download the image and prepare a microSD card please use at least 8GB card with similar speed card (or faster) or the performance of Linux will slow down.
The latest official image is Debian Jessie Linux with mainline u-boot and kernel 3.4.39+.

All images are compressed into an archive.
Before you can download the image to the microSD card you need to extract the archive and you need an external microSD card reader.
To extract the file:

### Using Windows

Use 7zip and then use [Win32DiskImager.exe](http://sourceforge.net/projects/win32diskimager/) for image writing.

### Using Linux

Use p7zip package.
If you have no installed 7zip then type:

```bash
apt-get install p7zip
```

Copy the archived image to your directory and unzip it with:

```bash
7za e a33-XXX.7z
```

where a33-XXX.7z must be replaced with the name of the image that you downloaded.

The output should be a new file named ``a33-XXX.img``.

Insert a suitable microSD card (at least 8GB, preferably class 10 speed) in your card reader and type:

```bash
ls /dev/sd
```

Then press <TAB> twice.
You will see a list of your sd devices like sda, sdb, sdc, etc.
Note that some of these devices may be your primary hard disk, so make sure you know which one is your microSD card before you proceed!
Otherwise you can easily destroy the data on your HDD if you choose the wrong sd-device.
You can determine which is the card by unplugging your sd card reader and identify which "sd" devices remove from the list.
Once you know which device is your sdcard like sda use this text instead of the sdX name in the references below:

```bash
dd if=a33-XXX.img of=/dev/sdX
```

Note that the image is 8GB and if you write it on bigger SD card it is reccomended to resize it in order to use all SD card space.
From root directory start resize_sd.sh script with the following parameters:

```bash
./resize_sd.sh /dev/mmcblk0 1
```

## Default user name and password

Under the command line interface you are automatically logged as super user (user ``root``, password ``olimex``).
However, under the graphical environment you are not auto-logged as super user and you must type ``sudo`` before the command (in the GUI the super-user is ``olimex`` and the password is ``olimex``).

## Editing configurations - changing script.bin file

Note that you have to be very careful with script.bin changing because the system can stop working.
It is recommeded to make a backup of script.bin file before changing it.

If you want to convert script.bin file from sdcard to script.fex file, you must first convert it using the bin2fex tool.
The script.bin file is located in /boot directory.

```bash
bin2fex /boot/script.bin script.fex
```

Now you can change the board modules and parameters, save the changes and exit from nano editor:

```bash
nano script.fex
save and exit
```

Now you have to convert script.fex to script.bin and replace it:

```bash
fex2bin script.fex /boot/script.bin
```

Then reboot: 

```bash
reboot
```

## Hardware tested, interfaces usage and instructions

Note that not all A33 boards have all the hardware described below.
The main tested onboard hardware modules are:

- GPIOs: these are located in /sys/class/gpio directory.
Note that first you have to export GPIOs.
For example, to exporte export all available GPIOs:

```bash
for i in `seq 1 1 230`; do echo $i > /sys/class/gpio/export; done
```

To set a port as output:

```bash
echo out > /sys/class/gpio/gpioXX_pxx/direction
```

To set a port in high level (locical 1):

```bash
echo 1 > /sys/class/gpio/gpioXX_pXX/value
```

- LCD: 5"(800x480) with capacitive touchscreen controler ft5x

```bash
nano /usr/share/X11/xorg.conf.d/10-evdev.conf
```

- [ASIX8877 USB-LAN](https://www.olimex.com/Products/OLinuXino/A13/USB-ETHERNET-AX88772B): usually it is eth1 interface
- [WIFI-RTL8188](https://www.olimex.com/Products/Modules/Ethernet/MOD-WIFI-RTL8188): you have to set your SSID network and your password in /etc/network/interfaces file
- [MOD-WIFI-R5370](https://www.olimex.com/Products/USB-Modules/MOD-WIFI-R5370): you have to set your SSID network and your password in /etc/network/interfaces file
- MINI-USB - It is configured only like HOST
- USB-HOST on GPIO connector: low/full/high USB hosts
- micoSD card interface: Linux boots from this microSD card usually; 

- MICROPHONE INPUT and HEADPHONES OUTPUT: to stream the audio from the MICROPHONE jack to the HEADPHONE jack use:

```bash
arecord -f S16_LE | aplay
```

- NAND support: /dev/nandX.
Note that you have to load nand.ko module located in ``/lib/modules/3.4.39+/kernel/drivers/``.
For example:

```bash
modprobe nand
```

or

```bash
insmod /lib/modules/3.4.39+/kernel/drivers/
```

Unfortunately currently there is no other method to prepare the NAND except for programing with android image using phoenix suit first. 

- I2C1(100KHz) - /dev/i2c-0 - SCL pin - PH02; SDA pin - PH03 (pins #36 and #34 of the GPIO connector)
- I2C2(100KHz) - /dev/i2c-1 - SCL pin - PH04; SDA pin - PH05 (pins #32 and #30 of the GPIO connector)
- UART0 (debug) - /dev/ttyS0 - TX pin - PB00; RX pin - PB01 (at available at UART0 header)
- UART1 - /dev/ttyS1 - TX pin - PG06; RX pin - PG07 (pins #25 and #27 of the GPIO connector)

- BUTTONs - we use evtest package for testing. To start it:

```bash
evtest
```

then enter event number for keyboard and now you can press the BUTTONs and you should see the result on terminal window.

- on-board camera support (only for A20-SOM-EVB) - you can do a simple camera test if you paste the next command:

```bash
mplayer -fps 60  -tv driver=v4l2:width=640:height=480:device=/dev/video0 tv://
```

and see camera stream to fremebufer output you set - HDMI, VGA or LCD

- USB gadgets - you can use mini USB connector to connect with the board via SSH. Note that USB0 interface IP address by default  is 192.168.2.1 Therefore the host IP address has to be in this network - 192.168.2.xxx(except 192.168.2.1). Use login:olimex and password:olimex to connect with the board.

- Video playing with full hardware acceleration. Open a console window in X environment. Type:

```bash
export VDPAU_DRIVER=sunxi
mpv --vo=vdpau --hwdec=vdpau --hwdec-codecs=all test.mp4
```

where test.mp4 is the video file name.
