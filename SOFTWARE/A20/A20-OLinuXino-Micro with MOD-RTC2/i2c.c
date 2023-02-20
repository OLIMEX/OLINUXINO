//      i2c.c
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




unsigned char DEVICE = 2;
int _DEBUG = 0;

//
// name: I2C_Open
// @param:
//          *fd - File descriptor
//          address - address of the slave device
//
// @return: NONE
void I2C_Open(int *fd, unsigned char address)
{
    switch(DEVICE)
    {
        case 0:
        {
            *fd = open(I2C0, O_RDWR);
            if(_DEBUG)
            printf("Opening control file %s%s%s....", WHITE, I2C0, GRAY);
        }
        break;
        case 1:
        {
            *fd = open(I2C1, O_RDWR);
            if(_DEBUG)
            printf("Opening control file %s%s%s....", WHITE, I2C1, GRAY);
        }
        break;
        case 2:
        {
            *fd = open(I2C2, O_RDWR);
            if(_DEBUG)
                printf("Opening control file %s%s%s....", WHITE, I2C2, GRAY);
        }
        break;
        case 3:
        {
            *fd = open(I2C3, O_RDWR);
            if(_DEBUG)
                printf("Opening control file %s%s%s....", WHITE, I2C3, GRAY);
        }
        break;
        case 4:
        {
            *fd = open(I2C4, O_RDWR);
            if(_DEBUG)
                printf("Opening control file %s%s%s....", WHITE, I2C4, GRAY);
        }
        break;
        case 5:
        {
            *fd = open(I2C5, O_RDWR);
            if(_DEBUG)
                printf("Opening control file %s%s%s....", WHITE, I2C5, GRAY);
        }
        break;
        default:
        break;
    }

    //Check for fault
    if(*fd < 0)
    {
        if(_DEBUG)
            printf("%sFAIL%s\n",RED, GRAY);
        exit(1);
    }
    if(_DEBUG)
        printf("%sDONE%s\n",GREEN, GRAY);

    if(_DEBUG)
        printf("Opening I2C-bus...");

    if(ioctl(*fd, I2C_SLAVE_FORCE, address) < 0)
    {
        if(_DEBUG)
            printf("%sFAIL%s\n",RED, GRAY);
        exit(1);
    }
    if(_DEBUG)
        printf("%sDONE%s\n",GREEN, GRAY);

}
//
// name: I2C_Close
// @param:
//              *fd - File descriptor
//
// @return: NONE
void I2C_Close(int *fd)
{
    if(_DEBUG)
        printf ("Closing...");

    if(close(*fd) < 0)
    {
        if(_DEBUG)
            printf("%sFAIL%s\n",RED, GRAY);
        exit(1);
    }
    if(_DEBUG)
        printf("%sDONE%s\n",GREEN, GRAY);
}
//
// name: I2C_Send
// @param:
//              *fd - File descriptor
//              *buffer - array of data to be sent
//              num_bytes - number of bytes to be sent
//
// @return: NONE
void I2C_Send(int *fd, unsigned char *buffer, unsigned char num_bytes)
{
    int count = 0;
    if(_DEBUG)
        printf("Sending...");

    count = write(*fd, buffer, num_bytes);
    if(count != num_bytes)
    {
        if(_DEBUG)
        {
            printf("%sFAIL%s\n",RED, GRAY);
            if(count >= 0)
                printf("%s%d%s of %s%d%s bytes send\n", WHITE, count, GRAY, WHITE, num_bytes, GRAY);
        }
        exit(1);
    }
    if(_DEBUG){
        printf("%sDONE%s\n",GREEN, GRAY);
    }
}
//
// name: I2C_Read
// @param:
//              *fd - File descriptor
//              *buffer - array of data to be read
//              num_bytes - number of bytes to be read
//
// @return: NONE
void I2C_Read(int *fd, unsigned char *buffer, unsigned char num_bytes)
{
    int count = 0;
    if(_DEBUG)
        printf("Readinging...");

    count = read(*fd, buffer, num_bytes);
    if(count != num_bytes)
    {
        if(_DEBUG)
        {
            printf("%sFAIL%s\n",RED, GRAY);
            if(count >= 0)
                printf("%s%d%s of %s%d%s bytes read\n", WHITE, count, GRAY, WHITE, num_bytes, GRAY);
        }
        exit(1);
    }
    if(_DEBUG)
        printf("%sDONE%s\n",GREEN, GRAY);
}
