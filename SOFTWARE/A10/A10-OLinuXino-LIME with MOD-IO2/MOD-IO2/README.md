This is a tutorial how to build a simple project for A10-OLinuXino-LIME Rev.F with MOD-IO2 Rev.B.

In addition to the boards you also need SD card with Debian image (tested with "a10_Lime_debian_3.4.90_release_3.img"), A10-OLinuXino-LIME-UEXT, LAN and USB serial cable (like USB-SERIAL-CABLE-F).

1) Supply the board and connect all the hardware - USB serial cable to UART0 - TX0, RX0 and GND; LAN cable to the ethernet connector (in case you need to download the files from GitHub); A10-OLinuXino-LIME-UEXT to GPIO1 connector of the host board and MOD-IO2 to the UEXT of A10-OLinuXino-LIME-UEXT.

2) Open a console terminal program (like PuTTY, HyperTerminal etc.) with baudrate 115200

3) Wait until the board is initialized:

You must see this: "root@a10-OLinuXino:~#"

4) Make a directory where the project files will be stored (this step is optional and it's only for better arrangement of the files in the directory tree). You can do this with the command:
Input:

	mkdir MOD-IO2

5) Set the created directory as current directory:
Input:

	cd MOD-IO2

6) Since we are going to download the source and header files from GitHub we must enable the internet connection:
Input:

	dhclient eth0

	6.1) if no DHCP available you would have to select a specific IP address (for example 192.168.0.150)

	ifconfig eth0 192.168.0.150

7) Download the files from GitHub:
Input:

	wget <URL>
	
In order to get the URL select the file you want to download and click on the "Raw" button; then copy the URL and trigger the command (wget) in the console of A10-OLinuXino-LIME. Do this for the five files: i2c.cpp; i2c.h; IO2.cpp; IO2.h; main.cpp.

8) Just to make sure everything is fine and all five files are downloaded check the content of the directory:
Input:

	ls

The respond of the host board should be:
Output:

	i2c.cpp  i2c.h  IO2.cpp  IO2.h  main.cpp

9) Compile all source files:
Input:

	g++ *.cpp

A new file should appear. We check the content of the directory again:
Input:

	ls
	
Output:

	a.out  i2c.cpp  i2c.h  IO2.cpp  IO2.h  main.cpp

10) Execute the generated file
Input:

	./a.out


Relay 1 should be turned ON/OFF depending on the analog input on GPIO0 (AN0); Relay 2 should be turned ON/OFF depending on the digital input on GPIO1.

2016/08/31
Stanimir Petev, Olimex