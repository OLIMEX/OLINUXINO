/*
 * main.c
 * 
 * Copyright 2012  OLIMEX Ltd
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include "i2c.h"

int main(int argc, char **argv)
{
	int file;
	unsigned char buffer[2], data[2], address;
	
	if(argc < 3)
	{
		printf("Too few arguments.\n Type -help.\n");
		exit(1);
	}
	
	if(!strcmp(argv[1], "-relay"))
	{
		address = strtol(argv[2], NULL, 0);
		buffer[0] = 0x10;
		buffer[1] = strtol(argv[3], NULL, 0);
		I2C_Open(&file, address);
		I2C_Send(&file, buffer, 2);
		I2C_Close(&file);
				
	}
	
	else if(!strcmp(argv[1], "-dig"))
	{
		int i;
		address = strtol(argv[2], NULL, 0);
		buffer[0] = 0x20;
		I2C_Open(&file, address);
		I2C_Send(&file, buffer, 1);
		I2C_Read(&file, data, 1);
		I2C_Close(&file);
		
		for(i = 0; i < 4; i++)
		{
			printf("IN[%d] = %d\n", i+1, (data[0] >> i) & 0x01);
		}
				
	}
	else if(!strcmp(argv[1], "-an1"))
	{
		float vcc;
		int i, temp = 0;
		address = strtol(argv[2], NULL, 0);

		buffer[0] = 0x30;
		I2C_Open(&file, address);
		I2C_Send(&file, buffer, 1);
		I2C_Read(&file, data, 2);
		I2C_Close(&file);
		
		for(i = 0; i < 8; i++)
		{
			temp |= ((data[0] & 0x80) ? 1 : 0) << i;
			data[0] <<= 1;
			 
		}
		temp |= ((data[1] & 0x02) ? 1 : 0) << 8;
		temp |= ((data[1] & 0x01) ? 1 : 0) << 9;
		
		vcc = (3.3*temp)/1023;
		printf("AN1: %.3fV\n", vcc);		
		
	}
	else if(!strcmp(argv[1], "-an2"))
	{
		float vcc;
		int i, temp = 0;
		address = strtol(argv[2], NULL, 0);

		buffer[0] = 0x31;
		I2C_Open(&file, address);
		I2C_Send(&file, buffer, 1);
		I2C_Read(&file, data, 2);
		I2C_Close(&file);
		
		for(i = 0; i < 8; i++)
		{
			temp |= ((data[0] & 0x80) ? 1 : 0) << i;
			data[0] <<= 1;
			 
		}
		temp |= ((data[1] & 0x02) ? 1 : 0) << 8;
		temp |= ((data[1] & 0x01) ? 1 : 0) << 9;
		
		vcc = (3.3*temp)/1023;
		printf("AN2: %.3fV\n", vcc);		
		
	}
	else if(!strcmp(argv[1], "-an3"))
	{
		float vcc;
		int i, temp = 0;
		address = strtol(argv[2], NULL, 0);

		buffer[0] = 0x32;
		I2C_Open(&file, address);
		I2C_Send(&file, buffer, 1);
		I2C_Read(&file, data, 2);
		I2C_Close(&file);
		
		for(i = 0; i < 8; i++)
		{
			temp |= ((data[0] & 0x80) ? 1 : 0) << i;
			data[0] <<= 1;
			 
		}
		temp |= ((data[1] & 0x02) ? 1 : 0) << 8;
		temp |= ((data[1] & 0x01) ? 1 : 0) << 9;
		
		vcc = (3.3*temp)/1023;
		printf("AN3: %.3fV\n", vcc);		
		
	}
	else if(!strcmp(argv[1], "-an4"))
	{
		float vcc;
		int i, temp = 0;
		address = strtol(argv[2], NULL, 0);
		buffer[0] = 0x33;
		I2C_Open(&file, address);
		I2C_Send(&file, buffer, 1);
		I2C_Read(&file, data, 2);
		I2C_Close(&file);
		
		for(i = 0; i < 8; i++)
		{
			temp |= ((data[0] & 0x80) ? 1 : 0) << i;
			data[0] <<= 1;
			 
		}
		temp |= ((data[1] & 0x02) ? 1 : 0) << 8;
		temp |= ((data[1] & 0x01) ? 1 : 0) << 9;
		
		vcc = (3.3*temp)/1023;
		printf("AN4: %.3fV\n", vcc);		
		
	}
	else if(!strcmp(argv[1], "-address"))
	{
		buffer[0] = 0xF0;
		address = strtol(argv[2], NULL, 0);
		buffer[1] = strtol(argv[3], NULL, 0);
		I2C_Open(&file, address);
		I2C_Send(&file, buffer, 2);
		I2C_Close(&file);		
	}
	else
	{
		printf("USAGE: -command [address] [data]\n");
		printf("\n\r");
		printf("-relay\t-Toggle relays\n");
		printf("-dig\t-Get digital inputs\n");
		printf("-an1\t-Get analog 1\n");
		printf("-an2\t-Get analog 2\n");
		printf("-an3\t-Get analog 3\n");
		printf("-an4\t-Get analog 4\n");
		printf("-address\t-Set new address\n");
		
		
	}

	
	
	return 0;
}

