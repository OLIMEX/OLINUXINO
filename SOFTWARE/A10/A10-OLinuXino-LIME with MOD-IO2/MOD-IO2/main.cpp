#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "i2c.h"
#include "IO2.h"

IO2 io2(0x21);

int main ()
{
	io2.pinMode(io2.GPIO0 | io2.GPIO1, io2.IN);
	io2.setPullpin(io2.GPIO0, io2.OFF);
	io2.setPullpin(io2.GPIO1, io2.ON);

	while (1)
	{
		// read analog input 0
		if (io2.analogRead (io2.AN0) < 0x1FF)
			io2.setRelay(io2.RELAY1, io2.ON);
		else
			io2.setRelay(io2.RELAY1, io2.OFF);

		// read digital input 1
		if (io2.digitalRead(io2.GPIO1))
			io2.setRelay (io2.RELAY2, io2.ON);
		else
			io2.setRelay (io2.RELAY2, io2.OFF);
	}
}
