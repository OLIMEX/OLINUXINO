#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <termios.h>
#include "uart.h"


int Serial_Open(int* fd, char *serial_Name, speed_t baudrate)
{
	
	struct termios serCfg;
	memset(&serCfg, 0, sizeof(serCfg));
	if((*fd = open(serial_Name, O_RDWR)) < 0)
		return -1;
	else		
		if(tcgetattr(*fd, &serCfg) != 0)
			return -1;

	cfsetispeed(&serCfg, baudrate);
	cfsetospeed(&serCfg, baudrate);
	cfmakeraw(&serCfg);

	tcflush(*fd, TCIFLUSH);
	if(tcsetattr(*fd, TCSANOW, &serCfg) != 0)
		return -1;
	return 0;
}
int Serial_Send(int *fd, char *string)
{
	int len;
	char *buffer;
	int bytes_sent;

	
	len = strlen(string);
	if(len > 255)
		return -1;
	buffer = (char*)malloc((len+1)*sizeof(char));
	if(buffer == NULL)
		return -1;
	strcpy(buffer, string);
	buffer[len] = 0;
	bytes_sent = write(*fd, buffer,len+1);
	if(bytes_sent < 0)
	{
		close (*fd);
		return -1;		
	}
	else
	{
		free(buffer);
		return 0;
	}	
}
int Serial_Read(int *fd, char *buff, int len)
{
	int chars;
	chars = read(*fd, buff, len);
	if(chars < 0)
		return -1;
	else
	{
		buff[1] = 0;		
		return 0;
	}	
}
int Serial_Close(int *fd)
{
		if(close (*fd) < 0)
			return -1;
		else
			return 0;		
}
int kbhit(void)
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}
#if 1
int getch(void)
{
	struct termios oldattr, newattr;
	int ch;
	tcgetattr( STDIN_FILENO, &oldattr );
	newattr = oldattr;
	newattr.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
	return ch;
}

int getche(void)
{
	struct termios oldattr, newattr;
	int ch;
	tcgetattr( STDIN_FILENO, &oldattr );
	newattr = oldattr;
	newattr.c_lflag &= ~( ICANON );
	tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
	return ch;
}
#endif
