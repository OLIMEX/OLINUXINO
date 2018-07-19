#!/bin/bash

if [ -d recoveryfs/skel ]; then
	(cd recoveryfs/skel; find .|cpio -o -Hnewc |gzip.exe > ../ramdisk)
	rm -rf rootfs
	mv recoveryfs rootfs
	boota -c
	mv rootfs recoveryfs
	mv new.fex recoverynew.fex
else
	echo "No recoveryfs/skel dir"
fi



