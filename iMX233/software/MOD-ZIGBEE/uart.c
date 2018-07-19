#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <termios.h>
#include "uart.h"

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
int Serial_Open(int* fd, const char *serial_Name, speed_t baudrate)
{
	
	struct termios serCfg;
	memset(&serCfg, 0, sizeof(serCfg));
	if((*fd = open(serial_Name, O_RDWR | O_NOCTTY | O_NDELAY )) < 0)
	{
		perror("Failed to open serial port");
		exit(1);
	}
	else		
		if(tcgetattr(*fd, &serCfg) != 0)
		{
			perror("Failed to get configuration");
			exit(1);
		}

	cfsetispeed(&serCfg, baudrate);
	cfsetospeed(&serCfg, baudrate);
	cfmakeraw(&serCfg);

	if(tcsetattr(*fd, TCSANOW, &serCfg) != 0)
	{
		perror("Failed to set configuration");
		exit(1);
	}
	return 0;
}
int Serial_Send(int *fd, const char *string)
{
	int len;
	char *buffer;
	int bytes_sent;

	
	len = strlen(string);
	buffer = (char*)malloc((len+1)*sizeof(char));
	if(buffer == NULL)
	{
		perror("Failed to allocate memory");
		exit(1);
	}
	strcpy(buffer, string);
	buffer[len] = 0;

	bytes_sent = write(*fd, buffer,2);
	if(bytes_sent < 0)
	{
		perror("Failed to send data");
		exit(1);	
	}
	else
	{
		free(buffer);
		return 0;
	}	
}
int Serial_SendChar(int *fd, char ch)
{

	int bytes_sent;
	char a[1];
	a[0] = ch;
	bytes_sent = write(*fd, a, 1);
	if(bytes_sent < 0)
	{
		perror("Failed to send data");
		exit(1);	
	}
	else
		return 0;
}
int Serial_Read(int *fd, char *buff, int len)
{
	int chars;
	chars = read(*fd, buff, len);
	return chars;
}
int Serial_Close(int *fd)
{
		if(close (*fd) < 0)
		{
			perror("Failed to close file");
			exit(1);
		}
		else
			return 0;		
}
