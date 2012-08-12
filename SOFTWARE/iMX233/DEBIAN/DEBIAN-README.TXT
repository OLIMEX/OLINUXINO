iMX233-OLinuXino Debian Squeeze rootfs

This is experimental Debian Squeeze rootfs.


You will need Open Embedded generated Kernel 2.6.35 which image you can download from: https://docs.google.com/open?id=0B-bAEPML8fwlT3g0MVBBTVpRQnM (image provided by JP Beale)

and the Debian rootfs you can download from: https://docs.google.com/open?id=0B-bAEPML8fwlMGVWQTJ6TXd1Qlk (image provided by JP Beale)


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
20. Copy the supplied files - OLinuXino_Debian_FS.tgz(the debian FS) and imx233-Olinuxino.zip(boot image) for example in your home/user directory, unzip imx233-Olinuxino.zip to imx233-Olinuxino.img
21. Enter the mounted sd card directory - cd /mnt/mmc 
22. Untar the rootfs to the card 'sudo tar -xzf /home/user/OLinuXino_Debian_FS.tgz' to put down the root filesystem. Note that in Debian FS you can use 'apt-get install .....' command 
	for login use 
	user:root
	password:RootPass
23. Copy the boot image in sdb1 device using dd, i.e. type - 'sudo dd if=/home/user/imx233-Olinuxino.img of=/dev/sdb1 
24. Type 'cd', 'sync' and umount sd card - 'sudo umount /dev/sdb2'

At this point the SD card should be ready for use. 
