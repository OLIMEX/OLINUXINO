#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <time.h>
#include "gpio-mmap.h"


//MOSI -> bank[0], pin[20]
//MISO -> bank[0], pin[0]
//SCK -> bank[0], pin[24]
//cs - bank[0], pin[3]

struct timespec delay = {
	0, 10000L
};

void SPI_Init(void)
{
	gpio_map();
	
	gpio_output(0, 20);
	gpio_input(0, 0);
	gpio_output(0, 24);
	gpio_output(0, 3);
	
	GPIO_WRITE(0, 3, 1);
	GPIO_WRITE(0, 24, 1);
	GPIO_WRITE(0, 20, 0);
}
void SPI_Read(char address, unsigned char *buffer, int bytes)
{
	
	int i, j;
	address |= 0x80;
	
	gpio_map();
	
	
	GPIO_WRITE(0, 3, 0);	
	
	//SEND ADDRESS
	for(i = 0; i < 8; i++)
	{
		nanosleep(&delay, NULL);
		GPIO_WRITE(0, 24, 0);
		if(address & 0x80)
			GPIO_WRITE(0, 20, 1);
		else
			GPIO_WRITE(0, 20, 0);
		address <<= 1;
		nanosleep(&delay, NULL);
		GPIO_WRITE(0, 24, 1);						
	}

	//READ DATA
	for(j = 0; j < bytes; j++)
	{
		buffer[j] = 0;
		for(i = 0; i < 8; i++)
		{
			nanosleep(&delay, NULL);
			GPIO_WRITE(0, 24, 0);
			
			nanosleep(&delay, NULL);
			GPIO_WRITE(0, 24, 1);
			buffer[j] |= GPIO_READ(0, 0);	
			if(i != 7)
				buffer[j] <<= 1;
		}	
		usleep(1000);
	}
	
	nanosleep(&delay, NULL);
	GPIO_WRITE(0, 3, 1);
}

