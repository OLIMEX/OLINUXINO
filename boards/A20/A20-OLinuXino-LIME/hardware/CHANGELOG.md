# IMPORTANT

A10-OLINUXINO-LIME AND A20-OLINUXINO-LIME SHARE SAME HARDWARE LAYOUT.

A10 AND A20 CHIPS ARE PIN-TO-PIN COMPATIBLE (DROP-IN REPLACEABLE).

ONLY THE MAIN CHIP IS DIFFERENT.

# Hardware revision A

1. Initial release

# Hardware revision B

1. R12, R13(ODT, RST) resistors changed to 0R board mounted for more conistent start up.
2. R8, R14 changed from 430R to 330R, better behavior.

# Hardware revision C

1. Corrected the label of PC3/PB1 to PC3/PB8
2. Added R73 - 10M near the 32768 crystal
3. Added 2 FET transistors and MCP130 for switching directly external +5V without voltage drop
4. USBDRV0 now goes through 3-pin SMD jumper(PB9/PH7) for compatibility reasons 
5. GPIO3(pin9) comes from 3-pin jumper PH7/PB9 for compatibility reasons
6. Shortened LDO4_2.8V wire
7. 10k resistors changed package from 0603 to 0402
8. Adjusted positioning of NAND-a flash pads
9. Added resistor pads between PC16/NWP and pin #19 #WP of NAND memory
10. Added R77 pull down ressitor at MCP130

# Hardware revision D

1. Added pads for 47uF and 2x22uF at both USB hosts - currently NA
2. Added 220uF/6.3V/tant at USB OTG - currently NA
3. Added 220uF/6.3V/tant at SATA pwr - currently NA
4. HDMI konektora s nov korpus rev B
5. Changed width of power-related wores near the main chip from 5 mils to 6 mils
6. Adjusted SATA, USB and HDMI routing
7. C148 now 22uF/6.3V - so SATA_PWR can start up better if there are capacitors, else it might restart
8. R41 changed to 6.81k/1% (down from 13k/1%), this increases the current available at USB1 to 1A6.81k/1%

# Hardware revision E

1. C148 changed from 22uF/0402 to 22uF/0603. 
2. Moved R58 slightly.

# Hardware revision F

1. Connect ODT1, CS1, CKE1 and ZQ1 for the 1GB DDR3 option
2. Added R78-330R/1% for ZQ1
3. Change DDR3 value to 1GB memory (H5TC8G63AMR-PBA) to test
4. Change R68 value from 4.99k/1% to 4.32k/1% in order to decrease DDR3 1.5V to 1.38V  

# Hardware revision G

1. R68 was changed back from 4.32k/1% to 4.99k/1% so VDD DDR3 is set to 1.5V instead of 1.35V, because A10 doesn't support 1.35V DDR3 power supply(according to the sunxi's comparison table!).
2. The Ethernet PHY was changed from RTL8201CP to LAN8710AI-EZK(QFN-32).
3. eMMC option added.
4. VDD_RTC's label changed from 3V3/30mA to 1V3/30mA.
5. The board's holes ware changed to fit case CASE_HAMMOND_1593LBK - with holes grounded and no milling!
6. Added support for capacitve touch screen with 2 matrices. By default it is set for resistive touch panel.
7. 33pF capacitors are now in 0402 package.
8. 49.9R 1% resistors (r80,r81,r82,r83) are now in 0402 package.
9. C187, C188 and C190 were tantalum but changed to 0805 (beter performance).
10. HDMI-CEC is now pulled up with 27K resistor and diode to 3.3V.
11. HDMI-HPD has serial resistor(R97) = 10K. R32 changed from 13k/0603 to 20k/0402.
12. D4 moved to top side (on the place of U20). U20 on the other side moved to the bottom (under PWR_JACK).
13. SPI flash now has jumper WE_ENABLE.
14. R56 now 2K in 0402 package.
15. Capacitors near RAM memory and processor have new and improved routing.
16. C44 moved a bit up.
17. SY8009AAAC changed with SY8089AAAC.
18. DDR memory changed.
19. USB kluchovete sa s priblizitelno 100uf kondenzatori na izhoda.
20. Added pads for 3V back-up AXP209 battery (that can keep RTOS alive)
21. Changed U14 from SY7208 to MT3608

# Hardware revision H

1. R19 now pull-down.
2. GPIO4 konektora had wrong male package; now fixed.
