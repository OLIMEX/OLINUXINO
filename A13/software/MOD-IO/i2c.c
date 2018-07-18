/*
 * i2c.c
 * 
 * Copyright 2012  <stefan-olimex@olimexserver11-desktop>
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
 
 
/*
 * 
 * name: I2C_Open
 * @param
 * @return
 * 
 */ 
 void I2C_Open(int *file, unsigned char address)
 {
	*file = (int)open("/dev/i2c-2", O_RDWR); 
	if(*file < 0)
	{
		perror("Failed to open I2C");
		exit(1);
	}
	else
	{
		if(ioctl(*file, I2C_SLAVE, address) < 0)
		{
			perror("Failed to access I2C bus");
			exit(1);
		}								
	}
 }
/*
 * 
 * name: I2C_Close
 * @param
 * @return
 * 
 */
 void I2C_Close(int *file)
 {
	 close(*file);
 }
/*
 * 
 * name: I2C_Send
 * @param
 * @return
 * 
 */
 void I2C_Send(int *file, char *buffer, int num)
 { 
	int bytes;
	bytes = write(*file, buffer, num);
	if(bytes != num)
	{
		perror("Failed to send data");
		exit(1);
	} 
 }
/*
 * 
 * name: I2C_Read
 * @param
 * @return
 * 
 */
 void I2C_Read(int *file, unsigned char *buffer, int num)
 { 
	int bytes;
	bytes = read(*file, buffer, num);
	if(bytes != num)
	{
		perror("Failed to read data");
		exit(1);
	} 
 }
 
 
 
 
