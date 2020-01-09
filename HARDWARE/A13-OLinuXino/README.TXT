This repository contains:

Original hardware design files for A13-OLinuXino and A13-OLinuXino-WIFI. These are made with Eagle 4.16r2. PDF exports are available also.

Both boards use same design files, but the A13-OLinuXino-WIFI comes with NAND memory and on-board MOD-WIFI-RTL8188ETV module.

Notice that it is possible that simultaneously purchased A13-OLinuXino and A13-OLinuXino-WIFI to be from different revisions.


Latest hardware revision: revision H

Major changes in revision H compared to previous revision G2:

- Q1 value changed from HC49/S1 to Q24MHz/20pF/20ppm/2P/HC-49SM(SMD);
- Q2 changed from HC49/S1 to Q12MHz/20pF/20ppm/2P/HC-49SM(SMD);
- Q3 value and package changed from 32768Hz/6pF to Q32.768kHz/12.5pF/20ppm/4P/SMD8038;
- RM16 renamed to RM11;
- U10 and U15 changed from SY8008C(AA)C to SY8009AAAC(SOT23-5);
- U16 changed from SY7208(SOT23-6) to MT3608(SOT23-6);
- PWR-JACK value changed from YDJ-1136 to PWRJ-2mm(YDJ-1134);
- Optimized R84 from 100k/1% to 4.99k/1% and R86 from 22k/1% to 1.1k/1% ;
- Optimized C134 from 47pF to 33pF ;
- Optimized C114 from 22pF to 33pF ;
- Optimized L12 from FB0805/600R/200mA(201209-601) to FB0805/600R/2A ;
- Optimized L1 from CL470nH/0805/1.76R/250mA to FB0805/600R/2A;
- Optimized R43, R98, and R99 from 51R to 22R;
- GPIO-2 and LCD_CON were changed from ML40 to BH40S.

Changes in revision G2 compared to previous revision G1:

- Changed C144 to 100pF

Changes in revision G1 compared to previous revision G:

- Added capacitor C144 1nF on DDR3-RST signal

Changes in revision F compared to previous revision E1:

- Changed the SD card package so that it is easier for repair and attachment
- Changed the package of the 32768 crystal to an SMT one (with 4 pins)

Board features:

- A13 Cortex A8 processor at 1GHz, 3D Mali400 GPU
- 512 MB RAM (2 x 256Mbit x 8)
- 4GB NAND Flash (for A13-OLinuXino-WIFI)
- 6-16VDC input power supply, noise immune design
- 3 + 1 USB hosts, 3 available for users, 1 leads to onboard pinout or to WIFI module (for A13-OLinuXino-WIFI)
- 1 USB OTG which can power the board
- SD-card connector for booting optional Linux images
- VGA video output
- Battery option and connector
- LCD signals available on connector so you still can use LCD if you disable VGA/HDMI
- Audio output
- Microphone input
- 4 Mount holes
- RTC PCF8563 on board for real time clock and alarms
- 5 Keys on board for android navigation
- UEXT connector for attaching expansion boards (sensors, relays, etc)
- GPIO connector with 68/74 pins and these signals:
- 17 for adding NAND flash;
- 22 for connecting LCDs;
- 20+4 including 8 GPIOs which can be input, output, interrupt sources;
- 3x I2C;
- 2x UARTs;
- SDIO2 for connecting SDcards and modules;
- 5 system pins: +5V, +3.3V, GND, RESET, NMI
- Dimensions: (120x120)mm ~ (4.7x4.7)''
