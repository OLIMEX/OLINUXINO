This is a simple demo for Olimex boards OLinuXino that runs GTK2 (GIMP ToolKit v2) application. 
The application provides option to select a pin on the board from a dropdown or radio buttons and then allows you to set it as input/output and to write/read its value.

Tested with OLinuXino-Micro rev.K and "Armbian_5.84.1_Olinuxino-a20_Ubuntu_bionic_next_5.0.14_desktop" image. 
For detailed information about the GTK itself we recommend the tutorial of Simon Long: https://www.raspberrypi.org/magpi-issues/C_GUI_Programming.pdf

In order to work properly you have to:
1) Flash a valid image to an SD card and run the board.

2) Login as root (otherwise you won't have access to pin direction and input/output value)

3) Install GTK. Open terminal and type:
	sudo apt-get install libgtk2.0-dev

4) Create folder where to store the sources:
	mkdir GPIOs

5) Navigate to this folder
	cd GPIOs

6) Download or copy the source files to the working directory. Should do it for the 2 sources and 1 header: main.c, gpio_lib.c and gpio_lib.h
	wget <URL>

7) Compile the sources (the flags listed here are important for the GTK)
	gcc *.c `pkg-config gtk+-2.0 --cflags --libs` -o GPIO

8) Run the application
	./GPIO

If you get error message like this: Gtk-WARNING **: 11:06:20.115: cannot open display:
try this (https://www.youtube.com/watch?v=DSySW9r5CIg):
	export DISPLAY=:0
and run the application again. This should make it but will solve the issue only for this session. To solve it permenantly you have to add it to the environment file:
	sudo nano /etc/environment
There add "export DISPLAY=:0" at the end of the file and save it.

9) When you run the application a window will appear with variaty of options.
You can choose the GPIO by either selecting Port/Pin from the dropdown menu, or select radio buttons of the GPIOs on the extentions.  When you select the GPIO you make it input or output and then you are able to select it's value (if output) with the check box or to read (if input) where the checkbox will show the value. Keep in mind not ALL of the pins are GPIO only. Some of them are multiplexed by ethernet, SD card, UARTs and other peripheral so they are excluded and if you select them nothing will happened.

2019/06/28
