# Hardware changes in revision G (released)

1. Added C223 - 220uF/tant with NA value on SATA PWR.
2. C216 is now 22uF/6.3V instead of 100n, so it gets charged slower.  
3. Update of all libraries. HDMI connector is changed to new package.
4. HDMI protection removed (caused incompatiblities with cheap HDMI monitors).

# Hardware changes in revision H (only prototypes; not released)

1. Changed the LAN part of the design, added two capactiors 1nF/2kV/X7R/1206, and 1nF/2kV/X7R/1206_or_1M, 
also the matrix is now 0402 with 49.9 ohm at 1%. 
2. Major polygon changes.
3. Removed HDMI protection ciruit(RCLAMP0524P becomes NA).
4. Preparation for memory changes from MEM4G16D3EABG-125 to K4B4G1646D-BCK0.
5. Position of C95 and C96 slightly adjusted, so that they are not under GPIO-1 and LAN. USB_OTG polygon changed. USB_HOST polygon adjusted. Removed few vias. 
6. Added few more vias for the VCC.
7. Improved white print. Year and revision updated. Web-site address updated.
8. Power jack package updated.
9. SATA package updated.

# Hardware changes in revision I (only prototypes; not released)

1. eMMC was added.
2. RTL8201CP(LQFP-48) Ethernet chip replaced with LAN8710AI-EZK(QFN-32). Both are MbE PHYs.
3. All libraries were updated.
4. L7, L15, L16 and L17 w�re changed from FB0805/600R/200mA(201209-601) to FB0805/600R/2A.
5. PWR-JAKPWR_JACK_UNI_MILLING_SMALLER_DRILLS changed to PWRJ-2mm(YDJ-1136).
6. R98 optimized from 10k/1% to 10k.
7. C195 and C208 optimized 22pF to 33pF.
8. R43 and R103 optimized from 51R to 100R.
9. C123, C126 and C135 were changed from 100uF/16V/LOWESR/105C/6.3x11mm_RM2.5 to 2x47uF/6.3V/0805, i.e. C123||C240, C126||C222 and C135||C223
10. D6 was renamed to D2 and changed to the D4's package.
11. U13 and U15 were changed from SY8008 to SY8009AAAC(SOT23-5).
12. ODT1, SCKE1, SCS1/SA15 and ZQ1 were connected to the 2 DDR memories with aim to support bigger sizes if one day A40 is released or A10 is used.
13. All resistor matrices: RM1-RM8 and RM16-RM21(RA0805_(4X0402)_0R(Board_Mounted)) were removed and the wires were shortened. The same thing was done for: R106-R117(0R(Board_Mounted)).
14. The viases that were between GPIO-1 pads 12/14 were removed.
15. The eMMC was placed over the NAND flash.
16. The cream layer was improved.
17. BKUP connector(HN1x2) was added for AXP's power management option.
18. RM1 and RM2 were added to enable CTP support for the LCD connector.
19. MP1482DS was exchanged with MP1584EN(SOIC8E). Because of this, were affected D5, C202, C203 and several other components as well.
20. The files were renamded from 4G to nothing, because the schematic is universal for all boms generated!
21. C217's value was changed from 220uF/10V/TANT to 220uF/10V/TANT_D!
22. Added reset control for the new PHY - C63 becomes NA down from 10uF, R26 becomes NA down from 10K and R25 is now 100R up from NA.

# Hardware changes in revision J (released)

1. Rotated RM1 and RM2 at 90 degrees for better soldering.
2. Created separate design for each of the variants of the board ( 1. commercial temeprature range and no flash memory - A20-OLinuXino-MICRO; 2. industrial temperature range and no flash memory - A20-OLinuXino-MICRO-IND; 3. commercial temperature range and flash memory - A20-OLinuXino-MICRO-EMMC; 4. industrial temperature range and flash memory - A20-OLinuXino-MICRO-EMMC-IND) - each of these boards would have own hardware revision from now on.

## A10-A20-OLINUXINO-MICRO

1. DDR3 memory is set to commercial Micron variant - MT41K256M16HA-125:E(DDR3L-1600_11-11-11);
2. The PHY was changed to commercial variant - LAN8710A-EZC-TR(QFN-32);
3. Changed two resistor values: r77 from 4.99k/1% to 12.4k/1% and r78 from 3.32k/1% to 10k/1% in order to decrease commercial DDR3L power supply from 1.5V to 1.35V; The text: "When DDR3L is used, VDD&VDDQ are set to 1.35V!!!" was added in the schematic!
4. The EMMC and all related components were changed to NA;
5. The NAND flash and all related components were changed to NA;

## A10-A20-OLINUXINO-MICRO-IND

1. Changed the memory to the industrial variant - MT41K256M16HA-125IT:E(DDR3L-1600_11-11-11)
2. Changed PHY to industrial variant LAN8710AI-EZK-TR(QFN-32)
3. Changed r77 from 4.99k/1% to 12.4k/1% and r78 from 3.32k/1% to 10k/1% in order to decrease commercial DDR3L power suplly from 1.5V to 1.35V; the text: "When DDR3L is used, VDD&VDDQ are set to 1.35V!!!" was added in the schematic!
4. The EMMC and all corresponding components were changed to NA;
5. The NAND Flash and all corresponding components were changed to NA;

## A20-OLINUXINO-MICRO-EMMC

1. DDR3 memory is set to commercial Micron variant(required by Ceco) - MT41K256M16HA-125:E(DDR3L-1600_11-11-11);
2. The PHY was changed to commercial variant - LAN8710A-EZC-TR(QFN-32);
3. Changed are r77 from 4.99k/1% to 12.4k/1% and r78 from 3.32k/1% to 10k/1% in order to decrease commercial DDR3L power suplly from 1.5V to 1.35V; The text: "When DDR3L is used, VDD&VDDQ are set to 1.35V!!!" was added in the schematic!

## A10-A20-OLINUXINO-MICRO-EMMC-IND

1. Change memory to industrial variant - MT41K256M16HA-125IT:E(DDR3L-1600_11-11-11)
2. Change PHY to industrial variant LAN8710AI-EZK-TR(QFN-32)
3. Change r77 from 4.99k/1% to 12.4k/1% and r78 from 3.32k/1% to 10k/1% in order to decrease industrial DDR3 power suplly from 1.5V to 1.35V; The text: "When DDR3L is used, VDD&VDDQ are set to 1.35V!!!" was added in the schematic!