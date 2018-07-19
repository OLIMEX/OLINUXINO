# A20-OLinuXino-LIME2

OPEN SOURCE HARDWARE EMBEDDED ARM LINUX SINGLE BOARD COMPUTER WITH ALLWINNER A20 DUAL CORE CORTEX-A7 1GB RAM AND GIGABIT ETHERNET

Upgraded and improved version of A20-OLinuXino-LIME with double the RAM and Gigabit Ethernet

A20-OLinuxino-LIME2 can be produced with NAND or eMMC Flash memory.
The prefix 'e' or 'n' define the type of Flash memory on the board.

![A20-OLinuXino-LIME2 Top](doc/images/A20-OLinuXino-LIME2/A20-OLinuXino-LIME2-1.jpg "A20-OLinuXino-LIME2 Top View")

# Features

- Allwinner A20 dual core Cortex-A7 processor, each core typically running at 1GHz and dual-core Mali 400 GPU
- 1GB DDR3 RAM memory
- 8GB NAND flash memory option (in A20-OLinuXino-LIME2-n8GB)
- 4GB eMMC flash memory option (in A20-OLinuXino-LIME2-e4GB)
- Native SATA support with connector and 5V SATA power jack
- Capable of FullHD (1080p) video playback 
- Native HDMI support with connector
- 2 x USB High-speed host with power control and current limiter
- USB-OTG with power control and current limiter
- 1000MBit native Ethernet
- LiPo Battery connector with battery-charging capabilities
- LCD row of pins (0.05'' step)
- 160 GPIOs on four GPIO rows of pins (0.05'' step)
- MicroSD card connector
- DEBUG-UART connector for console debug with USB-SERIAL-CABLE-F
- GPIO LED
- Battery charge status LED
- Power LED
- 2KB EEPROM for MAC address storage and more
- 3 BUTTONS with ANDROID functionality + RESET button
- 4 mount holes
- 5V DC input power supply jack, noise immune design
- PCB dimensions: (84 x 60)mm ~ (3.3 x 2.4)"

# Documents

- [User's manual](doc/manuals/A20-OLinuXino-LIME2-UM.pdf)
- [Board's dimensions](doc/images/dimensions_A20_lime2.png)

# Hardware

- [Schematics and hardware source file](hardware)
- [Eagle shield template](hardware/Rev.AA/shield_template_eagle)
- [KiCad shield template](hardware/Rev.AA/shield_template_kicad)

# Software

Image links, instructions, demo software, customer projects and more might be found at the [wiki page of the LIME2 board](https://www.olimex.com/wiki/A20-OLinuXino-LIME2).

# Notice

[Evaluation Board/Kit Important Notice](https://www.olimex.com/Products/Disclamer/evaluation-board-notice)

# FAQ

## I am an inexperienced user and I'm afraid to mess up initially. What are the most common errors in the initial start-up of the board?

The best idea is to download the latest Debian or Android image from the wiki and to pay attention if it is for microSD card or the NAND memory.
After that ensure the image is properly written to the SD card or the NAND memory.
Finally, if you are experiencing video output problems, ensure the image is set with the proper settings for the display/monitor you are going to use.
Double check if the power supply is sufficient (especially if powering a display from the board).

## Can I use the Debian or Android image for A20-OLinuXino-LIME or A10-OLinuXino-LIME instead of the one for A20-OLinuXino-LIME2?

No.
Both Debian and Android images are specfic for each board.
Make sure the image that you download and use is specifically marked as suitable for A20-OLinuXino-LIME2.
The prepared cards which we sell for the board are also different.

## I made a shield for my LIME board. I now own a LIME2 board. Can I use the shield with the LIME2 design?

No.
The LIME2 design swaps the signals of each row of each 0.5" step header.
All four GPIO headers and the LCD header are affected.
Shields made for LIME boards can't be connected directly to a LIME2 board.

## What do I need to connect one of the Olimex-made displays to the board?

There are two scenarios depending on the display you have at your disposal.

Newer displays have three 40-pin connectors.
This makes connections possible via a single 0.05" or 0.1" step cable (step of the cable depends on the step of the LCD connector of the board).

Older displays have a single 0.1" step connector and you would need an adapter to connect to boards with 0.05" connector (such as LIME2).
The the adapter is called A20-OLinuXino-LIME2-UEXT you would also need both 0.05" step cable and 0.1" step cable.
Note that A10-OLinuXino-UEXT adapter is not suitable for A20-OLinuXino-LIME2.
The layout of the LCD connector of A10-OLinuXino-LIME and A20-OLinuXino-LIME2 is different.

## I bought a supported LCD and the required cables but when I connect it to the A20-OLinuXino-LIME2 I receive no image on the display. What do I do wrong?

The default A20-OLinuXino-LIME2 Debian image is set for HDMI output and HD resolution.
To use LCD or lower the resolution you would need to start the shell script in the default Debian image.

## What is the operating temperature range of A20-OLinuXino-LIME2?

The board works in the commercial temperature range 0C to +70C.

## How long this board will be available?

This board will be available [forever](https://olimex.wordpress.com/2014/11/27/how-long-olinuxino-with-allwinner-socs-will-be-produced-again-now-we-know-the-answer-forever)!