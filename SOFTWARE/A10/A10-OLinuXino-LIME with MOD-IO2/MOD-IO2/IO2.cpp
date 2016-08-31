/*
 * IO2.cpp
 * 
 * Copyright 2013 OLIMEX LTD/Stefan Mavrodiev <support@olimex.com>
 * modified for A20-OLinuXino-Micro by Stanimir Petev
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
 
#include "IO2.h"
#include "i2c.h"


/*
 * Class variables
 */
 uint8_t IO2::address = 0x21;
 uint8_t IO2::relay_status = 0x00;
 uint8_t IO2::tris_status = 0x00;
 uint8_t IO2::lat_status = 0x00;
 uint8_t IO2::pp_status = 0x00;
 
  uint8_t IO2::IN = 1;
  uint8_t IO2::OUT = 0;
  uint8_t IO2::LO = 0;
  uint8_t IO2::HI = 1;
  uint8_t IO2::ON = 1;
  uint8_t IO2::OFF = 0;
	
  uint8_t IO2::RELAY1 = 0x01;
  uint8_t IO2::RELAY2 = 0x02;
	
  uint8_t IO2::GPIO0 = 0x01;
  uint8_t IO2::GPIO1 = 0x02;
  uint8_t IO2::GPIO2 = 0x04;
  uint8_t IO2::GPIO3 = 0x08;
  uint8_t IO2::GPIO4 = 0x10;
  uint8_t IO2::GPIO5 = 0x20;
  uint8_t IO2::GPIO6 = 0x40;
  
  uint8_t IO2::AN0 = 0x10;
  uint8_t IO2::AN1 = 0x11;
  uint8_t IO2::AN2 = 0x12;
  uint8_t IO2::AN3 = 0x13;
  uint8_t IO2::AN5 = 0x15;


/*
 * Constructors
 */
IO2::IO2()
{
}

IO2::IO2(uint8_t addr)
{
    address = addr;
}

void IO2::setRelay(uint8_t relay, uint8_t state)
{
	if(state)
		relay_status |= relay;
	else
		relay_status &= ~relay;
  
	int fd;
	unsigned char buff[2] = {SET_RELAY, relay_status};

	I2C_Open(&fd, address);
	I2C_Send(&fd, buff, 2);
	I2C_Close(&fd);
}

void IO2::setAddress(uint8_t newAddress)
{
	int fd;
	unsigned char buff[2] = {SET_ADDRESS, newAddress};

	I2C_Open(&fd, address);
	I2C_Send(&fd, buff, 2);
	I2C_Close(&fd);
}

uint16_t IO2::analogRead(uint8_t pin)
{

	int fd;
	uint16_t result;
	unsigned char buff[1] = {pin}, data[2];

	I2C_Open(&fd, address);
	I2C_Send(&fd, buff, 1);
	I2C_Close(&fd);

	I2C_Open(&fd, address);
	I2C_Read(&fd, data, 2);
	I2C_Close(&fd);
  
	result = (data[1] << 8) | data[0];
	return result;
}

void IO2::pinMode(uint8_t pin, uint8_t mode)
{
	if(mode)
		tris_status |= pin;
	else
		tris_status &= ~pin;

	int fd;
	unsigned char buff[2] = {SET_TRIS, tris_status};

	I2C_Open(&fd, address);
	I2C_Send(&fd, buff, 2);
	I2C_Close(&fd);
}

void IO2::digitalWrite(uint8_t pin, uint8_t level)
{
	if(level)
		lat_status |= pin;
	else
		lat_status &= ~pin;

	int fd;
	unsigned char buff[2] = {SET_LAT, lat_status};

	I2C_Open(&fd, address);
	I2C_Send(&fd, buff, 2);
	I2C_Close(&fd);
}

void IO2::setPullpin(uint8_t pin, uint8_t state)
{
	if(state)
		pp_status |= pin;
	else
		pp_status &= ~pin;

	int fd;
	unsigned char buff[2] = {SET_PU, pp_status};

	I2C_Open(&fd, address);
	I2C_Send(&fd, buff, 2);
	I2C_Close(&fd);
}

uint8_t IO2::digitalRead(uint8_t pin)
{

	int fd;
	unsigned char buff[1] = {GET_PORT}, data, result;

	I2C_Open(&fd, address);
	I2C_Send(&fd, buff, 1);
	I2C_Close(&fd);

	I2C_Open(&fd, address);
	I2C_Read(&fd, &data, 1);
	I2C_Close(&fd);
	return !!(data & pin);
}

uint8_t IO2::readID()
{
	int fd;
	unsigned char buff[1] = {GET_ID}, id, result;

	I2C_Open(&fd, address);
	I2C_Send(&fd, buff, 1);
	I2C_Close(&fd);

	I2C_Open(&fd, address);
	I2C_Read(&fd, &id, 1);
	I2C_Close(&fd);
	return id;
}
