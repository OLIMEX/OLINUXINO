This is a simple demo for Olimex boards OLinuXino that runs GTK2 (GIMP ToolKit v2) application. The application provides option to send or receive data from a module attached to any of the I2C on the OLinuXino board.

Tested with OLinuXino-Micro rev.K and "Armbian_5.84.1_Olinuxino-a20_Ubuntu_bionic_next_5.0.14_desktop" image. For detailed information about the GTK itself we recommend the tutorial of Simon Long: https://www.raspberrypi.org/magpi-issues/C_GUI_Programming.pdf

In order to work properly you have to:
1) Flash a valid image to an SD card and run the board.

2) Login as root (otherwise you won't have access to pin direction and input/output value)

3) Install GTK. Open terminal and type:
	sudo apt-get install libgtk2.0-dev

4) Create folder where to store the sources:
	mkdir I2C

5) Navigate to this folder
	cd I2C

6) Download or copy the source files to the working directory. Should do it for the 2 sources and 1 header: main.c, I2C.c and I2C.h
	wget <URL>

7) Compile the sources (the flags listed here are important for the GTK)
	gcc *.c `pkg-config gtk+-2.0 --cflags --libs` -o I2C

8) Run the application
	./I2C

If you get error message like this: Gtk-WARNING **: 11:06:20.115: cannot open display:
try this (https://www.youtube.com/watch?v=DSySW9r5CIg):
	export DISPLAY=:0
and run the application again. This should make it but will solve the issue only for this session. To solve it permenantly you have to add it to the environment file:
	sudo nano /etc/environment
There add "export DISPLAY=:0" at the end of the file and save it.

9) When you run the application a window will appear with variaty of options.
You can select the I2C (0-5) on which you have attached your slave device.
From the radio group choose whether you will read or write data.
Select the slave address, the register you will transfer data and the number of bytes you want to send/receive.
In the text box of data you will receive the data if you are reading or input the data that you want to send. You can send an array of data in which case you type them inside the <Data> text box.
Note that Slave address, Register and Data are in hexadecimal format, while Number of Bytes is decimal.

2019/07/12
