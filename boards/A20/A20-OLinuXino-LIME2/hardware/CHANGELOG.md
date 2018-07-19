# Notable hardware revision changes of A20-OLinuXino-LIME2

## Hardware revision B (pre-production)

Internal initial release.

## Hardware revision C (initial public release)

1. C175 changed from 100n na 22uF (and changed package from 0402 to 0603)
2. Updated white prints: Lime to Lime2; rev.B to rev.C, 2014 to 2015

## Hardware revision D (internal, not released)

1. eMMC memory added; NAND or eMMC optional. Schematics set for NAND per default.
2. C224 and C232 - 47uF/6.3V/0805 added in parallel to the 5V_SATA_PWR connector;
3. Changed L1, L2, L3 from FB/600R/0805 to 0R/0805;
4. Capacitors C84, C85 changed to 22uF/0603 down from 100nF/0402;
5. Fixed the white name print from A20-OLinuXino-Lime(bottom) to A20-OLinuXino-Lime2 (there was no "2");
6. Improved LAN-a routing
7. Added four extra data bits to the eMMC, through a resistor matrix
8. Changed EMMC_RST. Moved from PC4/NCE0/EMMC_RST to PC16/NWP/EMMC_RST

## Hardware revision E (internal, not released)

1. Fixed white print of EMMC_PWR jumper (added positions).
2. Changed the white print of board revision.
3. U15 and U17 were changed in value from SY8008C(AA)C to SY8009AAAC(SOT23-5).

## Hardware revision F (internal, not released)

1. Change Ethernet PHY controller to RTL8211E-VB-CG1. This required changing a lot of components and routing in the area around the controller too.
2. Changed the placing of the pull-up rezistorite of the RX wire (before the matrix).

## Hardware revision G (public release)

1. Added 4 improved mounting through holes, connected to GND. Removed the old milled mounting points.

## Hardware revision G1 (public release)

1. Changed a couple of components that improves board's power up sequence due to the high initial consumption of the Ethernet controller (RTL8211E). R109 changed to 47k/0402 (down from 470k/0402) and R107 (470k/0402) replaced by a capacitor 220nF/0402.

## Hardware revision G2 (public release)

1. The buttons were moved 5 mils further to the middle of the board to futher reduce deformation during panelization.
2. R32 and R36 were moved 40 mils further away from the Ethernet connector.
3. C207 was moved to the top side of the board
4. R76 was moved away from C200. Now it is placed in parallel to R83.
5. C179 was moved a little bit away while C163 was rotated.
8. Fixed white print, symbol and package of R107(220nF) to avoid confusion.
9. R105 was changed to NA; instead a pull-down R107 (4.7k) was added.
10. C150 was moved 15mils away from the Ethernet LEDs.
11. C212 and C213 were moved 40mils away from L16.
12. The white print year changed to 2017 and the revision print updated to G2.
13. C201 and C211 were changed from 22pF/0603 to 27pF/0603
14. C116 and C117 were changed from 33pF/0603 to 27pF/0603

There are three variants manufactured:

1. A20-OLinuXino-Lime2-eMMC_Rev_G2: eMMC present, the NAND Flash is NA, all components in commercial temperature range
2. A20-OLinuXino-Lime2-4GB_Rev_G2: NAND flash present, the eMMC is NA, all chips are in commercial temperature range
3. A20-OLinuXino-Lime2_Rev_G2: Both NAND flash and eMMC are NA, all chips are in commercial temperature range




