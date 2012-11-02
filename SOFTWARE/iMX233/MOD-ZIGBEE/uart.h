#ifndef UART_H
#define UART_H
#include <termios.h>

int Serial_Open(int* fd, const char *serial_Name, speed_t baudrate);
int Serial_Send(int *fd, const char *string);
int Serial_SendChar(int *fd, char ch);
int Serial_Read(int *fd, char *buff, int len);
int Serial_Close(int *fd);



#endif
