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

import smbus
import os
import time
import datetime
import optparse

I2C_Bus = 2
RTC_BUG_FIX = False
TIME_ZONE = 'Europe/Sofia'

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
    return year % 4


def bug_fix(buf, last_write):
    variation = 0
    if last_write[0] == BCDtoInt(buf[6])+1900 and last_write[1] <= 2 and last_write[2] <= 28:
        variation = 0
    elif  last_write[0] == BCDtoInt(buf[6])+1900 and last_write[1] >= 2:
        variation = 1
    elif last_write[0] < BCDtoInt(buf[6])+1900:
        for i in range(BCDtoInt(buf[6])+1900 - last_write[0]):
            variation += is_leap_year(last_write[0])
        variation -= 1
    if BCDtoInt(buf[3]) == 29 and BCDtoInt(buf[5]) == 2 and is_leap_year(BCDtoInt(buf[6])+1900) > 0:
        buf[3] = 1
        buf[5] = 3
    fix_time = '%s-%s-%s %s:%s:%s' % (BCDtoInt(buf[6])+1900, BCDtoInt(buf[5]), BCDtoInt(buf[3]),
                                          BCDtoInt(buf[2]), BCDtoInt(buf[1]),
                                          BCDtoInt(buf[0]))
    return datetime.datetime.strptime(fix_time, '%Y-%m-%d %H:%M:%S') - datetime.timedelta(days=variation)

def Read_RTC(option, opt, value, parser): # @UndefinedVariable):
    "Read the date from MOD-RTC"
    global I2C_Bus
    global RTC_BUG_FIX
    global TIME_ZONE
    I2C_Bus = parser.values.i2c
    RTC_BUG_FIX = parser.values.rtcbug
    TIME_ZONE = parser.values.timezone
    bus = smbus.SMBus(I2C_Bus)
    buf = bus.read_i2c_block_data(0x51, 0x02, 7)
    buf[0] &= 0x7F;
    buf[1] &= 0x7F;
    buf[2] &= 0x3F;
    buf[3] &= 0x3F;
    buf[4] &= 0x07;
    buf[5] &= 0x1F;
    buf[6] &= 0xFF;
    if RTC_BUG_FIX == True:
        buf2 = bus.read_i2c_block_data(0x51, 0x09, 3)
        buf2[0] &= 0xFF;
        buf2[1] &= 0x1F;
        buf2[2] &= 0x3F;
    
        last_write = [BCDtoInt(buf2[0])+1900, BCDtoInt(buf2[1]), BCDtoInt(buf2[2])]
        fix_time = bug_fix(buf, last_write)
        data = "%s-%s-%s %s:%s:%s" % ( fix_time.year, fix_time.month, fix_time.day, fix_time.hour, fix_time.minute, fix_time.second)
    else:
        data = "%d-%d-%d %d:%d:%d" % (BCDtoInt(buf[6])+1900, BCDtoInt(buf[5]), BCDtoInt(buf[3]),
                                      BCDtoInt(buf[2]), BCDtoInt(buf[1]),
                                      BCDtoInt(buf[0]))
    try:
        data = datetime.datetime.strptime(data, '%Y-%m-%d %H:%M:%S')
        print "%s-%s-%s %s:%s:%s" % ( data.year, data.month, data.day, data.hour, data.minute, data.second)
        return data
    except ValueError as e:
        print 'found rtc bug use  -b 1 option'
        print e
        return e

def Write_RTC(option, opt, value, parser):  # @UndefinedVariable
    "Write system clock to MOD-RTC" 
    global I2C_Bus
    global RTC_BUG_FIX
    I2C_Bus = parser.values.i2c
    RTC_BUG_FIX = parser.values.rtcbug
    
    bus = smbus.SMBus(I2C_Bus)
    a = time.localtime()
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
    bus.write_i2c_block_data(0x51, 0x02, buf)   
    if RTC_BUG_FIX == True:
        bus.write_i2c_block_data(0x51, 0x09, [InttoBCD(a.tm_year-1900), InttoBCD(a.tm_mon), InttoBCD(a.tm_mday)])
    print 'DONE!'
    return True



def Sync_RTC(option, opt, value, parser):
    "Sync system clock with MOD-RTC"
    global I2C_Bus
    global RTC_BUG_FIX
    global TIME_ZONE
    I2C_Bus = parser.values.i2c
    RTC_BUG_FIX = parser.values.rtcbug
    TIME_ZONE = parser.values.timezone

    bus = smbus.SMBus(I2C_Bus)  # @UndefinedVariable   
    buf = bus.read_i2c_block_data(0x51, 0x02, 7) 
    buf[0] &= 0x7F;
    buf[1] &= 0x7F;
    buf[2] &= 0x3F;
    buf[3] &= 0x3F;
    buf[4] &= 0x07;
    buf[5] &= 0x1F;
    buf[6] &= 0xFF; 

    if RTC_BUG_FIX == True:
        buf2 = bus.read_i2c_block_data(0x51, 0x09, 3)
        buf2[0] &= 0xFF;
        buf2[1] &= 0x1F;
        buf2[2] &= 0x3F;
        last_write = [BCDtoInt(buf2[0])+1900, BCDtoInt(buf2[1]), BCDtoInt(buf2[2])]
        fix_time = bug_fix(buf, last_write)
        dates = '%s-%s-%s' % (fix_time.year, fix_time.month, fix_time.day)
        times = '%s:%s:%s' % (fix_time.hour, fix_time.minute, fix_time.second)
    else:
        #return "%d-%d-%d %d:%d:%d" % (BCDtoInt(buf[6])+1900, BCDtoInt(buf[5]), BCDtoInt(buf[3]),
        #                              BCDtoInt(buf[2]), BCDtoInt(buf[1]),
        #                              BCDtoInt(buf[0]))
        dates = '%s-%s-%s' % (BCDtoInt(buf[6])+1900, buf[5], BCDtoInt(buf[3]))
        times = '%s:%s:%s' % (BCDtoInt(buf[2]), BCDtoInt(buf[1]),BCDtoInt(buf[0]))
        cmd = 'date -s %s' % (dates)
        os.system(cmd)
        cmd = 'date -s %s' % (times)
        os.system(cmd) 
    return True

def main():
    
    #get options from command line
    usage = "usage: %prog [options] arg1 ...."
    parser = optparse.OptionParser(usage = usage)

    parser.add_option("-b", "--rtcbug", type="int", dest="rtcbug", default=0,
                        help="Fix RTC-MOD bug 29-02 if year is not leap")
    
    parser.add_option("-t", "--timezone", type="str", dest="timezone", default='Europe/Sofia',
                        help="Write RTC time in local time")

    parser.add_option("-i", "--i2c", type="int", dest="i2c", default=2,
                        help="Select I2C-Bus, default i2c-0")      
              
    parser.add_option("-r", "--read",action="callback", callback=Read_RTC,
                        help="Read the date at MOD-RTC")                    
    parser.add_option("-w", "--write", action="callback", callback=Write_RTC,
                        help="Write system clock to MOD-RTC")                    
    parser.add_option("-s", "--sync", action="callback", callback=Sync_RTC,
                        help="Sync system clock with MOD-RTC")
        

    #parse options
    (options, args) = parser.parse_args()
    return True

if __name__ == '__main__':
    main()
