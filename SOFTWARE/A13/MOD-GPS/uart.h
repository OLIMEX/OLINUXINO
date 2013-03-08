
#ifndef _UART_H
#define _UART_H

extern int _DEBUG;

int OpenSerial(int *fd, char *SerialName, speed_t baudrate);
int CloseSerial(int *fd);
int WriteSerial(int *fd, char *buff, int len);
int ReadSerial(int *fd, char *buff, int len);
#endif
