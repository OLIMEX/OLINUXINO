CC	= gcc
CPP = g++
AR	= ar

all:
	mkdir -p build
	$(CC) -c -static -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast gpio_lib.c -o build/gpio.o
	$(CPP) -c -static -O2 A20-gpio.cpp -o build/A20-gpio.o
	$(AR) rcs libA20-gpio.a build/A20-gpio.o build/gpio.o

clean:
	rm -r build/ libA20-gpio.a
