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
void SetPin(unsigned char value);
void ReadPin(unsigned char value);
void SetPWM1(unsigned char value);
void SetPWM2(unsigned char value);
void ClosePWM(unsigned char value);


unsigned char i2caddr;
unsigned char mask[9]={1,2,4,8,16,32,64,128};
unsigned char imask[9]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
unsigned char hout = 1;




void Print_Help(void){
	
	puts("\n\
\n\
MOD-IO2 tool - http://www.Olimex.com/ , ver 1.02\n\n\
Usage:	./modio2tool [--debug] [-B X] [-A 0xXX] [-m] [commands] [argument]\n\n\
--verbose		- Prints debug messages\n\
--debug			- Same as above\n\n\
-B X, 	--setbusnumber	- Sets /dev/i2c-X, default 2\n\
-A 0xXX, --i2c_address	- Sets target i2c address, default 0x21\n\
-x 0xXX, --setaddress	- Sets the i2c address of the module to 0xXX\n\
-X, 	--setfactory	- Sets module i2c address to default 0x21\n\n\
Relay commands:\n\n\
-r,	--relays?	- Reads relay states\n\
-s X, 	--switchon	- Turns on relay\n\
-c X, 	--switchoff	- Turns off relay\n\
 where X is:\n\
  1 - Relay 1\n\
  2 - Relay 2\n\
  3 - both\n\
 example:\n\
  modio2tool -s 1	- Turns relay 1 on\n\
-S X,	--setrelays	- Sets relays outputs\n\n\
GPIO commands:\n\n\
-g,	--getport	- Reads the GPIO inputs\n\
-G,	--getlat	- Reads the GPIO outputs\n\
-T 0xXX, --settris	- Sets GPIO TRIS\n\
-P 0xXX, --setpullups	- Sets GPIO pull ups\n\
-i X, 	--readpin	- Reads digital input at GPIOX\n\
-p XY,	--pin		- Sets digital output at GPIOX\n\
 X - 0-7   GPIO#\n\
 Y - 0/1 - Level, 3 - toggle\n\
\n\
ADC commands:\n\n\
-a X, --adc		- Prints ADC values in Volts\n\
\n\
PWM's:\n\n\
-w 0xXX, --openpwm1	- Set PWM1 value and turn it on\n\
-W 0xXX, --openpwm2	- Set PWM2 value and turn it on\n\
-z X,	 --closepwm	- Close PWMx\n\n\
Others:\n\n\
-l, --listdev		- Lists addresses of modules connected to bus\n\
-V, --id		- Prints the hardware ID of MOD-IO2\n\
-f, --firmware		- Prints the current firmware version of MOD-IO2\n\
-m, --strip		- Omits human messages\n\
-h, --help		- Prints this help information\n\n\
");
exit(0);
}

