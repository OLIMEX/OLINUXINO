/*
 *
 * This file is part of pyA20.
 * i2c_lib.h is python I2C extension.
 *
 * Copyright (c) 2014 Stefan Mavrodiev @ OLIMEX LTD, <support@olimex.com>
 *
 * pyA20 is free software; you can redistribute it and/or modify
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

#ifndef _I2C_LIB_H
#define _I2C_LIB_H

extern int i2c_open(char *device, uint8_t address);
extern int i2c_close(int fd);
extern int i2c_send(int fd, uint8_t *buffer, uint8_t num_bytes);
extern int i2c_read(int fd, uint8_t *buffer, uint8_t num_bytes);

#endif