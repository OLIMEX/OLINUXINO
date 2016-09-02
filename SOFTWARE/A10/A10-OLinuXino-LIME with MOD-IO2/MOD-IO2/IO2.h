/*
 * IO2.h
 * 
 * Copyright 2013 OLIMEX LTD/Stefan Mavrodiev <support@olimex.com>
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

#ifndef IO2_H
#define IO2_H

typedef	unsigned char uint8_t;
typedef	unsigned int uint16_t;

#define SET_RELAY   	0x40
#define SET_ADDRESS 	0xF0

#define SET_TRIS	0x01
#define SET_LAT		0x02
#define GET_PORT	0x03
#define SET_PU		0x04
#define GET_TRIS	0x05
#define GET_LAT		0x06

#define GET_ID		0x20


class IO2
{
	private:
		static uint8_t address;
		static uint8_t relay_status;
		static uint8_t tris_status;
		static uint8_t lat_status;
		static uint8_t pp_status;
	public:
		IO2();
		IO2(uint8_t);
		void setRelay(uint8_t relay, uint8_t state);
		void setAddress(uint8_t newAddress);	
		void pinMode(uint8_t pin, uint8_t mode);
		void digitalWrite(uint8_t pin, uint8_t level);
		uint8_t digitalRead(uint8_t pin);
		uint16_t analogRead(uint8_t pin);
		void setPullpin(uint8_t pin, uint8_t state);
		uint8_t readID();	
		
		/* Constants */
		static uint8_t IN;
		static uint8_t OUT;
		static uint8_t LO;
		static uint8_t HI;
		static uint8_t ON;
		static uint8_t OFF;	
		static uint8_t RELAY1;
		static uint8_t RELAY2;	
		static uint8_t GPIO0;
		static uint8_t GPIO1;
		static uint8_t GPIO2;
		static uint8_t GPIO3;
		static uint8_t GPIO4;
		static uint8_t GPIO5;
		static uint8_t GPIO6;	
		static uint8_t AN0;
		static uint8_t AN1;
		static uint8_t AN2;
		static uint8_t AN3;
		static uint8_t AN5;

};
#endif
