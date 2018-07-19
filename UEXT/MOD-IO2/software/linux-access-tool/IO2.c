//      MOD-IO2.c
//      
//      Copyright 2015 OLIMEX LTD
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.


#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "i2c.h"

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

void Print_Help(void);
void Relay(unsigned char value);
void RelayOn(unsigned char value);
void ReadLAT(void);
void SetGPIO(unsigned char value);
void RelayOff(unsigned char value);
void ReadADC(unsigned char value);
void ReadID(void);
void Set_TRIS(unsigned char value);
void ReadGPIO(void);
void Set_PU(unsigned char value);
void Set_LAT(unsigned char value);
void ReadRelays(void);
void ReadSV(void);
void BusScan(void);
void Set_Address(unsigned char value);
void Set_Factory(void);

void Print_Help(void){
	
	puts("\n\
Usage:	./MOD-IO2 [--debug] [parameter] [argument]\n\n\
--verbose	- Print debug messages\n\
--debug		- Same as above\n\n\
Relays commands:\n\
-r, --relays?	- Read Relays states\n\
-s X, --switchon	- Turn On Relay\n\
-n X, --switchoff - Turn Off Relay\n\
     where X is:\n\
		1 - Relay 1\n\
		2 - Relay 2\n\
		3 both\n\
	 example:\n\
MOD-IO2 -s 1     - turns relay 1 on\n\
-S, --setrelays - Set Relays Outputs\n\n\
GPIO commands:\n\
-g, --getport - Read GPIO Inputs\n\
-t, --settris - Set GPIO TRIS\n\
-p, --setpullups - Set GPIO PullUps\n\n\
ADC commands:\n\
-a, --adc - Read ADC value in Volts\n\
\n\
-i, --id - Read Module Hardware Version ID\n\
-?, --firmware - Read Module Firmware Version\n\
--help		- Print this help\n\n\
");
exit(0);
}

int main(int argc, char **argv){
	
// Set I2C bus 	
	DEVICE = 2; // /dev/i2c-2 Olinuxino A20 micro, change if other board or i2c port used
	
	/* Read  program options */
	while(1){
		static struct option long_options[]=
		{
			{"verbose", no_argument,	&_DEBUG,	1},
			{"debug",	no_argument,	&_DEBUG,	1},
			
			
			{"relays?",	no_argument,0,'r'},
			{"adc",		required_argument,0,'a'},
			{"id",		no_argument,0,'i'},
			{"getport",		no_argument,0,'g'},
			{"getlat", no_argument,0,'G'},
			{"setrelays",	required_argument,0,'S'},
			{"setoutputs",	required_argument,0,'o'},
			{"settris",	required_argument,0,'t'},
			{"setpullups",	required_argument,0,'p'},
			{"switchon",	required_argument,0,'s'},
			{"switchoff",	required_argument,0,'n'},
			{"firmware",no_argument,0,'f'},
			{"help",	no_argument,0,'h'},
			{"busscan",	no_argument,0,'F'},
			{"setaddress", required_argument,0,'x'},
			{"setfactory",	no_argument,0,'X'},
			{0,		0, 			0,		0}
		};
		int option_index = 0;
		int c = getopt_long(argc, argv, "s:t:a:igGrp:o:S:n:fhx:bX?", long_options, &option_index);
		unsigned int value; 
		if (c == -1)
		{
			break;
		}
		
		switch(c)
		{
			case 'v':
				_DEBUG = 1;
				break;
			case 'h':
				Print_Help();
				break;
				
			case 'S':
				value = atoi(optarg);
				Relay(value);
				break;
			case 's':
				value = atoi(optarg);
				RelayOn(value);
				break;					
			case 'o':
				value = atoi(optarg);
				SetGPIO(value);
				break;	
			case 'n':
				value = atoi(optarg);
				RelayOff(value);
				break;				
			case 'b':
				DEVICE = atoi(optarg);
				break;					
			case 'a':
				value = atoi(optarg);
				ReadADC(value);
				break;
			case 'g':
				
				ReadGPIO();
				break;	
			case 'G':
				
				ReadLAT();
				break;	
			case 'F':
				BusScan();
				break;						
			case 'i':
				
				ReadID();
				break;	
			case 'X':
				Set_Factory();
				break;			
			case 'r':
				ReadRelays();
				break;
			case 't':
				value = atoi(optarg);
				Set_TRIS(value);
				break;
			case 'x':
				value = atoi(optarg);
				Set_Address(value);
				break;						
			case 'p':
				value = atoi(optarg);
				Set_PU(value);
				break;	
			case 'f':
			ReadSV();			
				break;
				
			default:
			
		//	Print_Help();
				break;
		}
	
	}
	
	
		
	return 0;
}