int main(int argc, char **argv){
	
// Set I2C bus 	
	DEVICE = 2; // /dev/i2c-2 Olinuxino A20 micro, change if other board or i2c port used
	i2caddr = 0x21;
	/* Read  program options */
	while(1){
		static struct option long_options[]=
		{
			{"verbose", no_argument,	&_DEBUG,	1},
			{"debug",	no_argument,	&_DEBUG,	1},
				
			{"relays?",	no_argument,0,'r'},
			{"adc",		required_argument,0,'a'},
			{"i2c_address",		required_argument,0,'A'},
			{"hwid",		no_argument,0,'V'},
			{"getport",		no_argument,0,'g'},
			{"getlat", no_argument,0,'G'},
			{"setrelays",	required_argument,0,'S'},
			{"setoutputs",	required_argument,0,'o'},
			{"settris",	required_argument,0,'t'},
			{"setpullups",	required_argument,0,'P'},
			{"seton",	required_argument,0,'s'},
			{"setoff",	required_argument,0,'c'},
			{"firmware",no_argument,0,'f'},
			{"help",	no_argument,0,'h'},
			{"listdev",	no_argument,0,'l'},
			{"setaddress", required_argument,0,'x'},
			{"setbusnumber",required_argument,0,'B'},
			{"pin",required_argument,0,'p'},
			{"readpin",required_argument,0,'i'},
			{"openpwm1",required_argument,0,'w'},
			{"openpwm2",required_argument,0,'W'},
			{"closepwm",required_argument,0,'z'},
			{"setfactory",	no_argument,0,'X'},
			{"strip",no_argument,0,'m'},
			{0,		0, 			0,		0}
		};
		int option_index = 0;
		int c = getopt_long(argc, argv, "s:T:a:VgGrp:o:S:c:fhx:B:XlA:p:i:mw:W:z:", long_options, &option_index);
		unsigned int value; 
		if (c == -1)
		{
			break;
		}
		
		switch(c)
		{
			case 'm':
				hout = 0;
				break;
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
			case 'A':
			i2caddr = strtol(optarg,NULL,16);
			break;			
			case 'o':
				value = strtol(optarg,NULL,16);
				SetGPIO(value);
				break;	
			case 'c':
				value = atoi(optarg);
				RelayOff(value);
				break;				
				
			case 'a':
				value = atoi(optarg);
				ReadADC(value);
				break;
			case 'g':
				
				ReadGPIO();
				break;	
			case 'i':
				value = atoi(optarg);
				ReadPin(value);
				break;					
			case 'G':
				
				ReadLAT();
				break;
			case 'p':
			value = strtol(optarg,NULL,16);
			SetPin(value);
			break;
			case 'B':
			DEVICE = atoi(optarg);
			break;		
			case 'l':
				BusScan();
				break;						
			case 'V':
				
				ReadID();
				break;	
			case 'X':
				Set_Factory();
				break;			
			case 'r':
				ReadRelays();
				break;
			case 'T':
				value = strtol(optarg,NULL,16);
				Set_TRIS(value);
				break;
			case 'x':
				value = strtol(optarg,NULL,16);
				Set_Address(value);
				break;						
			case 'P':
				value = strtol(optarg,NULL,16);
				Set_PU(value);
				break;	
			case 'f':
			ReadSV();			
				break;
			case 'w':
			value = atoi(optarg);
			SetPWM1(value);	
			break;;
			case 'W':
			value = atoi(optarg);
			SetPWM2(value);	
			break;
			default:			
		//	Print_Help();
				break;
		}
	
	}
	
	
		
	return 0;
}
/* Set PWM mode on GPIO5 - PWM2 */
void SetPWM2(unsigned char value)
{
	/* value:
	 * 0 to 255
	 */
	int fd;
	unsigned char buff[5];
	buff[0]=0x52;
	buff[1]=value;


	/* Open I2C-BUS */	
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	if (I2C_Send(&fd, buff,2 ))
	if (hout)
	printf("error!\n");
	else
	if (hout)
	printf("done.\n");

	/* Close I2C-BUS */
	I2C_Close(&fd);
	
 }
/* Set PWM mode on GPIO6 - PWM1 */
void SetPWM1(unsigned char value)
{
	/* value:
	 * 0 to 255
	 */
	int fd;
	unsigned char buff[5];
	buff[0]=0x51;
	buff[1]=value;


	/* Open I2C-BUS */	
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	if (I2C_Send(&fd, buff,2 ))
	if (hout)
	printf("error!\n");
	else
	if (hout)
	printf("done.\n");

	/* Close I2C-BUS */
	I2C_Close(&fd);
	
 }
 /* Close PWM 1 or 2 */
void ClosePWM(unsigned char value)
{
		/* value:
	 * 1 or 2 - PWM1 or PWM2, 0 - both
	 */
	int fd;
	unsigned char buff[5];
	
	switch(value){
	 case 0:
	  ClosePWM(1);
	  ClosePWM(2);
	  break;
	 case 1:
	  buff[1] =1;
	  break;
	 case 2:
	 buff[1]=2;
	 break;
	 default:
	 exit(1);
	 break;
 }
	
	buff[0]=0x50;
	buff[1]=value;


	/* Open I2C-BUS */	
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	if (I2C_Send(&fd, buff,2 ))
	if (hout)
	printf("error!\n");
	else
	if (hout)
	printf("done.\n");

	/* Close I2C-BUS */
	I2C_Close(&fd);
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
	{
		if (hout)
	printf("Found with I2c address 0x%02x \n", adrs);
       else printf("0x%02x\n",adrs);
   }
	/* Close I2C-BUS */
	I2C_Close(&fd);
}
	
} 	
}

/* Read Pin individualy */
void ReadPin(unsigned char value){
	int fd;
	unsigned char buff[5];
	unsigned char data[5];
    

	buff[0]=0x03;
	data[0]=0x00;
	data[1]=0;
	
	
	/* Open I2C-BUS */	
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read the GPIO */
	I2C_Read(&fd, data, 1);
	if (data[0] & mask[value]) data[1]=1;
	if (hout)
	printf("GPIO%d: %01x\n",value, data[1]);
 else
	printf("%01x\n", data[1]);
	/* Close I2C-BUS */
	I2C_Close(&fd);
		
}

