# Creating a bootable SD card for Olimex's A20-SOM, A20-OLinuXino-MICRO, A20-OLinuXino-LIME, A20-OLinuXino-LIME2 with kernel 3.4.90 and Linux Debian file system

## Toolchain setup

You should make sure that you have the tools needed for building the Linux Kernel.
You'd have to install them if you don’t have them already installed.
To install new software you should be logged to a user with super user rights on your Linux machine.
Type in the terminal:

```bash
sudo su
```

You will be asked for your password and then your prompt will change to # which means you are now the super user.
All future commands should be run in this mode.

Next update apt-get links by typing:

```bash
apt-get update
```

Next install the toolchain by typing the following:

```bash
apt-get install gcc-4.7-arm-linux-gnueabihf ncurses-dev uboot-mkimage build-essential git
```

This will install:

- GCC compiler used to compile the kernel
- Kernel config menu
- uBoot make image (required to allow the SD card to boot into the Linux image
- Git that allows you to download from the git repository which holds source code for some of the system
- Other tools for building the kernel

Note that if you use Debian, you may need to add:

```shell
deb http://www.emdebian.org/debian squeeze main
```

into the file:

```bash
/etc/apt/sources.list
```

You would also need a torrent client.
It is needed to be able to download the file system.
Any torrent that suits your needs would do the job as long as it can load torrent files.

Now you have all important tools to make your very own A20 kernel image!

## Building u-boot

First let's make the directory where we would build the A20 Linux:

```bash
mkdir a20-olimex
cd a20-olimex
```

Download the u-boot sources from the git repository.
Note there are lot of branches; use the sunxi branch.
The u-boot is tested with the following branch:

```bash
git rev-parse --verify HEAD 36080eb05e9a1e96d58e3168631d3cc9c612a0e3
```

Download u-boot sources:

```bash
git clone -b sunxi https://github.com/linux-sunxi/u-boot-sunxi.git
```

After the download you should have a new directory:

```bash
cd u-boot-sunxi/
```

With the following commands you have to configure the uboot for the different boards.
Only use the make command for your board:

### A20-OLinuXino_Lime2 board

```bash
make A20-OLinuXino_Lime2_config ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
```

### A20-SOM board

Note that there are 2 types of A20-SOM boards.
The main differences are in DDR3 memory bus speed. 

A20-SOM up to rev.B  - DDR3 memory bus speed is 384MHz(6 layer PCB)

A20-SOM after rev.D - DDR3 memory bus speed is 480MHz(8 layer PCB)

#### A20-SOM up to rev.B

```bash
make A20-SOM_config ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
```

#### A20-SOM after rev.D

You can use the u-boot settings for A20-Lime2:

```bash
make A20-OLinuXino_Lime2_config ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
```

### A20-OLinuXino-MICRO board

```bash
make A20-OLinuXino_MICRO_config ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
```

### A20-OLinuXino-LIME board

```bash
make A20-OLinuXino-Lime_config ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
```

Now type the following command for u-boot building

```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
```

At the end of the process you can check if everything is OK by

```bash
ls u-boot.bin u-boot-sunxi-with-spl.bin spl/sunxi-spl.bin
```

Output:

```
spl/sunxi-spl.bin  u-boot.bin  u-boot-sunxi-with-spl.bin
```

Go up a directory:

```bash
cd ..
```

You should be in the following directory:

```shell
/home/user/a20-olimex#
```

## Building kernel sources for A20 boards

Kernel sources for A20 are available within the git repository.
Note that the following building is made with the revision below: 

```bash
git rev-parse --verify HEAD e37d760b363888f3a65cd6455c99a75cac70a7b8
```

You can download the kernel sources using the following command:

```bash
git clone https://github.com/linux-sunxi/linux-sunxi
```

After the download go to the kernel directory:

```bash
cd linux-sunxi/
```

### Adding SPI support

If you don't need SPI, you can skip this section.

Download the ``spi-sun7i.c`` file using wget:

```bash
wget https://raw.githubusercontent.com/OLIMEX/OLINUXINO/master/SOFTWARE/A20/A20-build-3.4.90/spi-sun7i.c
```

and copy it in drivers/spi directory:

```bash
cp spi-sun7i.c drivers/spi 
```

Download the patch file SPI.patch using wget:

```bash
wget https://raw.githubusercontent.com/OLIMEX/OLINUXINO/master/SOFTWARE/A20/A20-build-3.4.90/SPI.patch
```

and apply the patch:

```bash
patch -p0 < SPI.patch
```

### Configure the system 

Here you need from a20 configuration file ``a20_olimex_defconfig``.
The file contains all kernel module settings.

Download ``a20_olimex_defconfig`` using wget:

```bash
wget https://raw.githubusercontent.com/OLIMEX/OLINUXINO/master/SOFTWARE/A20/A20-build-3.4.90/a20_olimex_defconfig
```

Copy ``a20_olimex_defconfig`` to configs directory:

```bash
cp a20_olimex_defconfig  arch/arm/configs/
```

and make:

```bash
make ARCH=arm a20_olimex_defconfig 
```

The result should be:

  configuration written to .config

If you wish to make your changes in the kernel configuration:

```bash
make ARCH=arm menuconfig
```

The menuconfig changes a .config text file, which you can view/edit even with a text editor like vi or nano.
With this command you can add or remove different modules for the different peripherials in the kernel.
Be careful when you change these settings because the kernel can stop working properly.

Note that if you want to have SPI support then you have to enable Device driver > SPI support > <*>SUN7I SPI Controller 
                                                                                                [*] SUN7I SPI Normal DMA mode select

Now you can continue with kernel image compiling 

```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j4 uImage
```

when this completes, you will have a uImage ready and the result should be:

```
Kernel: arch/arm/boot/zImage is ready
UIMAGE  arch/arm/boot/uImage
Image Name:   Linux-3.4.90+
Created:      Tue Aug 19 16:23:22 2014
Image Type:   ARM Linux Kernel Image (uncompressed)
Data Size:    4596072 Bytes = 4488.35 kB = 4.38 MB
Load Address: 40008000
Entry Point:  40008000
Image arch/arm/boot/uImage is ready
```

The next step is kernel modules building:

```bash
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j4 INSTALL_MOD_PATH=out modules
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j4 INSTALL_MOD_PATH=out modules_install
```

DONE!
At this point you have uboot and kernel modules.

The uImage file is located in ``linux-sunxi/arch/arm/boot/``

The kernel modules are located in ``linux-sunxi/out/lib/modules/3.x.xx`` where 3.x.xx is kernel version.
In our case the directory with modules is:

```
  linux-sunxi/out/lib/modules/3.4.90+
```

## Format and setup the SD-card

We suggest class 10 microSD card with memory capacity greater than 2GB.
We haven't tested microSD cards larger than 16GB.

We must first create the correct card partitions.
This is done with fdisk.

Plug SD card into your SD card reader and enter in the terminal:

```bash
ls /dev/sd
```

Then press <TAB> two times.
You will see a list of your sd devices (sda, sdb, sdc, etc).
Note that some of these devices may be your primary hard disk, so ensure you know which one is your sd card before you proceed.
You can easily damage your HDD data if you choose the wrong sd-device.
You can see which is which by unplugging your sd card reader, paying attention which "sd" device disappears from the list.

Once you know which one is your microSD (as sda#), use it instead of the sdX name in the references below:

```bash
fdisk /dev/sdX
```

then:

```
p
```

will list your partitions.
If there are already partitions on your card do:

```
d
enter
1
```

if you have more than one partitition press d while delete them all

Create the first partition, starting from 2048:

```
n
enter
p
enter
1
enter
enter
+16M
```

Create the second partition

```
n
enter
p
enter
2
enter
enter
enter
```

then list the created partitions:

```
p
enter
```

If you did everything correctly on 4GB card you should see something like:

```
Disk /dev/sdg: 3980 MB, 3980394496 bytes
123 heads, 62 sectors/track, 1019 cylinders, total 7774208 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0x00000000

   Device Boot      Start         End      Blocks   Id  System
/dev/sdg1            2048       34815       16384   83  Linux
/dev/sdg2           34816     7774207     3869696   83  Linux
```

Press ``w`` to write changes to sd card.

Now we have to format the file system on the card (the first partition should be vfat as this is FS which the Allwinner bootloader understands):

```bash
mkfs.vfat /dev/sdX1
```

The second should be normal Linux EXT3 FS:

```bash
mkfs.ext3 /dev/sdX2
```

## Write the Uboot and u-boot-sunxi-with-spl.bin

You should be in ``/home/user/a20-olimex#`` directory.

Note that you have to write u-boot-sunxi-with-spl.bin in /dev/sdX (not sdX1 or sdX2).

Type:

```bash
dd if=u-boot-sunxi/u-boot-sunxi-with-spl.bin of=/dev/sdX bs=1024 seek=8
```

## Write kernel uImage you build to the SD-card

You should be in the directory below

```
/home/user/a20-olimex#
```

Run:

```bash
mkdir /mnt/sd
mount /dev/sdX1 /mnt/sd
```

Now copy the Kernel uImage to root directory in partition 1:

```bash
cp linux-sunxi/arch/arm/boot/uImage /mnt/sd
```

## Write script.bin file

``script.bin`` is a file with very important configuration parameters.
THese include port GPIO assignments, DDR memory parameters, video resolution etc.
According to the A20 board you use you have to download and write a different script.bin file.

### A20-OLinuXino_Lime2

Download the script.bin file using wget:

```bash
wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/A20/A20-build-3.4.90/scripts_a20_Lime2_34_90_camera_rel_2/script.bin
```

### A20-SOM

Download the script.bin file using wget:

```bash
wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/A20/A20-build-3.4.90/A20-SOM-3.4.90_camera_scripts_rel_3/script_a20_SOM_HDMI_720p50/script.bin
```

### A20-OLinuXino-MICRO

Download the script.bin file using wget:

```bash
wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/A20/A20-build-3.4.90/script_a20_OLinuXino-micro_3.4.90_camera_rel_10/script.bin
```

### A20-OLinuXino-LIME

Download the script.bin file using wget:

```bash
wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/A20/A20-build-3.4.90/script_a20_lime_3.4.90_camera_rel_3/script.bin
```

Copy the downloaded script.bin file to the mounted first partition of the SD card:

```bash
cp script.bin /mnt/sd
sync
umount /dev/sdX1
```

## Debian rootfs

The Linux Kernel and Uboot are ready, now we have need from Linux distribution rootfs.

Basically the only difference between the different Linux distributions is the rootfs.
If you put Debian rootfs you will have Debian; if you put Ubuntu rootfs it will be Ubuntu etc.

How to build one is a long topic, but the good thing is that there are many already pre-built so we can just download one and use it.

Now leave the kernel directory:

```bash
cd ..
```

You should now be in the directory below:

```
/home/user/a20-olimex
```

Download Debian rootfs with the file name ``debian_FS_34_90_camera_A20-olimex.tgz``, which is available only as a torrent.
You would need a torrent client for it (Azureus, uTorrent, qBittorrent, etc).

The link to the torrent file is [here](https://www.olimex.com/wiki/images/2/29/Debian_FS_34_90_camera_A20-olimex.torrent).

Now mount the microSD card EXT3 FS partition:

```bash
mount /dev/sdX2 /mnt/sd
```

and unarchive the rootfs:

```bash
tar debian_FS_34_90_camera_A20-olimex.tgz -C /mnt/sd
ls /mnt/sd
```

The correct result should be:

```
bin   dev  home  lost+found  mnt  proc  run   selinux  sys  usr
boot  etc  lib   media       opt  root  sbin  srv      tmp  var
```

Now you have to replace the new generated kernel modules from /home/user/a20-olimex/linux-sunxi/out/lib/modules/ to the new Debian file system:

```bash
rm -rf /mnt/sd/lib/modules/*
cp -rfv linux-sunxi/out/lib/modules/3.x.xx+/ /mnt/sd/lib/modules/
```

where x.xx is the kernel version.
In our case:

```bash
cp -rfv linux-sunxi/out/lib/modules/3.4.90+/ /mnt/sd/lib/modules/
```

Replace /lib/firmware folder with the generated /linux-sunxi/out/firmware:

```bash
cp -rfv linux-sunxi/out/lib/firmware/ /mnt/sd/lib/
sync
umount /mnt/sdX2
```

Now at this point, you have Debian on your SD card's second partition and you have an SD card ready to boot Debian on A20-SOM or A20-OLinuXino-MICRO or A20-OLinuXino-LIME or A20-OLinuXino-LIME2.
Connect USB-SERIAL-CABLE-F to UEXT Tx, Rx and GND, or connect a HDMI screen.
Insert the SD-card.
Apply power according to A20 board type.
You should see u-boot logs, followed by kernel messages on the console.

The default username/password is: ``root`` / ``olimex``.
