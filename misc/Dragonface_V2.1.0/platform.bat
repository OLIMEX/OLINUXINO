::******************************************************************************
::*                           support MID\Homelete Platform tool
::*
::* Purpose     : 
::*
::* By            : Sam Liu   
::* Date         :2013_01_07


::******************************************************************************
@echo off
echo ************************************
echo * support MID\Homelete Platform tool *
echo ************************************

:SELECT
echo Please select a product platform to use this tool 
echo ************************************
echo 1: Homelet\Dangle
echo 2: MID
echo ************************************
set /p SEL=Please Select:

if %SEL%==1     goto HOMELET

if %SEL%==2     goto MIDP


@echo Selecti is invalid!!!

goto  SELECT

:HOMELET
copy .\update23_homelet.dll .\update23.dll /y
goto EXIT

:MIDP
copy .\update23_MID.dll .\update23.dll /y

:EXIT
pause
