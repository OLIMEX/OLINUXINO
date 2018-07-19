/*
 * untitled.c
 * 
 * Copyright 2012 Stefan Mavrodiev <OLIMEX LTD>
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
	/*Check for number of parameters. Should be at least 2*/
	if(argc < 2)
	{
		printf("Invalid number of parameters\n");
		exit(1);
	}
	else
	{
		if(!strcmp(argv[1], "--clc") || !strcmp(argv[1], "-c"))
		{
			/*Check parameters*/
			if(argc != 4)
			{
				printf("Invalid number of parameters\n");
				exit(1);
			}
			else
			{
				int fd;
				int address;
				int data;
				
				unsigned char buffer[5];
				
				address =	strtol(argv[2], NULL, 0);
				data 	=	strtol(argv[3], NULL, 0);
				
				buffer[0] = 0x04;
				buffer[1] = address;
				buffer[2] = 0x30;
				buffer[3] = data;
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, buffer, 4);
				/*CLOSE I2C-BUS*/
				I2C_Close(&fd);
				
				printf("Write\t: ");
				if(data & 0x01)
					printf("Open\n");
				else
					printf("Close\n");
					
				printf("Read\t: ");
				if(data & 0x02)
					printf("Open\n");
				else
					printf("Close\n");			
				  
			}
		}
		else if(!strcmp(argv[1], "--address") || !strcmp(argv[1], "-a"))
		{
			int fd;
				int address;
				
				unsigned char buffer[5];
				
				address =	strtol(argv[2], NULL, 0);
				
				buffer[0] = 0x04;
				buffer[1] = 0xF0;
				buffer[2] = 0xB0;
				buffer[3] = address;
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, buffer, 4);
				/*CLOSE I2C-BUS*/
				I2C_Close(&fd);
				
				printf("Done\n");
		}
	}
	return 0;
}

