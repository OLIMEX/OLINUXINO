#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include "I2C.h"

char I2C[] = I2C0;

void I2C_Open(int *fd, unsigned char address)
{
	*fd = open(I2C, O_RDWR);
	ioctl(*fd, I2C_SLAVE_FORCE, address);
}

void I2C_Close(int *fd)
{
	close(*fd);
}

int I2C_Send(int *fd, unsigned char *buffer, unsigned char num_bytes)
{
	return write(*fd, buffer, num_bytes);
}

int I2C_Read(int *fd, unsigned char *buffer, unsigned char num_bytes)
{
	return read(*fd, buffer, num_bytes);
}

int I2C_Transfer (unsigned char Address, unsigned char R_W, unsigned char Register, unsigned char *Data, int N)
{
	int fd;
	I2C_Open(&fd, Address);
	if (R_W == I2C_WRITE)	// write
	{
		unsigned char Buff[N+1];
		Buff[0] = Register;
		for (int i=0; i<N; i++)
			Buff[i+1] = Data[i];
		I2C_Send(&fd, Buff, N+1);
	}
	else	// read
	{
		I2C_Send(&fd, &Register, 1);
		I2C_Close(&fd);

		I2C_Open(&fd, Address);
		I2C_Read(&fd, Data, N);
	}
	I2C_Close(&fd);
}

void I2C_SendByte (unsigned char Address, unsigned char Register, unsigned char Data)
{
	I2C_Transfer (Address, I2C_WRITE, Register, &Data, 1);
}

unsigned char I2C_ReadByte (unsigned char Address, unsigned char Register)
{
	unsigned char Data;
	I2C_Transfer (Address, I2C_READ, Register, &Data, 1);
	return Data;
}
