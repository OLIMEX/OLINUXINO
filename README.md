# OLinuXino: Single-Board Linux Computer, Copyright (C) 2012-2018, OLIMEX Ltd

## Overview

OLinuXino is an Open Source Software and Open Source Hardware project, aiming to produce low cost (starting from EUR 24) Linux and Android Single Board Computers (SBC) in both commercial and industrial temperature range.
Board variants with extra flash memory are also available.

People keep comparing OLinuXino with Raspberry Pi and BeagleBone project, so we would like to state some of the differences.
Although the projects are similar, they have different goals and implementations:

Unlike the Raspberry Pi, OLinuXino models are completely open source, including hardware and software.
This means you have access to all CAD files and sources and you can reuse them for your own personal or commercial project.
There are **NO** restrictions to manufacture and sell these board for your own use or resale.

iMX233-OLINUXINO uses a widely-available microcontroller (iMX233) which only costs USD$5.50 for 100 pcs.
This means that people can spin off their own boards and manufacture them inexpensively as the processor is in TQFP easy to assembly by hobbyist package.

Raspberry Pi have not released any CAD files nor complete schematics.
The Raspberry Pi uses a Broadcom processor which is not available for sale in small quantities.
It also uses a BGA package which require expensive setup to assemble.
RPi is designed to be home or educational gadget whereas OLinuXino variants may operate within industrial-grade thermal environments (-25C to +85C) and are designed to be low-cost and resilient to noise.

BeagleBone releases open-source CAD hardware files but leverage a BGA processor.
BeagleBone boards are also very complex and difficult to manufacture in small quantities when costs are concerned.

iMX233-OLINUXINO uses processor on 454Mhz and have less memory and will not allow fancy graphics, but this is not our intention.

## Potential applications of OLinuXino

- 3D rep-rap printer controller including G-code interpreter - now all 3D printers use PC/Laptop connected to Arduino stepper driver.
This board will handle both without issue
- Low cost PLC running open source PLC porgramming languages
- Home Automation - connecting GSM module or Zigbee sensors would be easy with the existing UEXT connector
- OLinuXino has a 100Mbit Ethernet port, but there are many low cost $10 WIFI USB modules with Linux drivers like RTL8192 which allow an OLinuXino to wirelessly connect to the internet.
This enables the OLinuXino to control relays and sensors without the need for physical network connections.
- Having small linux module with GPIOs would be handy even to embed it in other products.
BeagleBone boards are not allowed to be used in commercial projects, whereas there are no restrictions with OLINUXINO.

## Development Path

### Step 1

OLinuXino project started with iMX233 from Freescale for several reasons: this is ARM9 processor running at 454Mhz with enough power to run linux and still in handsolder friendly TQFP package, which allow hobby DIY approach.
iMX233-OLinuXino-MICRO is even on 2 layer PCB and running at full speed.
The maximum memory of 64MB though limited the applications with it, so we were looking around for something more powerful when A13 from Allwinner came along.

### Step 2

A13 is Cortex-A8 processor which can address up to 512MB RAM and operate at 1GHz.
It comes in the hand-solderable TQFP package (actually it's the first and the only Cortex-A8 in TQFP package).
So designing A13-OLinuXino was the next logical step.
The first prototypes are already produced and everything works as expected. A13-OLinuXino production is scheduled for September.
A13-OLinuXino have 512MB RAM, 4 USB hosts (1 dedicated for WIFI), 1 USB-OTG, Audio out, Audio in, SD-card, VGA, buttons, 72 GPIOs, LCD connector

### Step 3

A10 is the big brother of A13 it's also Cortex-A8 running at 1Ghz (many Chinese tablet/set-top-box vendors write it's 1.2 or 1.5GHz but this is overclocking and same is possible with A13 too, but not recommended for normal operation).
It can address up to 2GB of RAM, and on top of A13 have SATA, HDMI, VGA and composite video outputs) + much more GPIOs as it's in BGA442 package.
We received many requests for a hackable A10 board while developing the A13-OLinuXino, as all current solutions are tablets or set-top-boxes which are not designed to have GPIO connectors and to allow hardware hacking.
The A10-OLinuXino will have same as A13 but including more GPIOs, 1GB RAM, HDMI, SATA and 100MBit Ethernet.

### Step 4

A10S is new processor from Allwinner featuring Ethernet and HDMI.
It is a low-cost board with A10S Cortex-A8 @ 1GHz + 512MB RAM + Ethernet 100Mbit + USB Host/USB-OTG + SD-card + SD-MMC card + HDMI is released.

### Step 5

A20 is Dual Core Cortex-A7 processor which is almost pin to pin compatible, so A10 board can work with A20 processors too.
We have low cost A20 board prototypes working and A20-SOM work in progress.

### Step 6

The Texas Instruments Sitara AM3352 processor is necessary for Industrial customers who desire product supply longevity.
TI as Freescale guarantee that when they begin producing a processor, production will last at least 5-10 years, alleviating customers from having to change their designs every year.
So, we work on AM3352 modules with:

- AM3352 720Mhz, Cortex-A8 processor
- 512MB DDR3 RAM (we will optionally make a 1GB RAM version)
- power supply 6-16VDC
- 4x USB 2.0 hosts
- 100Mbit Ethernet
- MicroSD card
- VGA
- CAN
- GPIO
- LCD connector with touchscreen to work with A13-LCD7TS and A13-43TS LCDs
- UEXT connector
- JTAG

### Step 7

Allwinner A64 chip into A64-OLinuXino development board.

### Step 8

DYI laptop TERES-I with A64 chip.

### Step 9

Expanded the A20 portfoloio with more convenient A20-SOM204 board.

### Step 10

...we don't know yet, as there are numerous candidates.

## Web resources

- [OLIMEX Ltd web site where the OLinuXino board info is hosted](http://www.olimex.com)
- [Hardware and software sources hosted on GitHub](https://github.com/OLIMEX/OLINUXINO)
- [Olimex forum for OLinuXino project discussions](https://www.olimex.com/forum/index.php)
- [Web IRC chat](http://webchat.freenode.net/?channels=olimex)
- OLinuXino IRC channel: join #olimex on irc.freenode.net

## License

### Hardware

The Hardware project is released under the Creative Commons Attribution-Share Alike 3.0 United States License.
You may reproduce it for both your own personal use, and for commercial use. 
You will have to provide a link to the original creator of the project http://www.olimex.com on any documentation or website.
You may also modify the files, but you must then release them as well under the same terms.
Credit can be attributed through a link to the creator website: http://www.olimex.com

### Software

The software is released under GPL.
