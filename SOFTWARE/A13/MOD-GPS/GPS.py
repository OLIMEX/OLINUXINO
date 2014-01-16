#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#       GPS.py
#       
#       Copyright 2013 Stefan Mavrodiev <support@olimex.com>
#       
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the License, or
#       (at your option) any later version.
#       
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#       
#       You should have received a copy of the GNU General Public License
#       along with this program; if not, write to the Free Software
#       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#       MA 02110-1301, USA.
        

import serial
import sys
import curses
import time




def main():
    
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()
        
    #add windows
    win1 = curses.newwin(9, 40, 4, 1)
    win1.border('|','|')
    win2 = curses.newwin(3, 40, 1, 1)
    win2.border('|','|')
    win3 = curses.newwin(3, 40, 13, 1)
    win3.border(0)
    
    win1.addstr(1, 3, "Date: ", curses.A_BOLD)
    win1.addstr(2, 3, "Latitude:", curses.A_BOLD)
    win1.addstr(3, 3, "Longitude:", curses.A_BOLD)
    win1.addstr(4, 3, "Altitude:", curses.A_BOLD)
    win1.addstr(5, 3, "Speed:", curses.A_BOLD)
    win1.addstr(6, 3, "Heading:", curses.A_BOLD)
    win1.addstr(7, 3, "Status:", curses.A_BOLD)
    
    win3.addstr(1, 14, "^C - EXIT", curses.A_BOLD)
    win2.addstr(1, 4, "RPi + RPi-UEXT + MOD-GPS DEMO", curses.A_BOLD)
    
    win1.refresh()
    win2.refresh()
    win3.refresh()
       
    while True:
        try:
            
            ser = serial.Serial('/dev/ttyAMA0', 19200, timeout = None)            
            
            #read the first line of data
            while True:
                line = ser.readline()
                list_s = line.split(',')
                
                if "$GPRMC" == list_s[0]:
                    win1.addstr(1, 18, list_s[9][0:2] + "." + list_s[9][2:4] + "." + list_s[9][4:6])
                    
                elif "$GPVTG" == list_s[0]:
                    win1.addstr(5, 18, list_s[7])
                    win1.addstr(6, 18, list_s[1])
                    
                elif "$GPGSA" == list_s[0]:
                    if list_s[2] == '1':
                        win1.addstr(7, 18, "NO FIX")
                    elif list_s[2] == '2':
                        win1.addstr(7, 18, "2D FIX")
                    elif list_s[2] == '3':
                        win1.addstr(7, 18, "3D FIX")
                
                elif "$GPGGA" == list_s[0]:
                    win1.addstr(2, 18, str(float(list_s[2])/100.0) + " " + list_s[3])
                    win1.addstr(3, 18, str(float(list_s[4])/100.0) + " " + list_s[5])
                    win1.addstr(4, 18, list_s[9])               
                
                win1.refresh()
                win2.refresh()
                win3.refresh()
                
        except:
            break       
    
    curses.nocbreak()
    curses.echo()
    curses.endwin()
    return
                    

if __name__ == '__main__':
	main()

