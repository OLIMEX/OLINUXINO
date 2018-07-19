copy bootfs\skel\initlogo.rle recoveryfs\skel\initlogo.rle
copy bootfs\skel\ueventd.sun4i.rc recoveryfs\skel\ueventd.sun4i.rc
copy bootfs\skel\ueventd.rc recoveryfs\skel\ueventd.rc
copy bootfs\skel\nand.ko recoveryfs\skel\nand.ko

call pack_boot.bat
call pack_recovery.bat
