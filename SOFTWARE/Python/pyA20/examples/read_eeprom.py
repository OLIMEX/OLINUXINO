#!/usr/bin/env python
"""Read from the EEPROM chip on A20-OLinuXino-MICRO

On the board there is small chip U3. This is 16kb eeprom memory AT24C16BN.
The i2c address can be different, but on this specific board is 0x50.

The text will be big mess if python3 is used.
"""

from pyA20 import i2c

__author__ = "Stefan Mavrodiev"
__copyright__ = "Copyright 2014, Olimex LTD"
__credits__ = ["Stefan Mavrodiev"]
__license__ = "GPL"
__version__ = "2.0"
__maintainer__ = __author__
__email__ = "support@olimex.com"

eeprom_address = 0x50

"""Initialize i2c bus"""
i2c.init("/dev/i2c-1")
i2c.open(eeprom_address)

"""Set address pointer to the first"""
i2c.write([0x00])

print "Dump eeprom:"
print "="*24

print "    ",
for i in xrange(16):
    print " %x" % i,

print "\t",
for i in xrange(16):
    print "%x" % i,
print ""

"""Print data"""
for i in xrange(128):
    page = i2c.read(16)
    print "%03x:" % (i*0x10),
    for j in xrange(0, 16):
        print "%02x" % page[j],

    """Print characters"""
    print "\t",
    for j in xrange(16):
        if page[j] <= 126 and page[j] >= 32:
            print chr(page[j]),
        else:
            print '.',
    print ""

i2c.close()