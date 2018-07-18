#include "LED8x8RGB.h"

#define	DELAY	10000000

void Delay (volatile unsigned long int Time)
{
	while (--Time);
}

LCD8x8RGB LCD;

const int BLX = (bigX * 8) / 2;
const int BLY = (bigY * 8) / 2;
const int URX = (bigX * 8) / 2 + 1;
const int URY = (bigY * 8) / 2 + 1;

int main ()
{
	int i=0;

	LCD.vClear ();
	LCD.Transfer ();
	while (1)
	{
		LCD.setColor (1);
		LCD.scrollString ((unsigned char *)"Demo for A20-OLinuXino-Micro and MOD-LED8x8RGB", Left);
		for (i=0; i<4; i++)
		{
			LCD.setColor (i+1);
			LCD.vClear ();
			LCD.drawRectangle(BLX-i*bigX,BLY-i*bigY,URX+i*bigX,URY+i*bigY);
			
			LCD.Transfer ();
			Delay (DELAY*10);
		}
		LCD.vClear ();
		LCD.Transfer ();
	}
}
