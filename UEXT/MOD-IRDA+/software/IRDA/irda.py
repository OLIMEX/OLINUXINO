#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  irda.py
#  
#  Copyright 2014 Stefan Mavrodiev <support@olimex.com>
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.


import I2C


class IRDA:

    # Private properties
    __SET_MODE = 0x01
    __SEND_DATA = 0x02
    __READ_DATA = 0x03
    __GET_ID = 0x20
    __CHIP_ID = 0x54
    
    # Public properties
    MODE_SIRC = 0x01
    MODE_RC5 = 0x00
    
    def __init__(self, i2c=2, address=0x24):

        self.i2c = i2c
        self.address = address 
        
        I2C.init(self.i2c)
        return
        
    def checkID(self, address=None):
        if address is None:
            address = self.address
            
        try:
            I2C.open(address)    
            I2C.write([self.__GET_ID])
            read_id = I2C.read(1)[0]
            I2C.close()
            if read_id == self.__CHIP_ID:
                # Chip_ID match
                return True
            else:
                # Chip_ID doesn't match
                return False
                
        except Exception, e:
            print e
            return False
            
    def setOperationMode(self, mode):
        try:
            I2C.open(self.address)
            I2C.write([self.__SET_MODE, mode])
            I2C.close()
            return True
            
        except Exception, e:
            print e
            return False
            
    def sendData(self, device, command):
        try:
            I2C.open(self.address)
            I2C.write([self.__SEND_DATA, device, command])
            I2C.close()
            return True
        
        except Exception, e:
            print e
            return False
            
    def readData(self):
        try:
            I2C.open(self.address)
            I2C.write([self.__READ_DATA])
            data = I2C.read(2)
            I2C.close()
            return data[0] << 8 | data[1]
            
        except Exception:
            return 0xFFFF

    @staticmethod
    def decode_SIRC(raw):
        device = IRDA.reverse(raw & 0x1F, 5)
        command = IRDA.reverse((raw >> 5) & 0x7F, 7)
        
        return command, device

    @staticmethod
    def reverse(data, length):
        reverse = 0
        for i in range(length):
            reverse <<= 1
            if data & (1 << i):
                reverse |= 1
            else:
                reverse |= 0
        return reverse
        
    @staticmethod
    def decode_RC5(raw):
        command = raw & 0x3F
        device = (raw >> 6) & 0x1F
        toggle = (raw >> 11) & 0x01
        
        return command, device
