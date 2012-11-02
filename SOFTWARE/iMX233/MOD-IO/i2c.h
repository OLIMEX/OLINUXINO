/*
 * i2c.h
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
 
 #ifndef I2C_H
 #define I2C_H
 
 void I2C_Open(int *file, unsigned char address);
 void I2C_Send(int *file, unsigned char *buffer, int num);
 void I2C_Read(int *file, unsigned char *buffer, int num);
 void I2C_Close(int *file);
 
 #endif
