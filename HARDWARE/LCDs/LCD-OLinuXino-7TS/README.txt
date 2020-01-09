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
	