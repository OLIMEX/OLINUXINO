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


------------------------------------------------------------------------------------------------------------------------------------------------------


Alternatively if you want to send commands to MOD-IO2 manually without compiling a program you can do this with i2c tools.
They allow you to check the address of the module (i2cdetect), to send (i2cset) and receive (i2cget) data. The format of the different tools can be seen when you type their name without parameters.
Note: In order to receive data (with i2cget) first you have to perform i2cset to trigger the data receiving. Otherwise it's possible to receive invalid result.

Below there is an example of how to do some of the basic functions provided by MOD-IO2 (read ID, set and read relay state, read digital and analog inputs). You can find the full list of supported commands and their format inside the "README.txt" of MOD-IO2 demo (https://www.olimex.com/Products/Modules/IO/MOD-IO2/resources/MOD-IO2_firmware_v_3_02.zip ). On this example we use MOD-IO2 connected to UEXT1 (I2C-2) with its default I2C address (0x21). The text after "//" is a simple comment of the performed action and the tabbed texts are the responses from the module.

The '-y' option is added so we don't have to confirm that we want to perform this action. Otherwise we will be prompted every time.


i2cdetect -y 2			// scanning for connected devices on I2C-2 bus


	     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
	00:          -- -- -- -- -- -- -- -- -- -- -- -- --
	10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	20: -- 21 -- -- -- -- -- -- -- -- -- -- -- -- -- --
	30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	70: -- -- -- -- -- -- -- --

i2cset -y 2 0x21 0x20		// send command (0x20) --> read ID

i2cget -y 2 0x21		// read result

	0x23

i2cset -y 2 0x21 0x40 0x03	// send command (0x40) --> relay 1 - ON; relay 2 - ON

i2cset -y 2 0x21 0x40 0x02	// send command (0x40) --> relay 1 - OFF; relay 2 - ON

i2cset -y 2 0x21 0x43		// send command (0x43) --> read relay status

i2cget -y 2 0x21		// read result

	0x02			// result depends on the state of the relays


i2cset -y 2 0x21 0x01 0x7f	// send command (0x01) --> all GPIOs input

i2cset -y 2 0x21 0x04 0x17	// send command (0x04) --> all Pull-ups enabled


i2cset -y 2 0x21 0x03		// send command (0x03) --> read GPIOs

i2cget -y 2 0x21		// read result

	0x1f			// result depends on the digital value of the GPIOs


i2cset -y 2 0x21 0x10		// send command (0x10) --> read analog 0 (GPIO 0)

i2cget -y 2 0x21		// read result

	0xbf			// result depends on the analog level of the GPIO 0



2016/09/02
Stanimir Petev, Olimex
