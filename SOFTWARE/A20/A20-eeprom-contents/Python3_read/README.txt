OLIMEX board EEPROM reader for Python 3
=======================================

brd_info3.py reads the 256-byte board-information record used by supported
OLIMEX OLinuXino and SOM boards and prints the board name, revision, serial
number, storage, RAM, MAC address, and temperature grade.

This is a separate Python 3 replacement for the older Python_read/brd_info.py
utility. It does not modify the original script.


Requirements
------------

* Linux with Python 3.
* Permission to read the board EEPROM.

On current Olimage systems, the at24 kernel driver normally exposes the EEPROM
as:

    /sys/bus/i2c/devices/<bus>-0050/eeprom

brd_info3.py reads that file directly. This is the preferred method: it does
not unbind a kernel driver and does not require a Python SMBus module.

If no readable sysfs EEPROM is available, the program can fall back to
/dev/i2c-* and the Python smbus module. On Debian, Ubuntu, and Olimage, install
that optional fallback dependency with:

    sudo apt-get update
    sudo apt-get install python3-smbus

The program never installs packages itself.


Installation
------------

Copy brd_info3.py to the board and make it executable:

    chmod +x brd_info3.py


Usage
-----

Automatic EEPROM discovery:

    ./brd_info3.py

Show the raw 256-byte EEPROM record too:

    ./brd_info3.py --raw

Restrict reading to a specific I2C bus:

    ./brd_info3.py --bus 2

List all board IDs known to this version:

    ./brd_info3.py --list-ids

If access is denied, run the command as root or with sudo:

    sudo ./brd_info3.py


Example
-------

    $ ./brd_info3.py
    EEPROM source: /sys/bus/i2c/devices/2-0050/eeprom
    Board: A20-OLinuXino-LIME2-e16Gs16M Rev.L, Serial: 0x1573f, ID: 9604
    External memory: eMMC 16 GB
    RAM size: 1 GB
    MAC address: 30:1F:9A:D0:AC:B6
    Commercial grade (0-70 degrees Celsius)


Detection and safety
--------------------

The program searches for address 0x50 on the available I2C buses and reads the
complete 256-byte board-information record. It accepts a device only when the
record starts with the Olimex header 55 AA 4C 4F and its stored CRC-32 matches
the CRC-32 calculated over bytes 0-251.

If more than one EEPROM has a valid Olimex header, automatic selection stops
and asks for --bus. It does not silently choose one.

Unlike the proposed change in pull request #106, this program never unbinds
the at24 driver. Consequently an exception cannot leave the system EEPROM
driver detached.


Board-ID source
---------------

The board ID and name table is copied from the newest published Olimex
u-boot-olinuxino release branch available when this program was written:

    release-20250305
    board/olimex/common/boards.c

Source:

    https://github.com/OLIMEX/u-boot-olinuxino/blob/release-20250305/board/olimex/common/boards.c

The Olimage source tree used for verification was OLIMEX/olimage master commit
2da96a22b2bf83b89a30bb37417d2de34ccbcdb8 (2026-06-28).

When Olimex adds a new board ID, update BOARD_NAMES in brd_info3.py from the
same U-Boot source and keep its displayed name unchanged.


Exit status
-----------

0  EEPROM decoded successfully, or --list-ids completed.
1  EEPROM discovery, reading, or validation failed.
2  Invalid command-line arguments.
