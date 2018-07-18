# How to make a bootable SD card for A13-OLinuXino and A13-OLinuXino-WIFI with kernel 3.4.90 and Debian file system

## Setup of the toolchain

You should make sure you have the tools for building the Linux Kernel and install them if you don't have them. To install new software you should be with super user rights on your Linux machine, so do this type in a terminal.

  sudo su

You will be asked for your password and then your prompt will change to # which means you are now the
super user, all future commands should be run in this mode.

First update apt-get links by typing

  apt-get update

Now install the toolchain by typing the following:

  apt-get install gcc-4.7-arm-linux-gnueabihf ncurses-dev uboot-mkimage build-essential git

This will install: GCC compiler used to compile the kernel, the kernel config menu uboot make image
which is required to allow the SD card to boot into the Linux image, Git that allows you to download
from the github which holds source code for some of the system, some other tools for building the kernel

Note that if you use debian may be you will need to add

  deb http://www.emdebian.org/debian squeeze main

in the file below:

  /etc/apt/sources.list

After the installation you now have all tools to make your very own A13 kernel image!

## Building Uboot

Here you have two options.
The first is to use sunxi u-boot and the second is to use mainline u-boot. 

The sunxi u-boot is suitable for the boards that have HYNIX DDR3 chip - H5TQ2G83CFR.
The mainline u-boot is suitable for boards that have SAMSUNG DDR3 chip - K4B2G0846Q.

Note that if you use the sunxi u-boot with SAMSUNG DDR3 memory then you can have different problems because the new SAMSUNG DDR chip is not supported in this u-boot.

The mainline u-boot will be suitable for HYNIX and SAMSUNG DDR3.

First let's make the directory where we will build the A13-OLinuXino linux:

  mkdir a13-olinuxino-kernel_3.4.90
  cd a13-olinuxino-kernel_3.4.90

Follow either branch 2.1 or 2.2 depending on the memory your board has.

### Building sunxi u-boot 

Let's download the uboot sources from the GitHub repository.
Note there are lot of branches but you have to use sunxi branch.
ALso note that the u-boot for A13-OLinuXino is tested with the next branch:

  git rev-parse --verify HEAD

4af825f11fbdac9e9c5560b2532474da6203cb53

Download u-boot sourses:

  git clone -b sunxi https://github.com/linux-sunxi/u-boot-sunxi.git

After the download you should have a new directory:

  cd u-boot-sunxi/

You can start uboot building with the next commands:

  make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- A13-OLinuXino_config
  make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-

At the end of the process you can check if everything is OK with:

  ls u-boot.bin u-boot-sunxi-with-spl.bin spl/sunxi-spl.bin

spl/sunxi-spl.bin  u-boot.bin  u-boot-sunxi-with-spl.bin

If you got these files everything is complete, well done so far

  cd ..

You should be in the following directory:

/home/user/a13-olinuxino-kernel_3.4.90#

skip to point 3.

### Building mainline u-boot

Then let's download the uboot sources from GitHub repository.
Note that the u-boot for A13-OLinuXino is tested with the next branch:

  git rev-parse --verify HEAD

1733259d25015c28c47990ec11af99b3f62f811c

Download u-boot sourses:

  git clone git://git.denx.de/u-boot.git

After the download you should have a new directory

  cd u-boot

Load the configuration file for A13-OLinuXino.

  make CROSS_COMPILE=arm-linux-gnueabihf- A13-OLinuXino_defconfig

you can configure some u-boot settings like DDR3 settings, clocks and other with:

  make menuconfig

It is recommended to choose:

[*]Enable workaround for booting old kernels

The board has stable behaviour with

```
[408] sunxi dram clock speed 
[204] sunxi mbus clock speed
```

...but you can choose other settings depending from your application.
Save it the settings and exit.

Build u-boot:

  make CROSS_COMPILE=arm-linux-gnueabihf-

When the build has completed, there will be u-boot-sunxi-with-spl.bin available in your u-boot tree. 

