This is a tutorial how to build a simple project for A20-OLinuXino-Micro Rev.F with MOD-LED8x8RGB Rev.B.

You will need:

- The boards
- SD card with Debian image (tested with ``A20_OLinuxino_Micro_debian_Jessie_34_103_2G_release_12.img``)
- Ethernet cable
- USB serial cable (like USB-SERIAL-CABLE-F)

1. Supply the board and connect all the hardware - USB serial cable to UART0 - TX0, RX0 and GND; and the MOD-LED8x8RGB to the SPI pins on either of the UEXTs.
(You must select in the code which of two SPI you use.
By default, SPI1 is used and the chip select is PortH.0).

2. Open a console terminal program (like PuTTY, HyperTerminal etc.) with baudrate 115200

3. Wait until the board is initialized and log in:

  A20-OLinuXino login: root
  Password: olimex

You must see this: ``root@a20-OLinuXino:~#``

4. Create a directory where the project files will be stored (this step is optional and it's only for better arrangement of the files in the directory tree):

  mkdir MOD-LED8x8RGB

5. Set the created directory as current directory:

  cd MOD-LED8x8RGB

6. Since we are going to download the source and header files from the git repository, an internet connection myst be established:

  dhclient eth0

6.1. If no DHCP server is available, you would have to select a specific IP address (for example 192.168.0.150)

  ifconfig eth0 192.168.0.150

7. Download the files from the git repository:

  wget <URL>

In order to get the URL, go to the git repository and locate the directory of the project.
Select the file you want to download and click on the "Raw" button.
Copy the URL and trigger the command (wget) in the console of A20-OLinuXino-Micro.
Do this for the six files: font.h; gpio_lib.cpp; gpio_lib.h; LED8x8RGB.cpp; LED8x8RGB.h; main.cpp.

8. Ensure that all six files are downloaded by checking the contents of the directory:

  ls

The response of the host board should be:

  LED8x8RGB.cpp  LED8x8RGB.h  font.h  gpio_lib.cpp  gpio_lib.h  main.cpp

9. Compile all source files:

  g++ *.cpp

A new file should appear.
We check the contents of the directory again:

  ls

Output:

  LED8x8RGB.cpp  LED8x8RGB.h  a.out  font.h  gpio_lib.cpp  gpio_lib.h  main.cpp

10. Execute the generated file:

  ./a.out

The LED matrix should slide a message and after that four rectangles with different sizes and colours will be displayed.
You can customize the demo by modifying the file LED8x8RGB.h.
There you can change the SPI module which is used and the number of MOD-LED8x8RGB boards in rows and columns.

2016/06/17

Stanimir Petev, Olimex
