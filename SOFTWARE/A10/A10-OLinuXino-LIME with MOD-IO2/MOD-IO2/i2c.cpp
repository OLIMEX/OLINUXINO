//      i2c.c
//		14.01.2013
//      
//      Copyright 2013 OLIMEX LTD
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
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include "i2c.h"




unsigned char DEVICE = 0;

void I2C_Open(int *fd, unsigned char address)
{
	*fd = open(I2C2, O_RDWR);
	ioctl(*fd, I2C_SLAVE_FORCE, address);
}

void I2C_Close(int *fd)
{
	close(*fd);
}

int I2C_Send(int *fd, unsigned char *buffer, unsigned char num_bytes)
{
	return write(*fd, buffer, num_bytes);
}

int I2C_Read(int *fd, unsigned char *buffer, unsigned char num_bytes)
{
	return read(*fd, buffer, num_bytes);
}
