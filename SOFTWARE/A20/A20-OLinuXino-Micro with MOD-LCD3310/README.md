This is a tutorial how to build a simple project for A20-OLinuXino-Micro Rev.F with demo for MOD-LCD3310 Rev.A.

You will need:

- The boards
- SD card with Debian image (tested with ``A20_debian_kernel_3_4_LAN_USBx2_Cards_LCD_HDMI_SATA_TS_X_GPIO_OTG_MIC_Video_accel_release7.img``)
- Ethernet cable
- USB serial cable (like USB-SERIAL-CABLE-F)

1. Connect all the hardware and supply the board - USB serial cable to UART0 - TX0, RX0 and GND; and the MOD-LCD3310 to UEXT1.

2. Open a console terminal program (like PuTTY, HyperTerminal etc.) with baudrate 115200

3. Wait until the board is initialized (you must see this: ``root@a20-OLinuXino:~#``)

4. Create a directory where the project files will be stored (this step is optional and it's only for better arrangement of the files in the directory tree):

  mkdir MOD-LCD3310

5. Set the created directory as current directory:

  cd MOD-LCD3310

6. Since we are going to download the source and header files from GitHUB we must enable the internet connection:

  ifup eth0

7. Download the files from GitHUB:

  wget <URL>

In order to get the URL, go to the git repository and locate the [directory of the project](https://github.com/OLIMEX/OLINUXINO/tree/master/SOFTWARE/A20/A20-OLinuXino-Micro%20with%20MOD-LCD3310).
Select the file you want to download and click on the "Raw" button.
Copy the URL and trigger the command (wget) in the console of A20-OLinuXino-MICRO.
Do this for the five files: gpio_lib.c; gpio_lib.h; LCD3310.c; LCD3310.h; main.c.

8. Ensure that all five files are downloaded check the content of the directory:

  ls

The response of the host board should be:

  gpio_lib.c  gpio_lib.h  LCD3310.c  LCD3310.h  main.c

9. Compile all source files:

  gcc *.c

A new file should appear.
We check the content of the directory again:

  ls

Output:

  a.out  gpio_lib.c  gpio_lib.h  LCD3310.c  LCD3310.h  main.c

10. Execute the generated file:

  ./a.out

This message should be shown in the display:

  "*** OLIMEX ***"
  "  OLinuXINO   "
  "  Micro and   "
  "  MOD-LCD3310 "
  "  using SPI   "
  "  interface   "

...where the rows 0, 2, 4 are with black background and white symbols, and rows 1, 3, 5 are with white background and black symbols.

2014/07/23

Stanimir Petev, Olimex
