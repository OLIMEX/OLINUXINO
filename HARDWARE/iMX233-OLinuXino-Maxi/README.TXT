iMX233-OLinuXino-MAXI hardware change log

Major changes:
	

iMX233-OLinuXino-MAXI hardware revision B1
======
1. Initial release

iMX233-OLinuXino-MAXI hardware revision B2
======
1. Removed FET1

iMX233-OLinuXino-MAXI hardware revision C
======
1. Changed the name in the white print "iMX233-OLINUXINO_Rev_C" to "iMX233-OLINUXINO-MAXI_Rev_C";
2. BOOT jumpers moved to the bottom and separated further apart; 
3. Jumpers 3.3V_E and 3.3VIO_E moved to the bottom and separated apart;
4. Jumpers SW_SCL/HW_SCL and SW_SDA/HW_SDA made bigger and default position changed to HW I2C positions;
5. Moved 5V_E jumper to the bottom of the board;
6. Removed FET that is in series to the power input;
7. Q2 moved up from D1;
8. 40-pin GPIO connector - edited the symbol to indicated the hole of hte plastic connector better;
9. PWR_JACK and RCA connectors separated further by 20 mils;
10. Atted schotky diode in series of RX and R1 changed from 100k to 4.7K;

iMX233-OLinuXino-MAXI hardware revision C1
======
1. Changed C23 and C24 from 33pF to 15pf for better start up behavior


iMX233-OLinuXino-MAXI hardware revision C2
======
1. Changed C23, C24 from 15pF to 4.7pF because the crystal starts better
2. Changed C83 from 10pF to 100pF because the board starts better on battery 

iMX233-OLinuXino-MAXI hardware revision D
======
1. Changed SD card package with a better one;
2. Fixed LiPo bat white print;
3. Changed C23, C24 from 4.7pF to 10pF, since 4.7 starts better, but loses color of vidoe si since the display color gets lost (frequency changes);

iMX233-OLinuXino-MAXI hardware revision D1
======
1. Changed C83 from 100pF to 20pF. The problem was that the Debiam 3.11 image does not recognize mass storage drives. Other usb devices can be rocognized. The capacitor values is critical because if 100pf is placed the board start better on battery, but does not recognize Mass storage drive. IF 10pF is placed - the mass storage drive is recognized but the board starts more dificult only with battery supply. So the compromise is 20 pF. 
