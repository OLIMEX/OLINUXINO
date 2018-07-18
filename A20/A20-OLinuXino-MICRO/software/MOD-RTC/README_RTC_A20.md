This is a tutorial describing how to build a simple project for A20-OLinuXino-Micro Rev.F with demo for MOD-RTC Rev.A.

You will need:

- The boards
- SD card with Debian image (tested with ``A20_debian_kernel_3_4_LAN_USBx2_Cards_LCD_HDMI_SATA_TS_X_GPIO_OTG_MIC_Video_accel_release7.img``)
- Ethernet cable
- USB serial cable (like USB-SERIAL-CABLE-F)

1. Connect all the hardware and supply the board - USB serial cable to UART0 - TX0, RX0 and GND; and the MOD-RTC to UEXT1.

2. Open a console terminal program (like PuTTY, HyperTerminal etc.) with baudrate 115200

3. Wait until the board is initialized (you must see this: ``root@a20-OLinuXino:~#``)

4. Make a directory where the project files will be stored (this step is optional and it's only for better arrangement of the files in the directory tree).
You can do this with the command:

```bash
mkdir RTC
```

5. Set the created directory as current directory:

```bash
cd RTC
```

6. Since we are going to download the source and header files from GitHUB we must enable the internet connection:

```bash
ifup eth0
```

6.1. Allow the https protocol.
(This step could be skipped if https is allowed by default)

```bash
export https_proxy="192.168.0.1:80"
```

7. Download the files from the git repository:

```bash
wget <URL>
```

In order to get the URL, go to the git repository and locate [the directory of the project](https://github.com/OLIMEX/OLINUXINO/tree/master/SOFTWARE/A20/A20-OLinuXino-Micro%20with%20MOD-RTC).
Select the file you want to download and click on the "Raw" button.
Copy the URL and trigger the command (wget) in the console of A20-OLinuXino-MICRO.
Do this for the three files: RTC.c; i2c.c; i2c.h.

8. Just to make sure everything is fine and all three files are downloaded check the content of the directory:

```bash
ls
```

The response of the host board should be:

```
i2c.c  i2c.h  RTC.c
```

9. Compile all source files:

```bash
gcc *.c
```

A new file should appear.
We check the content of the directory again:

```bash
ls
```

Output:

```
a.out  i2c.c  i2c.h  RTC.c
```

10. Execute the generated file with option to READ from I2C2

```bash
./a.out -r 2
```

Output:

```
Sec: 40
Min: 33
Hour: 4
MDays: 0
WDays: 5
Month: 0
Year: 20
```

...which **IS NOT** correct data as the date and time are not set!

11. Execute it again but this time with WRITE option.
Thus we will synchronise the time on the MOD-RTC with the one on the host board.
Run:

```bash
./a.out -w 2
```

12. Execute for third time to read again:

```bash
./a.out -r 2
```

Output:

```
Sec: 53
Min: 24
Hour: 11
MDays: 7
WDays: 1
Month: 7
Year: 114
```

The function "localtime" (which the host board uses) counts the years after 1900.
More information [here](http://linux.die.net/man/3/localtime).
