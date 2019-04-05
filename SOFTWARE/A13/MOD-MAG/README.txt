# MOD-MAG

## Description

The demo program is compiled on OLinuXino Maxi board, with the latest available version of ArchLinux v2.6.35-6.

The program opens the configuration file ``/dev/i2c-0`` and sets a pointer to this file.
The ``set_data`` and ``get_data`` functions use the ``ioctl`` function, which is part of the i2c driver.
The ``I2C_RDWR`` parameter means that in one transaction after write the read begins right after start condition - no STOP.
On the terminal are displayed registers from ``0x00`` to ``0x08``. 

For more information please read MOD-MAG user manual and MAG3110FCR2 datasheet.

## Support

https://www.olimex.com/dev

## Release

4 Sept 2012
