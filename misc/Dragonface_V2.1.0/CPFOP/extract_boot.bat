echo off

set path=%CD%\bin;%windir%\system32

if not exist boot.fex goto _no_target

bash extract_boot.sh boot.fex
goto _exit

:_no_target
echo No boot.fex found
echo %CD%

:_exit
::cmd /k

