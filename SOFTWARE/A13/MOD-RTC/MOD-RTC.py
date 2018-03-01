#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#       MOD_RTC.py
#       
#       Copyright 2013 OLIMEX LTD
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


#import libraries
import optparse
import smbus
from termcolor import colored
import sys
import os
import datetime
import time

#declare global variable
I2C_Bus = 0
DEBUG = 0

#function definitions
def BCDtoInt(bcd):
	"BDC -> INT"
	a = bcd & 0x0F
	b = bcd >> 4
	return b*10 + a
def InttoBCD(Int):
	"INT -> BDC"
	a = Int % 10;
	b = Int / 10;	
	return (b << 4) + a;

def is_leap_year(year):
    return year % 4 == 0 and (year % 100 != 0 or year % 400 == 0)

def PrintF(string, color):	
	try:
		print colored(string, color)
	except:
		print string
	return 

def Read_RTC(option, opt, value, parser):
	"Read the date from MOD-RTC"
	
	#set I2C-Bus
	global I2C_Bus
	I2C_Bus = parser.values.i2c
	
	#check verbose mode
	global DEBUG
	DEBUG = parser.values.verbose
	
	#Init I2C-Bus
	if DEBUG:
		print "Opening /dev/i2c-%d..." %(I2C_Bus),
	try:		
		bus = smbus.SMBus(I2C_Bus)
		if DEBUG:
			PrintF("Done", "green")
	except:
		if DEBUG:
			PrintF("Fail", "red")
		sys.exit(0)
	
	#Read data
	if DEBUG:
		print "Reading data...",
	try:	
		buf = bus.read_i2c_block_data(0x51, 0x02, 7)
		if DEBUG:
			PrintF("Done", "green")
	except:
		if DEBUG:
			PrintF("Fail", "red")
		sys.exit(0)
		
	
	#Process data
	if DEBUG:
		print "Processing data...",		
	try:	
		buf[0] &= 0x7F;
		buf[1] &= 0x7F;
		buf[2] &= 0x3F;
		buf[3] &= 0x3F;
		buf[4] &= 0x07;
		buf[5] &= 0x1F;
		buf[6] &= 0xFF;
		if DEBUG:
			PrintF("Done", "green")
	except:
		if DEBUG:
			PrintF("Fail", "red")
		sys.exit(0)
	
	if buf[4] == 0:
		wday = "Sun"
	elif buf[4] == 1:
		wday = "Mon"
	elif buf[4] == 2:
		wday = "Tue"
	elif buf[4] == 3:
		wday = "Wed"
	elif buf[4] == 4:
		wday = "Thu"
	elif buf[4] == 5:
		wday = "Fri"
	else:
		wday = "Sat"
		
	if buf[5] == 1:
		mon = "Jan"
	elif buf[5] == 2:
		mon = "Feb"
	elif buf[5] == 3:
		mon = "Mar"
	elif buf[5] == 4:
		mon = "Apr"
	elif buf[5] == 5:
		mon = "May"
	elif buf[5] == 6:
		mon = "Jun"
	elif buf[5] == 7:
		mon = "Jul"
	elif buf[5] == 8:
		mon = "Aug"
	elif buf[5] == 9:
		mon = "Sep"
	elif buf[5] == 10:
		mon = "Oct"
	elif buf[5] == 11:
		mon = "Nov"
	else:
		mon = "Dec"	
	if is_leap_year(BCDtoInt(buf[6])+1900) == False and BCDtoInt(buf[5]) == 2 and BCDtoInt(buf[5])==29:
        set_new_date = BCDtoInt(1)
        mon = "Mar"
        os.system("date -s \"%s %s %d %d:%d:%d %d\"" % (wday, mon, set_new_date,
									BCDtoInt(buf[2]), BCDtoInt(buf[1]),
									BCDtoInt(buf[0]), BCDtoInt(buf[6])+1900)
        Sync_RTC(option, opt, value, '-s')
    else:
        set_new_date = BCDtoInt(buf[3])

	
	#printing the date	
	print "%s %s %d %d:%d:%d %d" % (wday, mon, is_leap_year,
									BCDtoInt(buf[2]), BCDtoInt(buf[1]),
									BCDtoInt(buf[0]), BCDtoInt(buf[6])+1900)
		
	return "%s %s %d %d:%d:%d %d" % (wday, mon, is_leap_year,
									BCDtoInt(buf[2]), BCDtoInt(buf[1]),
									BCDtoInt(buf[0]), BCDtoInt(buf[6])+1900)
		
def Write_RTC(option, opt, value, parser):
	"Write system clock to MOD-RTC"
	
	#set I2C-Bus
	global I2C_Bus
	I2C_Bus = parser.values.i2c
	
	#check verbose mode
	global DEBUG
	DEBUG = parser.values.verbose
	
	#Init I2C-Bus
	if DEBUG:
		print "Opening /dev/i2c-%d..." %(I2C_Bus),
	try:		
		bus = smbus.SMBus(I2C_Bus)
		if DEBUG:
			PrintF("Done", "green")
	except:
		if DEBUG:
			PrintF("Fail", "red")
		sys.exit(0)
	
	if DEBUG:
		print "Reading system clock...",
	try:
		a = time.gmtime()
		if DEBUG:
			PrintF("Done", "green")
	except:
		if DEBUG:
			PrintF("Fail", "red")
		sys.exit(0)
		
	
	if DEBUG:
		print "Preparing data...",
	
	try:
		buf = []
		buf.append(InttoBCD(a.tm_sec))
		buf.append(InttoBCD(a.tm_min))
		buf.append(InttoBCD(a.tm_hour))	
		buf.append(InttoBCD(a.tm_mday))
		if a.tm_wday + 1 == 7:
			buf.append(0)
		else:
			buf.append(InttoBCD(a.tm_wday + 1))	
		buf.append(InttoBCD(a.tm_mon))
		buf.append(InttoBCD(a.tm_year-1900))
		
		PrintF("Done", "green")
		
	except:
		PrintF("Fail", "red")
		sys.exit(0)	
		
		
	if DEBUG:
		print "Sending data...",	
	try:
		bus.write_i2c_block_data(0x51, 0x02, buf)
		PrintF("Done", "green")
	except:
		PrintF("Red", "red")
		
	return	

def Sync_RTC(option, opt, value, parser):
	"Sync system clock with MOD-RTC"
	
	#set I2C-Bus
	global I2C_Bus
	I2C_Bus = parser.values.i2c
	
	#check verbose mode
	global DEBUG
	DEBUG = parser.values.verbose
	
	#Init I2C-Bus
	if DEBUG:
		print "Opening /dev/i2c-%d..." %(I2C_Bus),
	try:		
		bus = smbus.SMBus(I2C_Bus)
		if DEBUG:
			PrintF("Done", "green")
	except:
		if DEBUG:
			PrintF("Fail", "red")
		sys.exit(0)
	
	#Read data
	if DEBUG:
		print "Reading data...",
	try:	
		buf = bus.read_i2c_block_data(0x51, 0x02, 7)
		if DEBUG:
			PrintF("Done", "green")
	except:
		if DEBUG:
			PrintF("Fail", "red")
		sys.exit(0)
		
	
	#Process data
	if DEBUG:
		print "Processing data...",		
	try:	
		buf[0] &= 0x7F;
		buf[1] &= 0x7F;
		buf[2] &= 0x3F;
		buf[3] &= 0x3F;
		buf[4] &= 0x07;
		buf[5] &= 0x1F;
		buf[6] &= 0xFF;
		if DEBUG:
			PrintF("Done", "green")
	except:
		if DEBUG:
			PrintF("Fail", "red")
		sys.exit(0)
	
	if buf[4] == 0:
		wday = "Sun"
	elif buf[4] == 1:
		wday = "Mon"
	elif buf[4] == 2:
		wday = "Tue"
	elif buf[4] == 3:
		wday = "Wed"
	elif buf[4] == 4:
		wday = "Thu"
	elif buf[4] == 5:
		wday = "Fri"
	else:
		wday = "Sat"
		
	if buf[5] == 1:
		mon = "Jan"
	elif buf[5] == 2:
		mon = "Feb"
	elif buf[5] == 3:
		mon = "Mar"
	elif buf[5] == 4:
		mon = "Apr"
	elif buf[5] == 5:
		mon = "May"
	elif buf[5] == 6:
		mon = "Jun"
	elif buf[5] == 7:
		mon = "Jul"
	elif buf[5] == 8:
		mon = "Aug"
	elif buf[5] == 9:
		mon = "Sep"
	elif buf[5] == 10:
		mon = "Oct"
	elif buf[5] == 11:
		mon = "Nov"
	else:
		mon = "Dec"	
	
	if is_leap_year(BCDtoInt(buf[6])+1900) == False and BCDtoInt(buf[5]) == 2 and BCDtoInt(buf[3]) == 29:
            set_new_date = BCDtoInt(1)
            mon = "Mar"
    else:
        set_new_date = BCDtoInt(buf[3])
	#printing the date	
	os.system("date -s \"%s %s %d %d:%d:%d %d\"" % (wday, mon, set_new_date,
									BCDtoInt(buf[2]), BCDtoInt(buf[1]),
									BCDtoInt(buf[0]), BCDtoInt(buf[6])+1900))
	if is_leap_year(BCDtoInt(buf[6])+1900) == False and BCDtoInt(buf[5]) == 2:
            Write_RTC(option, opt, value, '-w')
									
									
	return

def main():
	
	#get options from command line
	usage = "usage: %prog [options] arg1 ...."
	parser = optparse.OptionParser(usage = usage)
	parser.add_option("-v", "--verbose", action="store_true",
						dest="verbose", default=False,
						help="Print extra messages")					
	parser.add_option("-i", "--i2c", type="int", dest="i2c", default=0,
						help="Select I2C-Bus, default i2c-0")					
	parser.add_option("-r", "--read",action="callback", callback=Read_RTC,
						help="Read the date at MOD-RTC")					
	parser.add_option("-w", "--write", action="callback", callback=Write_RTC,
						help="Write system clock to MOD-RTC")					
	parser.add_option("-s", "--sync", action="callback", callback=Sync_RTC,
						help="Sync system clock with MOD-RTC")
		

	#parse options
	(options, args) = parser.parse_args()
	
	
	
	
	
	
	return 0

if __name__ == '__main__':
	main()