/* Set Pin Outputs Individualy */
void SetPin(unsigned char value){
	int fd;
	unsigned char buff[5];
	unsigned char data[5];

	
	
	/* value >> 4 = GPIOx
	   value & 1 = L/H */
	
	
	/* Read LAT first! */
	buff[0]=0x06;
	data[0]=0x00;
	
	
	/* Open I2C-BUS */	
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read the ADC */
	
	I2C_Read(&fd, data, 1);
	/* Close I2C-BUS */
	I2C_Close(&fd);

    /* Set Bit
     * 0 - set low
     * 1 - set high
     * 3 - toggle */ 
     
    switch (value & 3)
    {
	case 0:
		data[0]&=imask[value>>4];
		break;
	case 1:
		data[0]|=mask[value>>4];	
		break;	
	case 3:	
	data[0]^=mask[value>>4];
		break;
	default:
	break;
	
	}
		
	/* write it back */
    buff[0]=0x02;
	buff[1]=data[0];
	/* Open I2C-BUS */	
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	if (I2C_Send(&fd, buff,2 ))
		printf("error!\n");
	else
	{
	if (hout)
	printf("done.\n");
	}
	/* Close I2C-BUS */
	I2C_Close(&fd);
	ReadLAT();
}

/* Set GPIO Outputs */
void Set_LAT(unsigned char value){
	int fd;
	unsigned char buff[5];
	
	
	buff[0]=0x02;
	buff[1]=value;
	

	/* Open I2C-BUS */	
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	if (I2C_Send(&fd, buff,2 ))
	 if (hout)
		printf("error!\n");
	
	else
	if (hout) printf("done.\n");

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
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	if (I2C_Send(&fd, buff,2 ))
	if (hout)
	printf("error!\n");

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
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	if (I2C_Send(&fd, buff,2 ))
	if (hout)
	printf("error!\n");

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
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	if (I2C_Send(&fd, buff,2 ))
	if (hout)
	printf("error!\n");

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
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	if (I2C_Send(&fd, buff,2 ))
	if (hout)
		printf("error!\n");
	else
	if (hout)
	printf("done.\n");

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
	if (hout)	
		printf("...found with i2c address 0x%02x \n", adrs);
		else
		printf("0x%02x\n", adrs);
	buff[0]=0xF0;
	buff[1]=0x21;
	
	/* value:
	 * New Address to be setted
	 * /
	

	/* Open I2C-BUS */	
	I2C_Open(&fd, adrs);
	
	/* Write register */
	if (I2C_Send(&fd, buff,2 ))
	{
		if (hout)
		printf("error!\n");
	exit(1);
	}
	else
	printf("done...");

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
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	if (I2C_Send(&fd, buff,2 ))
	if (hout)
	printf("error!\n");
	else
	if (hout)
	printf("done.\n");

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
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	if (I2C_Send(&fd, buff,2 ))
		if (hout) printf("error!\n");
	else
	if (hout) printf("done.\n");

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
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	if(I2C_Send(&fd, buff,2 ))
	if (hout) printf("error!\n");
	else
	if (hout) printf("done.\n");
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
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read ID */
	
	I2C_Read(&fd, data, 1);
	if (hout) 
	printf("ID: 0x%x\n", data[0]);
    else
    printf("0x%x\n", data[0]);
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
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read the FV */
	
	I2C_Read(&fd, data, 1);
	printf("Firmware Version: %d.%02d\n", data[0]>>4,data[0]&0x0f);

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
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read the ADC */
	
	I2C_Read(&fd, data, 1);
	if (hout)
	printf("Relays: 0x%02x\n", data[0]);
    else
    printf("0x%02x\n", data[0]);
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
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read the ADC */
	
	I2C_Read(&fd, data, 1);
	if (hout)
	printf("GPIO: 0x%02x\n", data[0]);
    else
    printf("0x%02x\n", data[0]);
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
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read the GPIO */
	
	I2C_Read(&fd, data, 1);
	if (hout)
	printf("GPIO: 0x%02x\n", data[0]);
    else
    printf("0x%02x\n", data[0]);
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
	ReadADC(7);
	break;	
	case 0:	
	case 1:
	case 2:
	case 3:
	case 7:	
	
	buff[0]=0x10+value;
	data[0]=0x00;
	
	
	/* Open I2C-BUS */	
	I2C_Open(&fd, i2caddr);
	
	/* Write register */
	I2C_Send(&fd, buff,1 );
	
	/* Read the ADC */
	
	I2C_Read(&fd, data, 2);
	/* Convert to Volts. Vref = 3.3V, ADC is 10 bits */
	float volts = data[0] * 0.003222656 + data[1]*0.825;
	if (hout)
	printf("ADC%u: %1.3fV\n",value, volts);
    else
    printf("%1.3f\n",volts);
	/* Close I2C-BUS */
	I2C_Close(&fd);
	break;
	default:
	
	printf("ADC%u not found\n",value);
}
}
