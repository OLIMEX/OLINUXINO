LCD-OLINUXINO-7 and LCD-OLINUXINO-7TS use the same design files (schematic, board files)

Both boards use the legacy driver board design, not the common LCD-DRIVER board.

Hardware revision A
========
Initial release

Hardware revision B
=====
1. New package for the PWR_JACK.
2. Changed few resistor to 1% tolerance (R1,R2,R9,R21).
3. Added 40-pin conector LCD_A64-OLINUXINO for the A64-OLinuXino board.

Hardware revision C
=====
1. Added 8 resistor positions (pull-up/down) that can be used for binary display identification. Of these 8 positions, 4 resistors have to be populated with 51k/1% and 4 should be left unpopulated. The 7 inch LCD's ID number was set to 0x03. The list of the rest of the display IDs is:

	0x01	-> 4.3" LCD
	0x02	-> 5.0" LCD
	0x03	-> 7.0" LCD
	0x04	-> 10.0" LCD
	0x05	-> 15.4" LCD
	
2. Changed the default position of the 5V/3V SMD jumper to be in position 5V.

Hardware revision D
=====
1. Improvements for all connectors for better soldering and better compatiblity;
2. Fixed lables for SYNK Mode and DE mode in the schematic.

Hardware revision E
=====
1. Board made wider by 40mils. BOM stays the same!

Hardware revision F
=====
1. LCD panel changed with similar but thinner one;
2. TP1, FPA-WZB-25-04-LF, connector added to duplicate the other touch-screen connector for the new 7" LCDs;
3. CE sign added;
4. Widened the 50 mils connector holes from 0.7 to 0.9mm;
5. T1 changed from DTC114YKA to LMUN2211LT1G;
6. Z1 changed from BZV55C6V8 to BZV55C10;
7. R12 changed from 2.2R to 1.1R/1%/R0603 to increase the backlight current from 90mA to 180mA to match the new LCD;
8. R29 changed from 560R/1% to 562R/1%;
9. R16, R17, R18, R35, R36, R37 and R38 changed from 51k/1% to 51.1k/1%;
10. Connectors LCD_LIME and LCD_LIME2 changed fron GPH127SMT-02x20 to MALE-PAV16X-2x20-HIGH-TUBE.
	