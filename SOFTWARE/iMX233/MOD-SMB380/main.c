/*
 * untitled.c
 * 
 * Copyright 2012 OLIMEX Ltd
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
#include <string.h>
#include <unistd.h>
#include "spi.h"
#include "i2c.h"

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Too few arguments. Use --help.\n");
	}
	else
	{
		if(!strcmp(argv[1], "-i2c"))
		{
			unsigned char buff[10];
			unsigned char temp[10];
			long int x, y, z;
			float r;
			float t;
			int fd;
			
			temp[0] = 0x00;
			temp[1] = 0;
				
			I2C_Open(&fd, 0x38);
			I2C_Send(&fd, temp, 1);
			I2C_Read(&fd, buff, 9);
			
			fprintf(stdout, "\033[22;33mCHIP_ID = 0x%02X\n\033[22;37m", buff[0]);
			fprintf(stdout, "\033[22;33mML_VER = 0x%02X\n\033[22;37m", buff[1] & 0x0F);
			fprintf(stdout, "\033[22;33mAL_VER = 0x%02X\n\033[22;37m", buff[1] >> 4);

			
			
			x = buff[3];
			x <<= 2;
			x |= buff[2] >> 6;
			
			if(buff[3]& 0x80)
			{
				r = ( -4.088 + (x & 0x1FF) * 0.008);		
			}
			else
			{
				r = (x & 0x1FF) * 0.008;
			}
			
			fprintf(stdout, "\033[22;36mACC_X = %.4f\n\033[22;37m", r);
			
			
			y = 0;
			y = buff[5];
			y <<= 2;
			y |= buff[4] >> 6;
			
			if(buff[5] & 0x80)
			{
				r = (-4.088 + (y & 0x1FF) * 0.008);		
			}
			else
			{
				r = (y & 0x1FF) * 0.008;
			}
			
			fprintf(stdout, "\033[22;36mACC_Y = %.4f\n\033[22;37m", r);

			z = 0;
			z = buff[7];
			z <<= 2;
			z |= buff[6] >> 6;
			
			if(buff[7] & 0x80)
			{
				r = (-4.088 + (z & 0x1FF) * 0.008);		
			}
			else
			{
				r = (z & 0x1FF) * 0.008;
			}
			
			fprintf(stdout, "\033[22;36mACC_Z = %.4f\n\033[22;37m", r);
			
			t = buff[8];
			t = t * 0.5;
			t -= 30;
			fprintf(stdout, "\033[22;35mTEMP = %.1f\n\033[22;37m", t);
		}
		else if(!strcmp(argv[1], "-spi"))
		{
			unsigned char buff[10];
			long int x, y, z;
			float r;
			float t;
			SPI_Init();
			SPI_Read(0x00, buff, 9);
			
			fprintf(stdout, "\033[22;33mCHIP_ID = 0x%02X\n\033[22;37m", buff[0]);
			fprintf(stdout, "\033[22;33mML_VER = 0x%02X\n\033[22;37m", buff[1] & 0x0F);
			fprintf(stdout, "\033[22;33mAL_VER = 0x%02X\n\033[22;37m", buff[1] >> 4);

			
			
			x = buff[3];
			x <<= 2;
			x |= buff[2] >> 6;
			
			if(buff[3]& 0x80)
			{
				r = ( -4.088 + (x & 0x1FF) * 0.008);		
			}
			else
			{
				r = (x & 0x1FF) * 0.008;
			}
			
			fprintf(stdout, "\033[22;36mACC_X = %.4f\n\033[22;37m", r);
			
			
			y = 0;
			y = buff[5];
			y <<= 2;
			y |= buff[4] >> 6;
			
			if(buff[5] & 0x80)
			{
				r = (-4.088 + (y & 0x1FF) * 0.008);		
			}
			else
			{
				r = (y & 0x1FF) * 0.008;
			}
			
			fprintf(stdout, "\033[22;36mACC_Y = %.4f\n\033[22;37m", r);

			z = 0;
			z = buff[7];
			z <<= 2;
			z |= buff[6] >> 6;
			
			if(buff[7] & 0x80)
			{
				r = (-4.088 + (z & 0x1FF) * 0.008);		
			}
			else
			{
				r = (z & 0x1FF) * 0.008;
			}
			
			fprintf(stdout, "\033[22;36mACC_Z = %.4f\n\033[22;37m", r);
			
			t = buff[8];
			t = t * 0.5;
			t -= 30;
			fprintf(stdout, "\033[22;35mTEMP = %.1f\n\033[22;37m", t);
		}
		else if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
		{
			fputs("\n\
USAGE: -[PARAMS]\n\
-spi\t\tRead data via SPI interface\n\
-i2c\t\tRead data via I2C interface\n", stdout);
		}
		else
		{
			printf("Unknown parameter\n");
		}
	}
	
	



	
	return 0;
}

