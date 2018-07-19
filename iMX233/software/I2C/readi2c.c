/*
 * Copyright © 2011 Jeroen De Schepper
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <math.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <time.h> 
#include <sys/time.h>

typedef unsigned char byte;

int deviceDescriptor;


/* Function pointer declarations */
void (*I2cSendData)(byte addr, byte *data, int len)=NULL;
void (*I2cReadData)(byte addr,byte *data,int len)=NULL;

void I2cSendData_device(byte addr,byte *data,int len)
{
	int result=0;
	if(ioctl(deviceDescriptor,I2C_SLAVE, addr))
		printf("I2cSendData_device : IOCTL Problem");
	
	result=write(deviceDescriptor,data,len);
}

void I2cReadData_device(byte addr,byte *data,int len)
{
	int result=0;
	if(ioctl(deviceDescriptor,I2C_SLAVE, addr))
		printf("I2cReadData_device : IOCTL Problem");

	result=read(deviceDescriptor,data,len);
}

void init_i2c(char *DeviceName)
{
	I2cSendData=I2cSendData_device;
	I2cReadData=I2cReadData_device;
	deviceDescriptor=open(DeviceName, O_RDWR);
	if (deviceDescriptor == -1)
	{
		printf("Error opening device '%s'\n",DeviceName);
		exit(-1);
	}
}

int main(int argc, char **argv)
{
	int i;
	byte data[2];
	init_i2c("/dev/i2c-0");

		I2cReadData(0x38,data,1);
		printf("data1 %x\n",data[0]);
		printf("data2 %x\n",data[1]);
		printf("data3 %x\n",data[2]);

	return 0;
}
