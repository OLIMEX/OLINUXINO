echo off

set path=%CD%\bin;%windir%\system32

if not exist recoveryfs goto _no_target

bash pack_recovery.sh
goto _exit

:_no_target
echo No recoveryfs dir found

:_exit
::cmd /k
