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
import pytz

I2C_Bus = 2
RTC_BUG_FIX = False
TIME_ZONE = 'UTC'

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
    if year % 4 == 0 and (year % 100 != 0 or year % 400 == 0):
        return True
    return False

# FIXME: Test me
def bug_fix(date_now, last_write, go_one_day_up):
    variation = 0
    if last_write[0] == date_now.year and last_write[1] <= 2 and last_write[2] <= 28:
        variation = 0
    elif  last_write[0] == date_now.year and last_write[1] >= 2 and is_leap_year(date_now.year) == False:
        variation = 1
    elif last_write[0] < date_now.year:
        for i in range(date_now.year - last_write[0]):
            if is_leap_year(last_write[0]+i) == False:
                variation += 1
        if last_write[1] <= 2 and last_write[2] <= 28 and variation > 0:
            variation -= 1
        elif date_now.day <=28 and date_now.month <= 2 and variation > 0 and is_leap_year(date_now.year) == False:
            variation -= 1
    if go_one_day_up == True and variation > 0:
        variation -= 1
    return date_now - datetime.timedelta(days=variation)

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
        go_one_day_up = False
        if BCDtoInt(buf[5]) == 2 and BCDtoInt(buf[3]) == 29 and is_leap_year(BCDtoInt(buf[6])+1900) == False:
            buf[5] = InttoBCD(3)
            buf[3] = InttoBCD(1)
            go_one_day_up = True

    date_now = "%d-%d-%d %d:%d:%d" % (BCDtoInt(buf[6])+1900, BCDtoInt(buf[5]), BCDtoInt(buf[3]),
                                  BCDtoInt(buf[2]), BCDtoInt(buf[1]),
                                  BCDtoInt(buf[0]))
    if RTC_BUG_FIX == True:
        buf2 = bus.read_i2c_block_data(0x51, 0x09, 3)
        buf2[0] &= 0xFF;
        buf2[1] &= 0x1F;
        buf2[2] &= 0x3F;
        date_now = datetime.datetime.strptime(date_now, '%Y-%m-%d %H:%M:%S')
        last_write = [BCDtoInt(buf2[0])+1900, BCDtoInt(buf2[1]), BCDtoInt(buf2[2])]
        date_now = bug_fix(date_now, last_write, go_one_day_up)
    else:
        try:
            date_now = datetime.datetime.strptime(date_now, '%Y-%m-%d %H:%M:%S')
        except ValueError as e:
            print 'found rtc bug use  -b 1 option'
            print e
            return False
    date_now = date_now.replace(tzinfo=pytz.UTC)
    date_now = date_now.astimezone(pytz.timezone(TIME_ZONE))
    print "%s-%s-%s %s:%s:%s TZ: %s" % ( date_now.year, date_now.month, date_now.day, date_now.hour, date_now.minute, date_now.second, TIME_ZONE)
    return date_now

def Write_RTC(option, opt, value, parser):  # @UndefinedVariable
    "Write system clock to MOD-RTC" 
    #global I2C_Bus
    #global RTC_BUG_FIX
    #I2C_Bus = parser.values.i2c
    #RTC_BUG_FIX = parser.values.rtcbug
    
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
    if RTC_BUG_FIX == True: 
        bus.write_i2c_block_data(0x51, 0x09, [InttoBCD(a.tm_year-1900), InttoBCD(a.tm_mon), InttoBCD(a.tm_mday)])
    bus.write_i2c_block_data(0x51, 0x02, buf) 
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
        go_one_day_up = False
        if BCDtoInt(buf[5]) == 2 and BCDtoInt(buf[3]) == 29 and is_leap_year(BCDtoInt(buf[6])+1900) == False:
            buf[5] = InttoBCD(3)
            buf[3] = InttoBCD(1)
            go_one_day_up = True

    date_now = "%d-%d-%d %d:%d:%d" % (BCDtoInt(buf[6])+1900, BCDtoInt(buf[5]), BCDtoInt(buf[3]),
                                  BCDtoInt(buf[2]), BCDtoInt(buf[1]),
                                  BCDtoInt(buf[0]))
    if RTC_BUG_FIX == True:
        buf2 = bus.read_i2c_block_data(0x51, 0x09, 3)
        buf2[0] &= 0xFF;
        buf2[1] &= 0x1F;
        buf2[2] &= 0x3F;
        go_one_day_up = False
        date_now = datetime.datetime.strptime(date_now, '%Y-%m-%d %H:%M:%S')
        last_write = [BCDtoInt(buf2[0])+1900, BCDtoInt(buf2[1]), BCDtoInt(buf2[2])]
        date_now = bug_fix(date_now, last_write, go_one_day_up)
    else:
        try:
            date_now = datetime.datetime.strptime(date_now, '%Y-%m-%d %H:%M:%S')
        except ValueError as e:
            print 'found rtc bug use  -b 1 option'
            print e
            return False
    date_now = date_now.replace(tzinfo=pytz.UTC)
    date_now = date_now.astimezone(pytz.timezone(TIME_ZONE))
    dates = '%s-%s-%s' % (date_now.year, date_now.month, date_now.day)
    times = '%s:%s:%s' % (date_now.hour, date_now.minute, date_now.second)
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
                        help="Fix RTC-MOD bug 29-02 if year is not leap, default 0 ( no bug )")
    
    parser.add_option("-t", "--timezone", type="str", dest="timezone", default='UTC',
                        help="Write RTC time in local time, default UTC")

    parser.add_option("-i", "--i2c", type="int", dest="i2c", default=2,
                        help="Select I2C-Bus, default i2c-2")      
              
    parser.add_option("-r", "--read",action="callback", callback=Read_RTC,
                        help="Read the date at MOD-RTC")                    
    parser.add_option("-w", "--write", action="callback", callback=Write_RTC,
                        help="Write system clock to MOD-RTC")                    
    parser.add_option("-s", "--sync", action="callback", callback=Sync_RTC,
                        help="Sync system clock with MOD-RTC")
        

    #parse options
    (options, args) = parser.parse_args()
    return 'DONE!'

if __name__ == '__main__':
    main()
