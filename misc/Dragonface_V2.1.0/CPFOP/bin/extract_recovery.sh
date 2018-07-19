#!/bin/bash

rm -rf rootfs
rm -rf recoveryfs

boota -f $1 -x 
mv rootfs recoveryfs

if [ "$?" -eq "0" ]; then
	cd recoveryfs
	mkdir skel
	gzip.exe -dc ramdisk |(cd skel; cpio -iv)
	echo "done"
else
	echo $?
fi