/* Use this function to find module i2c address */
void BusScan(void)
{
	int fd;
	unsigned char buff[5];
	unsigned char data[5];
	unsigned char adrs;
	
	
for (adrs = 1; adrs < 129; adrs++)
{	
	buff[0]=0x20;
	data[0]=0x00;

	
	/* Open I2C-BUS */	
	I2C_Open(&fd, adrs);
	
	if (1)
	{
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read the ADC */
	
	I2C_Read(&fd, data, 1);
	if (data[0]==0x23)
	printf("Found with I2c address 0x%02x \n", adrs);

	/* Close I2C-BUS */
	I2C_Close(&fd);
}
	
} 	
}


/* Set GPIO Outputs */
void Set_LAT(unsigned char value){
	int fd;
	unsigned char buff[5];
	
	
	buff[0]=0x02;
	buff[1]=value;
	

	/* Open I2C-BUS */	
	I2C_Open(&fd, 0x21);
	
	/* Write register */
	I2C_Send(&fd, buff,2 );

	/* Close I2C-BUS */
	I2C_Close(&fd);
	
}

/* Switch Relay Off */
void RelayOff(unsigned char value){
	int fd;
	unsigned char buff[5];
	
	
	buff[0]=0x42;
	buff[1]=value;
	
	/* value:
	 * 1 - switch relay 1 off
	 * 2 - switch relay 2 off
	 * 3 - switch both relays off
	 * /

	/* Open I2C-BUS */	
	I2C_Open(&fd, 0x21);
	
	/* Write register */
	I2C_Send(&fd, buff,2 );

	/* Close I2C-BUS */
	I2C_Close(&fd);
	
}

/* Switch Relay On */
void RelayOn(unsigned char value){
	int fd;
	unsigned char buff[5];
	
	
	buff[0]=0x41;
	buff[1]=value;
	
	/* value:
	 * 1 - switch relay 1 on
	 * 2 - switch relay 2 on
	 * 3 - switch both relays on
	 * /

	/* Open I2C-BUS */	
	I2C_Open(&fd, 0x21);
	
	/* Write register */
	I2C_Send(&fd, buff,2 );

	/* Close I2C-BUS */
	I2C_Close(&fd);
	
}

/* Set Relay states */
void Relay(unsigned char value){
	int fd;
	unsigned char buff[5];
	
	
	buff[0]=0x40;
	buff[1]=value;
	
	/* value:
	 * should be iterpreted as binary 
	 * b0 is relay 1
	 * b1 is relay 2
	 * 0 turn of relay
	 * 1 turn on relay
	 * /
	

	/* Open I2C-BUS */	
	I2C_Open(&fd, 0x21);
	
	/* Write register */
	I2C_Send(&fd, buff,2 );

	/* Close I2C-BUS */
	I2C_Close(&fd);
	
}
/* Set New I2C device address */
void Set_Address(unsigned char value){
	int fd;
	unsigned char buff[5];

	
	
	buff[0]=0xF0;
	buff[1]=value;
	
	/* value:
	 * New Address to be setted
	 * /
	

	/* Open I2C-BUS */	
	I2C_Open(&fd, 0x21);
	
	/* Write register */
	I2C_Send(&fd, buff,2 );

	/* Close I2C-BUS */
	I2C_Close(&fd);
	
}
/* Set Factory I2C device address */
void Set_Factory(void){
	int fd;
	unsigned char buff[5];
	unsigned char data[5];
	unsigned char adrs;
	
	
for (adrs = 0; adrs < 129; adrs++)
{	
	buff[0]=0x20;
	data[0]=0x00;

	
	/* Open I2C-BUS */	
	I2C_Open(&fd, adrs);
	
	if (1)
	{
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read the ADC */
	
	I2C_Read(&fd, data, 1);
	if (data[0]==0x23)
	{
	/* Close I2C-BUS */
	I2C_Close(&fd);		
		printf("Found with I2c address 0x%02x \n", adrs);
	buff[0]=0xF0;
	buff[1]=0x21;
	
	/* value:
	 * New Address to be setted
	 * /
	

	/* Open I2C-BUS */	
	I2C_Open(&fd, adrs);
	
	/* Write register */
	I2C_Send(&fd, buff,2 );

	/* Close I2C-BUS */
	I2C_Close(&fd);	
		printf("Reverted to 0x21\n");
		
	}	

	/* Close I2C-BUS */
	I2C_Close(&fd);
}

	
}	
	
	
}
/* Set GPIO Pull Ups */
void Set_PU(unsigned char value){
	int fd;
	unsigned char buff[5];

	
	
	buff[0]=0x04;
	buff[1]=value;
	
	/* value:
	 * 0b000dxddd
	 * bit0 coresponding to GPIO0, 
	 * as "1" enables the pullup and "0" - disables it.
	 * /
	

	/* Open I2C-BUS */	
	I2C_Open(&fd, 0x21);
	
	/* Write register */
	I2C_Send(&fd, buff,2 );

	/* Close I2C-BUS */
	I2C_Close(&fd);
	
}

