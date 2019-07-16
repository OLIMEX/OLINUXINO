#ifndef	_I2C_H
#define	_I2C_H

#define I2C0	"/dev/i2c-0"
#define I2C1	"/dev/i2c-1"
#define I2C2	"/dev/i2c-2"
#define I2C3	"/dev/i2c-3"
#define I2C4	"/dev/i2c-4"
#define I2C5	"/dev/i2c-5"

extern char I2C[];

#define	I2C_WRITE	0
#define	I2C_READ	1

void I2C_Open(int *fd, unsigned char address);

void I2C_Close(int *fd);

int I2C_Send(int *fd, unsigned char *buffer, unsigned char num_bytes);

int I2C_Read(int *fd, unsigned char *buffer, unsigned char num_bytes);

int I2C_Transfer (unsigned char Address, unsigned char R_W, unsigned char Register, unsigned char *Data, int N);

void I2C_WriteByte (unsigned char Address, unsigned char Register, unsigned char Data);

unsigned char I2C_ReadByte (unsigned char Address, unsigned char Register);

#endif
