iMX233-OLinuXino ARCHLINUX distribution

See https://github.com/OLIMEX/archlinuxarm-olinuxino for the latest installation instructions. Files referenced below are obsolete.


Kiril Zyapkov is one of our early developers who received free iMX233-OLinuXino-Maxi, he uses ARCHLinux during his daily work and he made ArchLinux distribution for OLinuXino.
The distribution use OpenEmbedded Layer to generate the necessary packages. The project is at GitHub: https://github.com/OLIMEX/archlinuxarm-olinuxino

If you browse the Github project you will notice PKGBUILD scripts, they are used by makepkg (which is part of pacman, the package manager for arch) to build the packages.
Kiril still work to create custom repo with the kernel, elftosb and imx-bootlets, now he is working on PKGBUILD for imx-bootlets, to may the install script for the kernelto add alone mmcblk0p1, and will be ready in the next days.

For these who want to test Archlinux without building the packages themself there is ready generated binary rootfs at https://docs.google.com/open?id=0B-bAEPML8fwlUnB6MTU4NVdqb1k
it works with the normal Open Embedded generated Kernel 2.6.35 which image you can download from: https://docs.google.com/open?id=0B-bAEPML8fwlWDRpMGl5TGVaNXM

Here are some documentation and useful links:

https://wiki.archlinux.org/index.php/Pacman
https://wiki.archlinux.org/index.php/Pacman_Tips
https://wiki.archlinux.org/index.php/Post_Installation_Tips

and here https://aur.archlinux.org/ is collection of many software which are not part of the repositories.
You can figure out how build system works by browsing their PKGBUILD files.
ArchlinuxARM uses the buildscripts of the original Arch, but with modifications of some packages, the repository is https://github.com/archlinuxarm/PKGBUILDs . There is scrip for rootfs generation https://github.com/archlinuxarm/PKGBUILDs/blob/master/scripts/distro-builder

Kiril is using the same script to generate rootfs-a for OLinuXino.


Short description about how to make bootable SD card with Linux file system for OLinuXino boards

(NOTE - this example is given with "sdb1 and sdb2" device but it could enumerate differently on your host, so confirm what it enumerates as before running fdisk!) 

1. Insert the card reader to the Linux host.
2. Un-mount the card reader, i.e. 'sudo umount /dev/sdb1'
3. run fdisk, 'sudo fdisk /dev/sdb'
4. Press 'p' to show the partitions on the card
5. Press 'd' to delete a partition. Repeat to remove all partitions
6. Press 'n' to create a new partition
7. Press 'p' to select the primary partition
8. Press '1' to create partition 1 on the card
9. Press Enter to start from first block
10. Type '16MB' to create the 16MB partitions
11. Press 't' to change the newly created partition type
12. Enter '53' for the new partition type
13. Press 'n' to create a second partition, select 'p'(primary), then '2'(number)
14. Press Enter to accept all default settings
15. Press 'w' to write the partitions to the card and exit the fdisk
16. Format the second partition on the SD card - run 'sudo mkfs.ext2 /dev/sdb2' command.
17. Mount the second partition on the card
18. If you do not have the mmc mounting point created yet, create one with 'sudo mkdir /mnt/mmc'
19. Run 'sudo mount /dev/sdb2 /mnt/mmc' to mount the partition as the mmc card
20. Copy the supplied files - alarm-olinuxino-rootfs.tar.gz(the ARCHLINUX FS) and imx233-Olinuxino.zip(boot image) for example in your home/user directory, unzip imx233-Olinuxino.zip to imx233-Olinuxino.img
21. Enter the mounted sd card directory - cd /mnt/mmc 
22. Untar the rootfs to the card 'sudo tar -xzf /home/user/alarm-olinuxino-rootfs.tar.gz' to put down the root filesystem.
	for login use 
	user:
	password:
23. Copy the boot image in sdb1 device using dd, i.e. type - 'sudo dd if=/home/user/imx233-Olinuxino.img of=/dev/sdb1 
24. Type 'cd', 'sync' and umount sd card - 'sudo umount /dev/sdb2'

At this point the SD card should be ready for use. 
