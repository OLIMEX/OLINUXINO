//      i2c.h
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


#ifndef I2C_H
#define I2C_H


#define I2C0	"/dev/i2c-0"
#define I2C1	"/dev/i2c-1"
#define I2C2	"/dev/i2c-2"
#define I2C3	"/dev/i2c-3"
#define I2C4	"/dev/i2c-4"
#define I2C5	"/dev/i2c-5"

#define RED		"\033[22;31m"
#define GREEN	"\033[22;32m"
#define GRAY	"\033[22;37m"
#define WHITE	"\033[01;37m"

extern void I2C_Open(int *f, unsigned char address);
extern void I2C_Send(int *f, unsigned char *buffer, unsigned char num_bytes);
extern void I2C_Read(int *f, unsigned char *buffer, unsigned char num_bytes);
extern void I2C_Close(int *f);

extern unsigned char DEVICE;
extern int _DEBUG;

#endif

