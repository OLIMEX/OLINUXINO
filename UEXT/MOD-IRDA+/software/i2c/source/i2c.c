/*
 * i2c.c
 * 
 * Copyright 2013 Stefan Mavrodiev <support@olimex.com>
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
 */

#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include "i2c.h"


// 
// name: I2C_Open
// @param:
//			*fd - File descriptor
//			address - address of the slave device
//	
// @return: NONE
int I2C_Open(int *fd, unsigned char bus, unsigned char address){
    char i2c[15];
    sprintf(i2c, "/dev/i2c-%d", bus);
    *fd = open(i2c, O_RDWR);	
	if(*fd < 0)
		return -1;
        
	if(ioctl(*fd, I2C_SLAVE_FORCE, address) < 0)
		return -1;
        
    return 0;
}
// 
// name: I2C_Close
// @param:
//			*fd - File descriptor
//	
// @return: NONE
int I2C_Close(int *fd){

	if(close(*fd) < 0)
		return -1;
    return 0;
}
// 
// name: I2C_Send
// @param:
//			*fd - File descriptor
//			*buffer - array of data to be sent
//			num_bytes - number of bytes to be sent 
//	
// @return: NONE
int I2C_Send(int *fd, unsigned char *buffer, unsigned char num_bytes)
{
	int count = 0;
    
	count = write(*fd, buffer, num_bytes);
	if(count != num_bytes)
        return -1;
    return 0;
}
// 
// name: I2C_Read
// @param:
//			*fd - File descriptor
//			*buffer - array of data to be read
//			num_bytes - number of bytes to be read 
//	
// @return: NONE
int I2C_Read(int *fd, unsigned char *buffer, unsigned char num_bytes){
	
	int count = 0;

	count = read(*fd, buffer, num_bytes);
	if(count != num_bytes)
        return -1;
    return 0;
}
