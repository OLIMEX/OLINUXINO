//      RGB.c
//      
//      Copyright 2013 stefan <stefan@debian>
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

void Print_Help(void)
{
	puts("\n\
--verbose	- Print debug messages\n\
--debug		- Same as above\n\
--help		- Print this help\n");
exit(0);
}

int main(int argc, char **argv)
{
	int fd = 1;
	char ch[10];
	unsigned char data[10];
	unsigned char address;
	unsigned char device;
	int R, G, B;
	int choice;

	while(1)
	{
		static struct option long_options[]=
		{
			{"verbose", no_argument,	&_DEBUG,	1},
			{"debug",	no_argument,	&_DEBUG,	1},
			
			{"help",	no_argument,	0,		'h'},
			{0,		0, 			0,		0}
		};
		int option_index = 0;
		int c = getopt_long(argc, argv, "hv", long_options, &option_index);
		
		if (c == -1)
			break;
		switch(c)
		{
			case 'h':
				Print_Help();
				break;
				
			case '?':
				break;
				
			default:
				break;
		}
	
	}
	
	while(1)
	{
		printf("%sI2C-bus%s: ", WHITE, GRAY);
		fgets(ch, 10, stdin);
		device = strtol(ch, NULL, 0);
		if(device < 0 || device > 5)
			printf("%sInvalid i2c-bus....%s\n", WHITE, GRAY);
		else
		{
			DEVICE = device;
			break;
		}
	}
	
	printf("\n%sMOD-RGB%s\n", WHITE, GRAY);
	printf("--------------------\n\n");
	printf("%s1%s. START PWM\n", WHITE, GRAY);
	printf("%s2%s. STOP PWM\n", WHITE, GRAY);
	printf("%s3%s. SET RGB\n", WHITE, GRAY);
	printf("%s4%s. START AUDIO\n", WHITE, GRAY);
	printf("%s5%s. STOP AUDIO\n", WHITE, GRAY);
	printf("%s6%s. SET ADDRESS\n", WHITE, GRAY);
	printf("%s0%s. EXIT\n\n", WHITE, GRAY);
	
	while(1)
	{
		printf("%sEnter%s: ", WHITE, GRAY);
		fgets(ch, 10, stdin);
		choice = strtol(ch, NULL, 0);
		if(choice < 0 || choice > 6)
			printf("%sInvalid choice...%s\n", WHITE, GRAY);
		else
			break;	
		
	}
	
	
	switch(choice)
	{
		case 1:
			printf("%sAddress%s: ", WHITE, GRAY);
			fgets(ch, 10, stdin);
			address = strtol(ch, NULL, 0);
			if(_DEBUG)
				printf("Setting data...");
			data[0] = 0x01;		//COMMAND 
			if(_DEBUG)
				printf("%sDONE%s\n", GREEN, GRAY);
			
			I2C_Open(&fd, address);	
			I2C_Send(&fd, data, 1);
			I2C_Close(&fd);
			break;
			
			
		case 2:
			printf("%sAddress%s: ", WHITE, GRAY);
			fgets(ch, 10, stdin);
			address = strtol(ch, NULL, 0);
			if(_DEBUG)
				printf("Setting data...");
			data[0] = 0x02;		//COMMAND 
			if(_DEBUG)
				printf("%sDONE%s\n", GREEN, GRAY);
			
			I2C_Open(&fd, address);	
			I2C_Send(&fd, data, 1);
			I2C_Close(&fd);
			break;
			
		case 3:
			printf("%sAddress%s: ", WHITE, GRAY);
			fgets(ch, 10, stdin);
			address = strtol(ch, NULL, 0);
			if(_DEBUG)
				printf("Selected address: %s0x%02X\n%s", WHITE, address, GRAY);
			while(1)
			{
				printf("%sR%s: ", WHITE, GRAY);
				fgets(ch, 10, stdin);
				R = strtol(ch, NULL, 0);
				if(R < 0 || R > 255)
					printf("%sValue must be between 0 and 255...%s\n", WHITE, GRAY);
				else
					break;		
			}
			while(1)
			{
				printf("%sG%s: ", WHITE, GRAY);
				fgets(ch, 10, stdin);
				G = strtol(ch, NULL, 0);
				if(G < 0 || G > 255)
					printf("%sValue must be between 0 and 255...%s\n", WHITE, GRAY);
				else
					break;		
			}
			while(1)
			{
				printf("%sB%s: ", WHITE, GRAY);
				fgets(ch, 10, stdin);
				B = strtol(ch, NULL, 0);
				if(B < 0 || B > 255)
					printf("%sValue must be between 0 and 255...%s\n", WHITE, GRAY);
				else
					break;		
			}
			
			if(_DEBUG)
				printf("Setting data...");
			data[0] = 0x03;		//COMMAND
			data[1] = (unsigned char)R;
			data[2] = (unsigned char)G;
			data[3] = (unsigned char)B;
			if(_DEBUG)
			{
				int i;
				printf("%sDONE%s\n", GREEN, GRAY);
				printf("DATA[%s", WHITE); 
				for(i = 0; i < 4; i++)
					printf("0x%02X ", data[i]);
				printf("%s]\n", GRAY); 
			}
			
			I2C_Open(&fd, address);	
			I2C_Send(&fd, data, 4);
			I2C_Close(&fd);
			break;
			
			
			
		case 4:
			printf("%sAddress%s: ", WHITE, GRAY);
			fgets(ch, 10, stdin);
			address = strtol(ch, NULL, 0);
			if(_DEBUG)
				printf("Setting data...");
			data[0] = 0x14;		//COMMAND 
			if(_DEBUG)
				printf("%sDONE%s\n", GREEN, GRAY);
			
			I2C_Open(&fd, address);	
			I2C_Send(&fd, data, 1);
			I2C_Close(&fd);
			break;
			
		case 5:
			printf("%sAddress%s: ", WHITE, GRAY);
			fgets(ch, 10, stdin);
			address = strtol(ch, NULL, 0);
			if(_DEBUG)
				printf("Setting data...");
			data[0] = 0x15;		//COMMAND 
			if(_DEBUG)
				printf("%sDONE%s\n", GREEN, GRAY);
			
			I2C_Open(&fd, address);	
			I2C_Send(&fd, data, 1);
			I2C_Close(&fd);
			break;
			
		case 6:
			printf("%sAddress%s: ", WHITE, GRAY);
			fgets(ch, 10, stdin);
			address = strtol(ch, NULL, 0);
			printf("%sNew Address%s: ", WHITE, GRAY);
			fgets(ch, 10, stdin);
			data[3] = strtol(ch, NULL, 0);
			if(_DEBUG)
				printf("Setting data...");
			data[0] = 0xB0;		//COMMAND 
			if(_DEBUG)
				printf("%sDONE%s\n", GREEN, GRAY);
			
			I2C_Open(&fd, address);	
			I2C_Send(&fd, data, 1);
			I2C_Close(&fd);
			break;
			
		case 0:
			exit(0);
			
		default:
			break;
		
	}
	
	
	
	
	return 0;
}

