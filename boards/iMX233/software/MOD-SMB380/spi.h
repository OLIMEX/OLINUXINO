#ifndef _SPI_H
#define _SPI_H

void SPI_Init(void);
void SPI_Read(char address, unsigned char *buffer, int bytes);
void SPI_Write(char address, unsigned char *buffer, int bytes);
#endif
