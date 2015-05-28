Simple Linux example for MOD-IO2
==============

Relay control via analog and digital input.
--------------

This is a tutorial how to build a simple project for A20-OLinuXino-Micro Rev.F with demo for MOD-IO2 Rev.B.

In addition to the boards you also need SD card with Debian image (tested with "A20_OLinuxino_Micro_debian_34_90_release_10.img"), LAN and USB serial cable (like USB-SERIAL-CABLE-F).

1. Connect all the hardware and supply the board - USB serial cable to UART0 - TX0, RX0 and GND; and the MOD-IO2 to UEXT1.

2. Open a console terminal program (like PuTTY, HyperTerminal etc.) with baudrate 115200

3. Wait until the board is initialized (you must see this: "root@a20-OLinuXino:~#")

4. Make a directory where the project files will be stored (this step is optional and it's only for better arrangement of the files in the directory tree). You can do this with the command:

Input:

```
mkdir MOD-IO2
```

5. Set the created directory as current directory:

Input:

```
cd MOD-IO2
```

6. Since we are going to download the source and header files from GitHUB we must enable the internet connection:

Input:

```
ifup eth0
```

7. Download the files from GitHUB:

Input:

```
wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/UEXT%20MODULES/MOD-IO2/simple-relay-example/main.cpp
wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/UEXT%20MODULES/MOD-IO2/simple-relay-example/IO2.h
wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/UEXT%20MODULES/MOD-IO2/simple-relay-example/IO2.cpp
wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/UEXT%20MODULES/MOD-IO2/simple-relay-example/i2c.h
wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/UEXT%20MODULES/MOD-IO2/simple-relay-example/i2c.cpp
```

In order to get the URL, go to GitHUB find the directory of the project: https://github.com/OLIMEX/OLINUXINO/tree/master/SOFTWARE/UEXT%20MODULES/MOD-IO2/simple-relay-example ; select the file you want to download and click on the "Raw" button; then copy the URL and trigger the command (wget) in the console of A20-OLinuXino-Micro. Do this for the five files: i2c.cpp; i2c.h; IO2.cpp; IO2.h; main.cpp.

8. Just to make sure everything is fine and all five files are downloaded check the content of the directory:

Input:
```
ls
```

The response from the host board should be:

```
i2c.cpp  i2c.h  IO2.cpp  IO2.h  main.cpp
```

9. Compile all source files:

Input:

```
g++ *.cpp
```

A new file should appear. We check the content of the directory again:

Input:

```
ls
```

Output:
```
a.out  i2c.cpp  i2c.h  IO2.cpp  IO2.h  main.cpp
```

10. Execute the generated file

Input:

```
./a.out
```

The relays state will change depending on input. Each relay uses different input. Relay 1 uses analog input 0 (GPIO0) - when GPIO0 is at low level the relay would turn on, when there is high level analog input the relay would turn on. Relay 2 uses digital input 1 (GPIO1) and acts reversly - when GPIO1 is connected to GND, the relay will turn off; when it is connected to 3.3V the relay would turn on. To terminate the program press CTRL+Z.


2015/05/28
Stanimir Petev, Olimex