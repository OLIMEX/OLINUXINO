#!/bin/bash

rm -rf rootfs
rm -rf bootfs

boota -f $1 -x
mv rootfs bootfs

if [ "$?" -eq "0" ]; then
	cd bootfs
	mkdir skel
	gzip.exe -dc ramdisk |(cd skel; cpio -iv)
	echo "done"
else
	echo $?
fi



