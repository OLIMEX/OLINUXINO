#ifndef UART_H
#define UART_H

int Serial_Open(int* fd, char *serial_Name, speed_t baudrate);
int Serial_Send(int *fd, char *string);
int Serial_Read(int *fd, char *buff, int len);
int Serial_Close(int *fd);
int getch(void);
int getche(void);



#endif
