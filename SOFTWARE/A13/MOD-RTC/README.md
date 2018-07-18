= MOD-RTC.py

Execute program:

  sudo python MOD-RTC.py

Example:

  // Write system clock to MOD-RTC
  sudo python MOD-RTC.py --verbose --i2c=0 -w

  // Change system clock
  sudo date -s "1 JAN 1901"

  // Read MOD-RTC
  sudo python MOD-RTC.py --verbose --i2c=0 -r

  // Sync system clock
  sudo python MOD-RTC.py --verbose --i2c=0 -s

  // View current date
  date


= MOD-RTC.c

Compile program:

  sudo make clean
  sudo make

Execute program:

  sudo su
  ./MOD-RTC

Same example as above:

  sudo su
  ./MOD-RTC --verbose -w 0
  date -s "1 JAN 1901"
  ./MOD-RTC --verbose -r 0
  ./MOD-RTC --verbose -s 0
  date

Where 0 is I2C-Bus:

- 0 for IMX233
- 2 for A13
- 0,1 for RPi, depending of the revision

If you want to sync system clock on boot you can add the code to rc.local

  vi /etc/rc.local

for example add the line:

  ./MOD-RTC -s 0

28.01.2013
Olimex LTD
