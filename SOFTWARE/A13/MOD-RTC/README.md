# MOD-RTC.py

Execute program:

```bash
sudo python MOD-RTC.py
```

Example:

```bash
# Write system clock to MOD-RTC
sudo python MOD-RTC.py --verbose --i2c=0 -w

# Change system clock
sudo date -s "1 JAN 1901"

# Read MOD-RTC
sudo python MOD-RTC.py --verbose --i2c=0 -r

# Sync system clock
sudo python MOD-RTC.py --verbose --i2c=0 -s

# View current date
date
```

# MOD-RTC.c

Compile program:

```bash
sudo make clean
sudo make
```

Execute program:

```bash
sudo su
./MOD-RTC
```

Same example as above:

```bash
sudo su
./MOD-RTC --verbose -w 0
date -s "1 JAN 1901"
./MOD-RTC --verbose -r 0
./MOD-RTC --verbose -s 0
date
```

Where 0 is I2C-Bus:

- 0 for IMX233
- 2 for A13
- 0,1 for RPi, depending of the revision

If you want to sync system clock on boot you can add the code to rc.local

```bash
vi /etc/rc.local
```

For example, add the line:

```bash
./MOD-RTC -s 0
```

28.01.2013

Olimex LTD
