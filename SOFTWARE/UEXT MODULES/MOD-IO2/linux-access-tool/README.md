Linux tool for easier use of MOD-IO2
==============

How to use?
--------------

This software tool requires a Linux-enabled board and OLIMEX MOD-IO2. Your MOD-IO2 board has to use firmware revision 3.x - for full compatibility the firmware should be 3.02 or newer. 

You would also need to establish proper I2C connection between the board and MOD-IO2 (if using an OLIMEX Linux-enabled board this is established automatically when using the UEXT). 

To use the tool download the binary file "modio2tool" to your board. Navigate to the folder where you placed it and type "./modio2tool -h" to get help on the available commands.

How to compile?
--------------
If you wish to modify the sources or compile the binary yourself - download the all raw files from folder linux-access-tool. Place them in a freshly created folder in your Linux-enabled board (OLinuXino; SOM; etc). You need to have properly installed GCC compiler.

Navigate to the folder and compile the project by typing "make".
