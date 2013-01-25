/*
 * main.c
 * 
 * Copyright 2012 OLIMEX LTD
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include "uart.h"

void Console_Read(int fd);
void Message_Read(char *msg);

void WaitForChar(int *fd, char ch)
{
	char buffer[2];
	char c;
	buffer[1] = 0;
	
	do
	{
		Serial_Read(fd, buffer, 1);
		c = buffer[0];
	}while(c != ch);
}

int main(int argc, char **argv)
{
	if(argc == 1)
	{
		FILE *file;
		char msg[50];
		
		time_t mytime;
		mytime = time(NULL);

		while(1)
		{
			Message_Read(msg);
			fprintf(stdout, "%s\n", msg);
			time(&mytime);
			file = fopen("LOG", "a+");
			fprintf(file, "%s\t%s", msg, ctime(&mytime));
			fclose(file);
		}
		
	}
	if(argc == 2)
	{
		if(!strcmp(argv[1], "-config"))
		{
			int fd;
		//	char ch;
			fprintf(stdout, "CONFIGURATING.......%s", "0%");
			fflush(stdout);
			Serial_Open(&fd, "/dev/tty1", B19200);
			Serial_SendChar(&fd, '\n');
			fprintf(stdout, "\rCONFIGURATING......%s", "10%");
			fflush(stdout);			
			sleep(1);
			Console_Read(fd);
			fprintf(stdout, "\rCONFIGURATING......%s", "20%");
			fflush(stdout);
			Serial_SendChar(&fd, '1');
			fprintf(stdout, "\rCONFIGURATING......%s", "30%");
			fflush(stdout);
			sleep(1);	
			Console_Read(fd);
			fprintf(stdout, "\rCONFIGURATING......%s", "40%");
			fflush(stdout);
			Serial_SendChar(&fd, '1');
			fprintf(stdout, "\rCONFIGURATING......%s", "50%");
			fflush(stdout);
			sleep(1);
			Console_Read(fd);
			fprintf(stdout, "\rCONFIGURATING......%s", "60%");
			fflush(stdout);
			Serial_SendChar(&fd, '2');
			fprintf(stdout, "\rCONFIGURATING......%s", "70%");
			fflush(stdout);
			sleep(1);	
			Console_Read(fd);
			fprintf(stdout, "\rCONFIGURATING......%s", "80%");
			fflush(stdout);
			Serial_SendChar(&fd, '1');
			fprintf(stdout, "\rCONFIGURATING......%s", "90%");
			fflush(stdout);
			sleep(1);
			Console_Read(fd);
			fprintf(stdout, "\rCONFIGURATING.....%s\n", "100%");
			fflush(stdout);
		}
		else if(!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))
		{
			fputs("\
USAGE:	./%s\n\
		./%s -config\n", argv[1], argv[1]);
		}		
		else
		{
			printf("Invalid parameter.\n\
					Type --help for more information\n");
		}
	}
	return 0;
}

void Console_Read(int fd)
{
	int n;
	char buff[2];
	buff[1] = 0;

	while(1)
	{
		n = Serial_Read(&fd, buff, 1);
		if(n < 0) break;
//		fprintf(stdout, "%s", buff);
//		fflush(stdout);
			
	}	
}
void Message_Read(char *msg)
{
	int fd;
	char ch;
	int n;
	int i = 0;
	char buff[2];
	buff[1] = 0;
	tcflush(fd, TCIOFLUSH);
	Serial_Open(&fd, "/dev/tty1", B19200);
	WaitForChar(&fd, 10);
	while(1)
	{
		n = Serial_Read(&fd, buff, 1);
		if(n > 0)
		{
			ch = buff[0];
			if(ch == 13) break;
			msg[i++] = ch;
			msg[i] = 0;
			fflush(stdout);
		}	
	}
	Serial_Close(&fd);
}
	
