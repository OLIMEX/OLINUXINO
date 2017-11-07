LCD-OLINUXINO-10 and LCD-OLINUXINO-10TS use the same design files (schematic, board files)

Hardware revision A
========
Initial release

Hardware revision B
========
1. Added connector for A64-OLinuXino.
2. R1 and R2 changed to 20k/1%.
3. Changed the PWR_JACK connector.

Hardware revision C
========
1. Added 8 resistor positions (pull-up/down) that can be used for binary display identification. Of these 8 positions, 4 resistors have to be populated with 51k/1% and 4 should be left unpopulated. The 10 inch LCD's ID number was set to 0x04. The list of the rest of the display IDs is:

	0x01	-> 4.3" LCD
	0x02	-> 5.0" LCD
	0x03	-> 7.0" LCD
	0x04	-> 10.0" LCD
	0x05	-> 15.4" LCD
	
2. Fixed wrong label about the modes in the schematic. In MODE=0, SYNK Mode - DE must be grounded. In MODE=1, DE Mode - DE must be VS and HS must be pulled high.

Hardware revision C1
=========
1. Certain displays required Z1 change from BZV55C8V2 to BZV55C5V1.


