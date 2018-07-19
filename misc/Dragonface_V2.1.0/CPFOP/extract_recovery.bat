echo off

set path=%CD%\bin;%windir%\system32

if not exist recovery.fex goto _no_target

bash extract_recovery.sh recovery.fex
goto _exit

:_no_target
echo No recovery.fex found
echo %CD%

:_exit
::cmd /k
