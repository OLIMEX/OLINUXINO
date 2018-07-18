# A13-Python module for GPIO and SPI (bitbang) access

Now you can have access to the hardware of A13-OLinuXino when you write in Python.
GPIOs are named same way as in the schematic: PIN1 ... PIN39

1. Install package

Download from https://pypi.python.org/pypi/pyA13/

  tar -zxvf pyA13-0.1.12.tar.gz
  cd pyA13-0.1.12.tar.gz
  sudo python setup.py install

2. Example:

## GPIO

```python
#!/usr/bin/env python

import A13_GPIO as GPIO

#init module
GPIO.init()

#configure module
GPIO.setcfg(GPIO.PIN#, GPIO.OUT)
GPIO.setcfg(GPIO.PIN#, GPIO.IN)

#read the current GPIO configuration
config = GPIO.getcfg(GPIO.PIN#)

#set GPIO high
GPIO.output(GPIO.PIN#, GPIO.HIGH)

#set GPIO low
GPIO.output(GPIO.PIN#, GPIO.LOW)

#read input
state = GPIO.input(GPIO.PIN#)

#cleanup
GPIO.cleanup()
```

Or use blinking led script

  sudo python blink.py

## SPI

```python
#import A13_SPI as SPI

#init
SPI.init(mode)

#write
SPI.write(byte1, byte2, byte3, ....)

#read
SPI.read(address, byte1, byte2, byte3, ....)
```
