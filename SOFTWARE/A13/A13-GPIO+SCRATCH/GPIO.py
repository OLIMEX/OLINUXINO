#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  IO.py
#  
#  Copyright 2013 Stefan Mavrodiev <support@olimex.com>
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


import scratch
import sys
import optparse
import time
import A13_GPIO as gpio




def main():
        
    usage = "usage: %prog [options] arg1 ...."
    parser = optparse.OptionParser(usage = usage)
    parser.add_option("-v", "--verbose", action="store_true",
                        dest="verbose", default=False,
                        help="Print extra messages")
    parser.add_option("--host", type="str", dest="host",
                        default="127.0.0.1",
                        help="Scratch host")
    parser.add_option("--port", type="int", dest="port",
                        default=42001,
                        help="Scratch port")
    
    #parse options
    (options, args) = parser.parse_args()
    
    #check verbose mode
    DEBUG = parser.values.verbose
    
    #get HOST_IP
    HOST = parser.values.host
    
    #get HOST_PORT
    PORT = parser.values.port
    
    if(DEBUG):
        print("Initializating GPIOs..."),
    try:
        gpio.init()
        
        #Configure inputs
        gpio.setcfg(gpio.PIN9, gpio.INP)    #input 1
        gpio.setcfg(gpio.PIN8, gpio.INP)    #input 2
        gpio.setcfg(gpio.PIN7, gpio.INP)    #input 3
        gpio.setcfg(gpio.PIN6, gpio.INP)    #input 4
        
        #Configure outputs
        gpio.setcfg(gpio.PIN29, gpio.OUT)   #output 1
        gpio.setcfg(gpio.PIN30, gpio.OUT)   #output 2
        gpio.setcfg(gpio.PIN31, gpio.OUT)   #output 3
        gpio.setcfg(gpio.PIN32, gpio.OUT)   #output 4
        gpio.setcfg(gpio.PIN33, gpio.OUT)   #output 5
        gpio.setcfg(gpio.PIN34, gpio.OUT)   #output 6
        gpio.setcfg(gpio.PIN35, gpio.OUT)   #output 7
        gpio.setcfg(gpio.PIN36, gpio.OUT)   #output 8
        gpio.setcfg(gpio.PIN37, gpio.OUT)   #output 9
        gpio.setcfg(gpio.PIN38, gpio.OUT)   #output 10
        gpio.setcfg(gpio.PIN39, gpio.OUT)   #output 11
        
        #Set all outputs LOW
        gpio.output(gpio.PIN29, gpio.LOW)
        gpio.output(gpio.PIN30, gpio.LOW)
        gpio.output(gpio.PIN31, gpio.LOW)
        gpio.output(gpio.PIN32, gpio.LOW)
        gpio.output(gpio.PIN33, gpio.LOW)
        gpio.output(gpio.PIN34, gpio.LOW)
        gpio.output(gpio.PIN35, gpio.LOW)
        gpio.output(gpio.PIN36, gpio.LOW)
        gpio.output(gpio.PIN37, gpio.LOW)
        gpio.output(gpio.PIN38, gpio.LOW)
        gpio.output(gpio.PIN39, gpio.LOW)
        
        if(DEBUG):
            print("Done")
    except Exception:
        if(DEBUG):
            print("Fail")
        else:
            print("Failed to initialize ports")
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
        
        
                    
    for msg in listen(s):
        if(DEBUG):
            print("New message received: " + str(msg))
        if msg[0] == 'broadcast':
            if msg[1] == 'INPUT1':
                i = gpio.input(gpio.PIN9)
                s.sensorupdate({'INPUT1': i});
            elif msg[1] == 'INPUT2':
                i = gpio.input(gpio.PIN8)
                s.sensorupdate({'INPUT2': i});
            elif msg[1] == 'INPUT3':
                i = gpio.input(gpio.PIN7)
                s.sensorupdate({'INPUT3': i});
            elif msg[1] == 'INPUT4':
                i = gpio.input(gpio.PIN6)
                s.sensorupdate({'INPUT4': i});
        
        
        #Handle outputs
        elif msg[0] == 'sensor-update':
            
            g = msg[1]
            keys = g.viewkeys()
            for key in keys:
       #         if(DEBUG):
       #            print("Key: " + str(key) + "\tValue: " + str(g[key]))
                    
                #Catch message for OUTPUT1
                if key == 'OUTPUT1':
                    pin = gpio.PIN29
                elif key == 'OUTPUT2':
                    pin = gpio.PIN30
                elif key == 'OUTPUT3':
                    pin = gpio.PIN31
                elif key == 'OUTPUT4':
                    pin = gpio.PIN32
                elif key == 'OUTPUT5':
                    pin = gpio.PIN33
                elif key == 'OUTPUT6':
                    pin = gpio.PIN34
                elif key == 'OUTPUT7':
                    pin = gpio.PIN35
                elif key == 'OUTPUT8':
                    pin = gpio.PIN36
                elif key == 'OUTPUT9':
                    pin = gpio.PIN37
                elif key == 'OUTPUT10':
                    pin = gpio.PIN38
                elif key == 'OUTPUT11':
                    pin = gpio.PIN39
                    
                #toggle GPIO
                if g[key] == 1:
                    if(DEBUG):
                        print(str(key) + ": HIGH..."),
                    try:
                        gpio.output(pin, gpio.HIGH)
                        if(DEBUG):
                            print("Done");
                    except Exception:
                        if(DEBUG):
                            print("Fail")
                        else:
                            print("Failed to set " + str(key))
                elif g[key] == 0:
                    if(DEBUG):
                        print(str(key) + ": LOW..."),
                    try:
                        gpio.output(pin, gpio.LOW)
                        if(DEBUG):
                            print("Done");
                    except Exception:
                        if(DEBUG):
                            print("Fail")
                        else:
                            print("Failed to clear " + str(key))
                else:
                        print(str(key) + ": Invalid output state!")
   
    
             
    
    return 0

    
    
def listen(s):
    while True:
        try:
            yield s.receive()
        except scratch.ScratchError:
            raise StopIteration


if __name__ == '__main__':
        main()
        



