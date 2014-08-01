The tool is suitable for converting the binary configuration file used by the Debian distributions for Allwinner boards into text file. This allows easier and faster editing of a number of peripherals.

The script.bin file can usually be found in the main directory of a microSD card prepared with official Debian image. The folder containing the script can be inspected under both Windows and Linux.

This tool is compiled for Windows. It is tested under Windows 8.1 x64. For Linux convertors please check the sunxi tools here: https://github.com/linux-sunxi/sunxi-tools

Note that it is possible to change the script.bin in a running Debian image in newest releases! More information might be found at the end of the document! Not all releases feature the live method of changing fex to bin so if you board doesn't have it follow the other methods.

How to convert bin to fex or fex to bin:

1.Place your script.bin or script.fex file inside the folder where fexc.exe, FEX2BIN.BAT and BIN2FEX.BAT are located. It is important that the file you are going to convert to fex is named script.bin (if you are aiming for bin -> fex conversation else you would need to edit the BIN2FEX.BAT file). It is important that the file you are going to convert to bin is named script.fex (if you are aiming for bin -> fex conversation else you would need to edit the FEX2BIN.BAT file).

2. Run either BIN2FEX.BAT or FEX2BIN.BAT - which, respectively, convert BIN -> FEX and FEX -> BIN

3. The output file would show up in the same folder.

IMPORTANT! It might be wise to rename the file after conversation since new operation would overwrite previously converted script.fex and script.bin files.

The executable was not compiled by Olimex LTD.

For information about script.bin and script.fex please refer to sunxi documentation. This wiki article might be very helpful when editing the script.fex:

http://linux-sunxi.org/Fex_Guide 

Changing script.bin file without removing the microSD card:
========================
Note that you have to be very careful when changing the script.bin because the system can stop working. It is recommeded to make backup of script.bin file before change it.

The tools for script.bin changing are located in /opt/sunxi-tools directory
# cd /opt/sunxi-tools
# ./chscr.sh
This will convert script.bin file from sdcard to script.fex file and the file  will be opened using nano editor. 
Now you can change the board modules and parameters, save the changes and exit from nano editor
# ./wrscr.sh
this will convert script.fex to script.bin and the script.bin file will be written to sdcard.
