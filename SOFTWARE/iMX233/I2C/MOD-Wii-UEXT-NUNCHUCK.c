#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>





int main(int argc, char **argv) 
{

        int file;
	char buffer_in[6], buffer_out[2];
	int c, z;
	int num_bytes_read,num_bytes_write, address, i, data[6]; 
	char device[] = "/dev/i2c-0";
	

	address = 0x52;	
		
	
	
	buffer_out[0] = 0xF0;
	buffer_out[1] = 0x55;
	num_bytes_write = 2;

	if ((file = open(device, O_RDWR)) < 0)	
		exit(1);	
	if(ioctl(file, I2C_SLAVE, address) < 0)
		exit(1);	
	if(write(file, buffer_out, num_bytes_write) != num_bytes_write)	
		exit(1);
	close(file);

	while(1)
	{
		buffer_out[0] = 0x00;
		num_bytes_write = 1;
	
		if ((file = open(device, O_RDWR)) < 0)	
			exit(1);	
		if(ioctl(file, I2C_SLAVE, address) < 0)
			exit(1);	
		if(write(file, buffer_out, num_bytes_write) != num_bytes_write)	
			exit(1);
		close(file);			

		num_bytes_read = 6;
		if ((file = open(device, O_RDWR)) < 0)
			exit(1);

		
		if(ioctl(file, I2C_SLAVE, address) < 0)
			exit(1);
		if(read(file, buffer_in, num_bytes_read) != num_bytes_read)
			exit(1);

		for(i = 0; i < num_bytes_read; i++)
		{
			buffer_in[i] ^= 0x17;
			buffer_in[i] += 0x17;
			data[i] = buffer_in[i];				
		}
		z = data[5] & 0x01;
		c = (data[5] >> 1) & 0x01;
		
		data[2] <<= 2;
		data[2] |= ((data[5] >> 2) & 0x03);
		
		data[3] <<= 2;
		data[3] |= ((data[5] >> 4) & 0x03);

		data[4] <<= 2;
		data[4] |= ((data[5] >> 6) & 0x03);
	

		printf("Analog X: %d\n", data[0]);
		printf("Analog Y: %d\n", data[1]);
		printf("X-axis: %d\n", data[2]);
		printf("Y-axis: %d\n", data[3]);
		printf("Z-axis: %d\n", data[4]);

		if(z)
			printf("Button Z: not pressed\n");
		else
			printf("Button Z: pressed\n");

		if(c)
			printf("Button C: not pressed\n");
		else
			printf("Button C: pressed\n");

	
	printf("\f");
	usleep(200000);
	}
	
    return 0;
}