Now you have to create a new ``boot.cmd`` file with the following contents:

  setenv bootm_boot_mode sec
  setenv bootargs console=ttyS0,115200 root=/dev/mmcblk0p2 rootwait panic=10
  load mmc 0:1 0x43000000 script.bin || load mmc 0:1 0x43000000 boot/script.bin
  load mmc 0:1 0x42000000 uImage || load mmc 0:1 0x42000000 boot/uImage
  bootm 0x42000000

Convert boot.cmd in boot.scr with the next command

  mkimage -C none -A arm -T script -d boot.cmd boot.scr
 
The result should be a new boot.scr file that you have to copy to the first SD card partitition(where uImage and script.bin file are located).

## Building kernel sources for A13-OLinuXino

Kernel sources for A13 are available on GitHub.
Note that the following building is made with the revision below: 

  git rev-parse --verify HEAD

e37d760b363888f3a65cd6455c99a75cac70a7b8

You can download the kernel sources using the following command:

  git clone https://github.com/linux-sunxi/linux-sunxi

After the download go to the kernel directory

  cd linux-sunxi/

The next step is to configure the system 

Here you need from a13 configuration file – a13_linux_defconfig.
The file contains all kernel module settings.

Download a13_linux_defconfig using the wget command:

  wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/A13/A13-build/a13_linux_defconfig

then copy a13_linux_defconfig file to configs directory:

  cp a13_linux_defconfig /arch/arm/configs/.

and compile:

  make ARCH=arm a13_linux_defconfig

The result should be:

configuration written to .config

If you wish to make your changes in the kernel configuration do:

  make ARCH=arm menuconfig

The menuconfig changes a .config text file, which you can view/edit even with a text editor like vi,nano.
With this command you can add or remove different modules for the different peripherials in the kernel.
Be careful when use this as this may cause the kernel to not work properly.

Now you can continue with kernel image compiling 

  make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j4 uImage

when this finishes, you will have uImage ready and the result should be:

  OBJCOPY arch/arm/boot/zImage
  Kernel: arch/arm/boot/zImage is ready
  UIMAGE  arch/arm/boot/uImage
  Image Name:   Linux-3.4.90+
  Created:      Thu Jul  3 16:00:03 2014
  Image Type:   ARM Linux Kernel Image (uncompressed)
  Data Size:    4340720 Bytes = 4238.98 kB = 4.14 MB
  Load Address: 40008000
  Entry Point:  40008000
  Image arch/arm/boot/uImage is ready

Now you can build the kernel modules:

  make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j4 INSTALL_MOD_PATH=out modules
  make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j4 INSTALL_MOD_PATH=out modules_install

**DONE!**
At this point you have uboot and kernel modules.

The uImage file is located in ``linux-sunxi/arch/arm/boot/``

The kernel modules are located in

  linux-sunxi/out/lib/modules/3.x.xx
  where 3.x.xx is kernel version

in our case the directory with modules is:

  linux-sunxi/out/lib/modules/3.4.90+

## Format and setup the SD-card

We suggest a 4GB class 10 MicroSD card, but you may use any card between 2GB and 16GB.

First we have to make the correct card partitions; this is done with fdisk.

Plug the SD card into your SD card reader and enter in the terminal:

  ls /dev/sd

Then press <TAB> two times and you will see a list of your sd devices like sda sdb sdc.
Note that some of these devices may be your hard disk so make sure you know which one is your sd card before you proceed as you can damage your HDD if you choose the wrong sd-device.
You can do this by unplugging your sd card reader and identify which ``sd`` devices remove from the list.

Once you know which device is your sdcard like sda use this text instead of the sdX name in the references below:

  fdisk /dev/sdX

then do these steps:

  p

will list your partitions.

If there are already partitions on your card, run:

  d
  enter
  1

if you have more than one partition, pressing d will delete them all.

Create the first partition, starting from 2048

  n
  enter
  p
  enter
  1
  enter
  enter
  +16M

Create second partition:

  n
  enter
  p
  enter
  2
  enter
  enter
  enter

Now list the created partitions:

  p
  enter

