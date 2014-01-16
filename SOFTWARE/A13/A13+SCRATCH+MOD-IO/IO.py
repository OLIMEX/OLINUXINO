#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  IO.py
#  
#  Copyright 2013 OLIMEX LTD <support@olimex.com>
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


import smbus
import scratch
import datetime
import sys
import optparse




def main():
    
    usage = "usage: %prog [options] arg1 ...."
    parser = optparse.OptionParser(usage = usage)
    parser.add_option("-v", "--verbose", action="store_true",
						dest="verbose", default=False,
						help="Print extra messages")
    parser.add_option("-a", "--address", type="int", dest="address",
                        default=0x58, help="MOD-IO address")
    parser.add_option("--host", type="str", dest="host",
                        default="127.0.0.1",
                        help="Scratch host")
    parser.add_option("--port", type="int", dest="port",
                        default=42001,
                        help="Scratch port")
    parser.add_option("-i", "--i2c", type="int", dest="i2c", default=0,
						help="Select I2C-Bus, default i2c-0")					

    
    #parse options
    (options, args) = parser.parse_args()
    
    #set I2C-Bus
    I2C = parser.values.i2c
	
    #set Address
    ADDRESS = parser.values.address
    
    #check verbose mode
    DEBUG = parser.values.verbose
    
    #get HOST_IP
    HOST = parser.values.host
    
    #get HOST_PORT
    PORT = parser.values.port
    
    
    if(DEBUG):
        print("Opening I2C-bus../dev/i2c-" + str(I2C) + "..."),
    try:
        bus = smbus.SMBus(I2C)
        if(DEBUG):
            print("Done")
    except Exception:
        if(DEBUG):
            print("Fail")
        else:
            print("Failed to open I2C-bus.")
        sys.exit()
    
    
    if(DEBUG):
        print("Connecting to SCRATCH @ [" + str(HOST) + ":" + str(PORT) + "]..."),
    try:
	s = scratch.Scratch(host=str(HOST), port=PORT)
	if(DEBUG):
	    print("Done")
    except Exception:
	if(DEBUG):
	    print("Fail")
	else:
	    print("Failed to connect to SCRATCH.")
	sys.exit()
            
    def listen():
        while True:
            try:
                yield s.receive()
            except scratch.ScratchError:
                raise StopIteration

     
    a = s.receive()
    relays = a[1]
    state = relays['rel4'] << 3 | relays['rel3'] << 2 | relays['rel2'] << 1 | relays['rel1']
    #print("Initials state: " + str(state))
    bus.write_i2c_block_data(0x58, 0x10, [state])
    
    for msg in listen():            
        if msg[0] == 'sensor-update':
            if 'rel1' in msg[1]:
                if msg[1]['rel1'] == 1:
                    state = state | 0x01;
                else:
                    state = state & ~(0x01);
                    
            elif 'rel2' in msg[1]:
                if msg[1]['rel2'] == 1:
                    state = state | 0x02;
                else:
                    state = state & ~(0x02);
                    
            elif 'rel3' in msg[1]:
                if msg[1]['rel3'] == 1:
                    state = state | 0x04;
                else:
                    state = state & ~(0x04);
                    
            if 'rel4' in msg[1]:
                if msg[1]['rel4'] == 1:
                    state = state | 0x08;
                else:
                    state = state & ~(0x08);
        bus.write_i2c_block_data(ADDRESS, 0x10, [state])         
    
    return 0

	

if __name__ == '__main__':
	main()



