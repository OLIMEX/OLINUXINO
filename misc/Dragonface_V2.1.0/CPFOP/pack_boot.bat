echo off

set path=%CD%\bin;%windir%\system32

if not exist bootfs/ goto _no_target

bash pack_boot.sh
goto _exit

:_no_target
echo No rootfs dir found

:_exit
echo 'yes'
::cmd /k
