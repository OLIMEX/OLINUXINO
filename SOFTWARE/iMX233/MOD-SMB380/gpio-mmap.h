#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define GPIO_BASE 0x80018000
#define GPIO_WRITE_PIN(gpio,value) GPIO_WRITE((gpio)>>5, (gpio)&31, value)
#define GPIO_WRITE(bank,pin,value) (gpio_mmap[0x140+((bank)<<2)+((value)?1:2)] = 1<<(pin))
#define GPIO_READ_PIN(gpio) GPIO_READ((gpio)>>5, (gpio)&31)
#define GPIO_READ(bank,pin) ((gpio_mmap[0x180+((bank)<<2)] >> (pin)) & 1)

int *gpio_mmap = 0;

int *gpio_map() {
	int fd;
	if (gpio_mmap != 0) return;
	fd = open("/dev/mem", O_RDWR);
	if( fd < 0 ) {
		perror("Unable to open /dev/mem");
		fd = 0;
	}

	gpio_mmap = mmap(0, 0xfff, PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
	if( -1 == (int)gpio_mmap) {
		perror("Unable to mmap file");
		gpio_mmap = 0;
	}
	if( -1 == close(fd))
		perror("Couldn't close file");
	fd=0;
}

int gpio_rd(long offset) {
	return gpio_mmap[offset/4];
}

void gpio_wr(long offset, long value) {
	gpio_mmap[offset/4] = value;
}

void gpio_output(int bank, int pin) {
	gpio_mmap[0x1C1 + (bank*4)] = 1 << pin;
}

void gpio_input(int bank, int pin) {
	gpio_mmap[0x1C2 + (bank*4)] = 1 << pin;
}

