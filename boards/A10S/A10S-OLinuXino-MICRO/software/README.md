# A10S GPIO under Android

## GPIO using ADB

1. Boot android (from nand or sdcard)
2. Connect USB-to-Serial cable on UART1
3. Open terminal at speed 115200
4. When loading is ready in the terminal type "sh" and enter
5. "ls /sys/class/gpio_sw" will give you a list of all accessable GPIOs.
The GPIOs name is according to the chip ports.
For example, PE3 is LED1.
6. To set PE3 as output:

```bash
echo 1 > /sys/class/gpio_sw/PE3/mul_sel
```

or as input

```bash
echo 0 > /sys/class/gpio_sw/PE3/mul_sel
```

7. To turn on an LED:

```bash
echo 1 > /sys/class/gpio_sw/PE3/data
```

and to turn it off:

```bash
echo 0 > /sys/class/gpio_sw/PE3/data
```

8. If GPIO is as input to read state:

```bash
cat /sys/class/gpio_sw/PE3/data
```

## GPIO using Android SDK

Basically, you should read and write to the filesystem.
Ensure that your application is running as root, or nothing will happen.
The demo program toggle LED1 on every button press.
All read/write methods are realized in the GPIO.java file.