/* Set GPIO outputs */
void SetGPIO(unsigned char value){
	int fd;
	unsigned char buff[5];

	
	
	buff[0]=0x02;
	buff[1]=value;
	
		/* value:
	 * 0b0ddddddd
	 * bit0 coresponding to GPIO0, 
	 * as "1" set pin high "0" - set pin low.
	 * /
	

	/* Open I2C-BUS */	
	I2C_Open(&fd, 0x21);
	
	/* Write register */
	I2C_Send(&fd, buff,2 );

	/* Close I2C-BUS */
	I2C_Close(&fd);
	
}

/* Set pin directions */
void Set_TRIS(unsigned char value){
	int fd;
	unsigned char buff[5];

	
	
	buff[0]=0x01;
	buff[1]=value;
	
		/* value:
	 * 0b0ddddddd
	 * bit0 coresponding to GPIO0, 
	 * as "1" set pin as input "0" - set pin as output.
	 * /
	

	/* Open I2C-BUS */	
	I2C_Open(&fd, 0x21);
	
	/* Write register */
	I2C_Send(&fd, buff,2 );

	/* Close I2C-BUS */
	I2C_Close(&fd);
	
}

/* Read Board ID */
void ReadID(void)
{
	int fd;
	unsigned char buff[5];
	unsigned char data[5];
	
	
	buff[0]=0x20;
	data[0]=0x00;
	
	
	/* Open I2C-BUS */	
	I2C_Open(&fd, 0x21);
	
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read ID */
	
	I2C_Read(&fd, data, 1);
	printf("ID: 0x%x\n", data[0]);

	/* Close I2C-BUS */
	I2C_Close(&fd);


}

/* Read Board Firmware Version */
void ReadSV(void)
{
	int fd;
	unsigned char buff[5];
	unsigned char data[5];
	
	
	buff[0]=0x21;
	data[0]=0x00;
	
	
	/* Open I2C-BUS */	
	I2C_Open(&fd, 0x21);
	
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read the ADC */
	
	I2C_Read(&fd, data, 1);
	printf("Firmware Version: %d.0.%2d\n", data[0]>>4,data[0]&0x0f);

	/* Close I2C-BUS */
	I2C_Close(&fd);


}

/* Read Relays States */
void ReadRelays(void)
{
	int fd;
	unsigned char buff[5];
	unsigned char data[5];
	
	
	buff[0]=0x43;
	data[0]=0x00;
	
	
	/* Open I2C-BUS */	
	I2C_Open(&fd, 0x21);
	
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read the ADC */
	
	I2C_Read(&fd, data, 1);
	
	printf("Relays: 0x%02x\n", data[0]);

	/* Close I2C-BUS */
	I2C_Close(&fd);


}
/* Read GPIO Digital Outputs */
void ReadLAT(void)
{
	int fd;
	unsigned char buff[5];
	unsigned char data[5];
	
	
	buff[0]=0x06;
	data[0]=0x00;
	
	
	/* Open I2C-BUS */	
	I2C_Open(&fd, 0x21);
	
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read the ADC */
	
	I2C_Read(&fd, data, 1);
	
	printf("GPIO: 0x%02x\n", data[0]);

	/* Close I2C-BUS */
	I2C_Close(&fd);


}
/* Read GPIO Digital Inputs */
void ReadGPIO(void)
{
	int fd;
	unsigned char buff[5];
	unsigned char data[5];
	
	
	buff[0]=0x03;
	data[0]=0x00;
	
	
	/* Open I2C-BUS */	
	I2C_Open(&fd, 0x21);
	
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read the ADC */
	
	I2C_Read(&fd, data, 1);
	
	printf("GPIO: 0x%02x\n", data[0]);

	/* Close I2C-BUS */
	I2C_Close(&fd);


}

/* Read GPIO Analog Inputs */
void ReadADC(unsigned char value)
{
	int fd;
	unsigned char buff[5];
	unsigned char data[5];
	
	
	switch(value)
	{
	case 255:
	ReadADC(0);
	ReadADC(1);
	ReadADC(2);
	ReadADC(3);
	ReadADC(5);
	break;	
	case 0:	
	case 1:
	case 2:
	case 3:
	case 5:	
	
	buff[0]=0x10+value;
	data[0]=0x00;
	
	
	/* Open I2C-BUS */	
	I2C_Open(&fd, 0x21);
	
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read the ADC */
	
	I2C_Read(&fd, data, 2);
	/* Convert to Volts. Vref = 3.3V, ADC is 10 bits */
	float volts = data[0] * 0.003222656 + data[1]*0.825;
	printf("ADC%u: %1.3fV\n",value, volts);

	/* Close I2C-BUS */
	I2C_Close(&fd);
	break;
	default:
	
	printf("ADC%u not found\n",value);
}
}
