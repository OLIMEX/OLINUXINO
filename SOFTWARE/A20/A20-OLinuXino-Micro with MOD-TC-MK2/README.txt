Demo code for MOD-TC-MK2 and A20-OLinuXino-MICRO

This demonstrates functionality of MOD-TC-MK2 hardware revision B connected to A20-OLinuXino-Micro rev.K with A20-OLinuXino-bullseye-base-20211223-102355 image on an SD card.

Important! This demo is not compatible with MOD-TC-MK2 hardware revision A.

To make it work you will need some sort of communication between your computer and the OLinuXino (for example USB-Serial). Connect the RX, TX and GND (from USB-Serial) to the pins of UART0 (on the OLinuXino): connect GND to GND, RX to TX and TX to RX. Open a terminal program (like PuTTY) on the port of the USB-Serial (or whatever tool you are using) with 115200 baudrate, 8 data bits, 1 stop bit, no parity.

Also you need to connect MOD-TC-MK2 to UEXT1 of the A20-OLinuXino-Micro via 10 pin UEXT cable. Power the board (7-24 volts). Note: if you want to connect it to UEXT2 you should change the value of the USED_I2C_BUS macro in the code (line 23) from 2 to 1. You also need to connect a thermocouple to the MOD-TC-MK2. Then:

1) Wait for the Linux to boot and log (by default user: root; password: olimex)

2) Create a new folder (optional). To do so type in the console:

mkdir MOD-TC-MK2

And switch to it as a current directory:

cd MOD-TC-MK2

3) Get the source file to the OLinuXuno. You have 2 options to do so:

3a) download the source from github if you have access to the internet

wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/A20/A20-OLinuXino-Micro%20with%20MOD-TC-MK2/MOD-TC-MK2.c
	
3b) or create a new file:
	
nano main.c

And then copy the content from your computer and paste it into the terminal. Close and save the file. (CTRL+X --> Y --> Enter)

4) Compile it:
	
gcc *.c
	
5) You can check the list of files in the current directory (optional)

ls

The output should be:

a.out main.c
	
6) Run the executable file:

./a.out

If everything is connected properly on the terminal should be printed the values of the inner and outer temperature measured by the MOD-TC-MK2 module

Inner temperature: 23.1250 C
Outer temperature: 23.25 C
	
In case of some sort of error you will see one of the following errors:

Fault: Open Circuit;

To stop it press CTRL+C

2022/02/24
Stanimir Petev, Olimex