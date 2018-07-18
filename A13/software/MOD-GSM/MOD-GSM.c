/*
 * MOD-GSM.c
 * 
 * Copyright 2012  OLIMEX Ltd
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
#include <time.h>

struct phonebook
{
	char name[50];
	char number[14];
};
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

	bytes_sent = write(*fd, buffer, strlen(buffer));
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

void CheckNumber(char *string, char *phone)
{
	FILE *f;
	int lines, i;
	char ch;
	struct phonebook *names = NULL;
	strcpy(string, phone);
	f = fopen("PHONE-BOOK", "r");
	if(f!=NULL)
	{
		lines = 0;				
		while(!feof(f))
		{
			ch = getc(f);
			if(ch == '\n')
			lines++;
		}
		names = (struct phonebook*)malloc(lines * sizeof(struct phonebook));
		rewind(f);
		for(i = 0; i < lines; i++)
		{
			fscanf(f, "%s\t%s\n", names[i].name, names[i].number); 
			if(!strcmp(phone, names[i].number))
				{
					string[0] = 0;
					strcpy(string, names[i].name);
					break;
				}
		}
		free(names);				
		fclose(f);						
	}
	else
	{
		printf("Error opening file\n");
	}
}
int main(int argc, char** argv)
{
	int fd, i, lines, flag;
	char buffer[256], phone[14], command[6], string[20]; 
	struct phonebook *names = NULL;
	char ch;
	char number[30];
	char sms[160];

	FILE *f;
	
	time_t mytime;
	mytime = time(NULL);
	
	if(argc == 1)
	{
		char ch;
		FILE *f;

			
		
		Serial_Open(&fd, "/dev/tty1", B115200);
		if(tcflush(fd, TCIOFLUSH) != 0)
			exit(1);

		Serial_Send(&fd, "ATE0\r\n");
		sleep(1);
		Serial_Send(&fd, "AT+CLIP=1\r\n");
		sleep(1);
		Serial_Send(&fd, "AT+CMGF=1\r\n");
			

		while(1)
		{
			ch = 0;
			i = 0;
			while(ch != '+')
			{
				Serial_Read(&fd, buffer, 1);
				ch = buffer[0];
			}
			while(ch != ':')
			{
				Serial_Read(&fd, buffer, 1);
				ch = buffer[0];
				command[i++] = ch;
			}
			command[i-1] = 0;
			
			if(!strcmp(command, "CLIP"))
			{
				ch = 0;
				while(ch != '"')
				{
					Serial_Read(&fd, buffer, 1);
					ch = buffer[0];
				}
				ch = 0;
				i = 0;
				while(ch != '"')
				{
					Serial_Read(&fd, buffer, 1);
					ch = buffer[0];
					phone[i++] = ch;
				}
				phone[i-1] = 0;
				

				CheckNumber(string, phone);
				
				f = fopen("CALL-LOG", "a+");
				time(&mytime);
				if(f != NULL)
				{
					if(!strcmp(string, phone))
						fprintf(f, "%s\t%25s\t%25s", "", phone, ctime(&mytime));
					else
						fprintf(f, "%s\t%25s\t%25s", string, phone, ctime(&mytime));
				}
				fclose(f);		
				free(names);
				
				
				
				
				printf("\r\n%s IS CALLING...\n", string);
				
				for(i = 0; i < 20; i++)
				{
					 fprintf(stdout, "%c", '=');
					 fflush(stdout);
					 usleep(100000);
					
				}
				printf("\r\nCALL TERMINATED\n\r");
				
				
				Serial_Send(&fd, "ATH\r\n");					
				

			}
			else if(!strcmp(command, "CMTI"))
			{
				char index[4];
				while(ch != ',')
						{
							Serial_Read(&fd, buffer, 1);
							ch = buffer[0];
						}
			
				Serial_Read(&fd, buffer, 1);
				ch = buffer[0];
				index[0] = ch;
				index[1] = '\r';
				index[2] = '\n';
				index[3] = 0;
				printf("\n\rRecieved SMS\n");
				for(i = 0; i < 20; i++)
				{
					 fprintf(stdout, "%c", '=');
					 fflush(stdout);
					 usleep(50000);
					
				}
				printf("\r\n");
				command[0] = 0;
				strcpy(command, "AT+CMGR=");
				strcat(command, index);
				Serial_Send(&fd, command);
				
				while(ch != ':')
						{
							Serial_Read(&fd, buffer, 1);
							ch = buffer[0];
						}
				while(ch != '+')
						{
							Serial_Read(&fd, buffer, 1);
							ch = buffer[0];
						}
				i = 0;
				phone[i++] = ch;
				while(ch != '"')
						{
							Serial_Read(&fd, buffer, 1);
							ch = buffer[0];
							phone[i++] = ch;
						}
				phone[i-1] = 0;
				
				while(ch != '\n')
						{
							Serial_Read(&fd, buffer, 1);
							ch = buffer[0];
						}
				ch = 0;
				i = 0;		
				while(ch != '\n')
						{
							Serial_Read(&fd, buffer, 1);
							ch = buffer[0];
							sms[i++] = ch;
						}
				sms[i-2] = 0;		
				
				CheckNumber(string, phone);		
				
				
				
				printf("From: ");
				if(!strcmp(string, phone))
				{
					printf("%s\n", string);
				}
				else
				{
					printf("(%s)%s\n", string, phone);
				}
				printf("Date: %s", ctime(&mytime));
				printf("Message: %s\n", sms);
				
				f = fopen("SMS-LOG", "a+");
				if(f == NULL)
				{
					printf("Error opening file!\n");
				}
				else
				{
					fprintf(f, "From: ");
					if(!strcmp(string, phone))
					{
						fprintf(f, "%s\n", string);
					}
					else
					{
						fprintf(f,"(%s)%s\n", string, phone);
					}
					
					fprintf(f, "Date: %s", ctime(&mytime));
					fprintf(f, "----------------------------------\n\r");
					fprintf(f, "Message: %s\n", sms);
					fprintf(f, "----------------------------------\n\r\n\r");
					fclose(f);
				}
				
				
				
				
				
				
			}		
		}
		Serial_Close(&fd);
	
		
	}
	else
	{	
		if(!strcmp(argv[1], "-pb"))
		{
			if(argc < 3)
			{
				printf("Too few parameters!\n");
				exit(1);
			}
			if(!strcmp(argv[2], "ADD") || !strcmp(argv[2], "add"))
			{
				if(argc != 5)
				{
					printf("Invalid nunmber of parameters!\n");
					exit(1);
				}
				f = fopen("PHONE-BOOK" , "a+");
				fprintf(f, "%s\t%s\n", argv[3], argv[4]);
				fclose(f);
				printf("Record added\n");
				
			}
			else if(!strcmp(argv[2], "REMOVE") || !strcmp(argv[2], "remove"))
			{
			
				f = fopen("PHONE-BOOK" , "r");				
				if(f == NULL)
					{
						printf("Error opening file!\n");
						exit(1);
					}	
				

				lines = 0;				
				while(!feof(f))
				{
					ch = getc(f);
					if(ch == '\n')
						lines++;
				}
			
				
				if(!lines)
				{
					printf("The file is empty!\n");
					exit(1);
				}
	

				names = (struct phonebook*)malloc(lines * sizeof(struct phonebook));
				rewind(f);
				for(i = 0; i < lines; i++)
				{
					fscanf(f, "%s\t%s\n", names[i].name, names[i].number);
				}
				
				fclose(f);

				
				f = fopen("PHONE-BOOK", "w");
				
				for(i = 0; i < lines; i++)
				{
					if(strcmp(names[i].name, argv[3]))
						fprintf(f, "%s\t%s\n", names[i].name, names[i].number);
				}			

				free(names);
				fclose(f);
				printf("Record removed\n");
				
			}
			else
			{
				printf("Invalid parameter [%s]!\n", argv[2]);
				exit(1);
			}
			
		}
		if(!strcmp(argv[1], "-sms"))
		{

			
			if(argc != 3)
			{
				printf("Invalid number of parameters!\n");
				exit(1);
			}

			
			strcpy(number, "AT+CMGS=\"");
			if(argv[2][0] == '+')
				strcat(number, argv[2]);
			else
			{
				f = fopen("PHONE-BOOK" , "r");				
				if(f == NULL)
					{
						printf("Error opening file!\n");
						exit(1);
					}	
				

				lines = 0;				
				while(!feof(f))
				{
					ch = getc(f);
					if(ch == '\n')
						lines++;
				}
			
				
				if(!lines)
				{
					printf("The file is empty!\n");
					exit(1);
				}
	

				names = (struct phonebook*)malloc(lines * sizeof(struct phonebook));
				rewind(f);
				flag = 0;
				for(i = 0; i < lines; i++)
				{
					fscanf(f, "%s\t%s\n", names[i].name, names[i].number);
					if(!strcmp(argv[2], names[i].name))
					{
						strcat(number, names[i].number);
						flag++;
						break;
					}
						
				}
				free(names);
				fclose(f);
				if(!flag)
				{
					printf("No such name in the phonebook\n");
					exit(1);
				}			
			
			}
			strcat(number, "\"\r\n");
			
			printf("TYPE SMS\n");
			i = 0;
			do
			{
				ch = getchar();
				if(ch == '\n')
					{
						sms[i++] = '\n';
						sms[i++] = '\r';
						sms[i++] = 26;
						sms[i] = 0;
					}
				else
					{
						sms[i++] = ch;
					}
				
				
			}while(ch != '\n');
			
			Serial_Open(&fd, "/dev/tty1", B115200);


			
			Serial_Send(&fd, number);
			sleep(1);
			Serial_Send(&fd, sms);
			
			Serial_Close(&fd);
				
			
			printf("MESSAGE SENT\n");	
					
		}
		else if(!strcmp(argv[1], "--h") || !strcmp(argv[1], "-h") || !strcmp(argv[1], "-help"))
		{
			printf("USAGE:\n\r");
			printf("\n\r%s\n\r", argv[0]);
			printf("\t\tTo use the program as call and sms monitor. When a call or sms\n\r");
			printf("\t\tis recieved  the corresponding log is updated.\n\r");
			printf("\n\r%s -pb [ADD|REMOVE] NAME NUMBER\n\r", argv[0]);
			printf("\t\tUpdate the PHONE-BOOK. When using REMOVE enter only NAME.\n\r");
			printf("\n\r%s -sms [NUMBER|NAME]\n\r", argv[0]);
			printf("\t\tSend SMS to number. You can use NAME if its pressent in PHONE-BOOK.\n\r");
			printf("\t\tThe number must start with \"+\"\n\r\n\r");
					
		}
		else
		{
			printf("Invalid argument!\n");
			exit(1);
		}
	}
	return 0;
}