If you did everything correctly (we used a 4GB card), you should see something like:

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

  w

write changes to sd card.
Now we have to format the file system on the card:

The first partition should be ``vfat`` as this is the type of filesystem that the Allwinner bootloader understands:

  mkfs.vfat /dev/sdX1

the second should be normal Linux EXT3 FS

  mkfs.ext3 /dev/sdX2


## Write the Uboot and sunxi-spl.bin

You should be in the ``/home/user/a13-olinuxino-kernel_3.4.90#`` directory.
Note that you have to write u-boot-sunxi-with-spl.bin in /dev/sdX (not sdX1 or sdX2).

  dd if=u-boot-sunxi/u-boot-sunxi-with-spl.bin of=/dev/sdX bs=1024 seek=8

Note that if you use mainline u-boot then you have to copy the generated boot.scr file in the first sdcard partition. For example:

  mkdir /mnt/sd
  mount /dev/sdX1 /mnt/sd
  cp /u-boot/boot.scr /mnt/sd/

## Write kernel uImage you build to the SD-card

You should be in the ``/home/user/a13-olinuxino-kernel_3.4.90#`` directory.

  mkdir /mnt/sd
  mount /dev/sdX1 /mnt/sd

Copy the Kernel uImage to root directory in partition 1:

  cp linux-sunxi/arch/arm/boot/uImage /mnt/sd

## Write script.bin file

``script.bin`` is a file with very important configuration parameters like port GPIO assignments, DDR memory parameters, Video resolution etc.
Download the script.bin using wget command:

  wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/A13/A13-build/script.bin
  cp script.bin /mnt/sd
  sync
  umount /dev/sdX1

## Debian rootfs

The Linux Kernel and Uboot are now ready.
Now we require the Linux distribution rootfs.
Basically, the only difference between the different Linux distributions is the rootfs, so if you put Debian rootfs you will have Debian, if you put Ubuntu rootfs it will be Ubuntu, etc.

How to build one is a long topic.
The good thing is that there are many already pre-built so we can just download one and use it!

Exit the kernel directory

  cd ..

You should be in the directory ``/home/user/a13-olinuxino-kernel_3.4.90``

Download the Debian rootfs - a13_olinuxino_debian_fs_kernel_34_90_rel_10.tgz using the wget command:

  wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/A13/A13-build/a13_olinuxino_debian_fs_kernel_34_90_rel_10.torrent

Mount your sd card EXT3 FS partition:

  mount /dev/sdX2 /mnt/sd

and unarchive the rootfs:

  tar xzvf a13_olinuxino_debian_fs_kernel_34_90_rel_10.tgz -C /mnt/sd
  ls /mnt/sd

The right result should be:

```
bin   dev  home  lost+found  mnt  proc  run   selinux  sys  usr
boot  etc  lib   media       opt  root  sbin  srv      tmp  var
```

Now you have to replace the new generated kernel modules from /home/user/a13-olinuxino-kernel_3.4.90linux-sunxi/out/lib/modules/ to the new debian file system

  rm -rf /mnt/sd/lib/modules/*
  cp -rfv linux-sunxi/out/lib/modules/3.x.xx+/ /mnt/sd/lib/modules/

where x.xx is the kernel version in our case:

  cp -rfv linux-sunxi/out/lib/modules/3.4.90+/ /mnt/sd/lib/modules/

replace /lib/firmware folder with the generated /linux-sunxi/out/firmware:

  rm -rf /mnt/sd/lib/firmware/
  cp -rfv linux-sunxi/out/lib/firmware/ /mnt/sd/lib/

  sync
  umount /mnt/sdX2

At this point you have Debian on your SD card second partition and you have an SD card ready to boot debian on A13-OLinuXino.

Connect USB-SERIAL-CABLE-F to UEXT Tx.Rx and GND, or connect a HDMI screen.
Put the SD-card in A13-OLinuXino and apply 6-16V power, you should see Uboot and then Kernel messages on the console

The default username/password is: ``root`` / ``olimex``
