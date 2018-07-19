#!/bin/bash

if [ -d bootfs/skel ]; then
	(cd bootfs/skel; find .|cpio -o -Hnewc |gzip.exe > ../ramdisk)
	rm -rf rootfs
	mv bootfs rootfs
	boota -c
	mv rootfs bootfs
	mv new.fex bootfsnew.fex
	
else
	echo "No bootfs/skel dir"
fi



