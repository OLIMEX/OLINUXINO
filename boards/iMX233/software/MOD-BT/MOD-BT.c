/*
 * untitled.c
 * 
 * Copyright 2012 OLIMEX Ltd
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
#include <string.h>
#include <termios.h>
#include "uart.h"

int main(int argc, char **argv)
{

	if(argc == 2)
	{
		if(!strcmp(argv[1], "--transmit") || !strcmp(argv[1], "-t"))
		{
			int i;
			int fd;
			char ch;
			char temp[100];
			i = 0; 
			while(1)
			{				
				ch = getche();
				temp[i++] = ch;
				if('\n' == ch)
					break;
			}
			temp[i++] = '\r';
			temp[i] = 0;
			Serial_Open(&fd, "/dev/ttySP1", B115200);
			Serial_Send(&fd, temp);
			Serial_Close(&fd);	
		}
		else if(!strcmp(argv[1], "--receive") || !strcmp(argv[1], "-r"))
		{
			int fd;
			char ch;
			char buffer[2];
			
			buffer[1] = 0;
			
			Serial_Open(&fd, "/dev/ttySP1", B115200);
			while(1)
			{
				Serial_Read(&fd, buffer, 1);
				ch = buffer[0];
				if(ch == '\r')
					break;
				fprintf(stdout, "%c", ch);
				fflush(stdout);
			}	
			
			Serial_Close(&fd);
			printf("\n\r");	
		}
		else if(!strcmp(argv[1], "--config") || !strcmp(argv[1], "-c"))
		{
			int fd;			
			Serial_Open(&fd, "/dev/ttySP1", B115200);
			Serial_Send(&fd, "AT+BTAUT=1,0\r");
			Serial_Close(&fd);			
			fputs("Configurating...Done\n\r", stdout);
		}
		else if(!strcmp(argv[1], "--start") || !strcmp(argv[1], "-s"))
		{
			int fd;			
			Serial_Open(&fd, "/dev/ttySP1", B115200);
			Serial_Send(&fd, "AT+BTSRV=1\r");
			Serial_Close(&fd);			
			fputs("Starting...Done\n\r", stdout);
		}
		else if(!strcmp(argv[1], "--stop") || !strcmp(argv[1], "-p"))
		{
			int fd;			
			Serial_Open(&fd, "/dev/ttySP1", B115200);
			Serial_Send(&fd, "+++");
			Serial_Close(&fd);			
			fputs("Stopping...Done\n\r", stdout);
		}
		else if(!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))
		{
			fputs("\n\
USAGE: ./mod-bt [PARAMS]\n", stdout);
		
			fputs("\n\
-c, --config\t\tConfigure\n\
-s, --start\t\tStart the module\n\
-p, --stop\t\tStop the module\n\
-t, --transmit\t\tSend a message to BT device.\n\
-r, --receive\t\tReceive a message from BT device\n", stdout);
		}
		else
		{
			fputs("Unknown parameter!\n", stdout);
		}
	}
	else
	{
		fputs("Wrong number of parameters!\n",stdout);
	}
	
	return 0;
}

