#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include "uart.h"
#include "MOD-RFID125.h"

struct _users{
	char name[20];
	char code[20];
};

struct _users *users;
unsigned int LINES;

void ReadLines(FILE* file)
{
	char ch;
	
	LINES = 0;
	rewind(file);
	while(!feof(file))
	{
		ch = fgetc(file);
		if(ch == '\n')
			LINES++;
	}
		
}
void FillUsers(void)
{
	FILE *file;
	int i;	
	
	
	file = fopen("USERS", "r");
	if(file == NULL)
	{
		perror("Failed to open \"USERS\"");
	}
	else
	{
		ReadLines(file);
		rewind(file);
		users = malloc(LINES * sizeof(struct _users));
		for(i = 0; i < LINES; i++)
		{
			
			fscanf(file, "%s\t\t%s\r\n", users[i].name, users[i].code);
			
		}
		fclose(file);			
	}
}
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
void ReadCode(char* code)
{
	int fd;
	int i;
	char ch;
	char buffer[2];
	buffer[1] = 0;
	i = 0;
	
	fflush(stdin);
	Serial_Open(&fd, "/dev/tty1", B9600);
	WaitForChar(&fd, '-');
	while(1)
			{
				Serial_Read(&fd, buffer, 1);			
				ch = buffer[0];
				if(ch == '\n')
					break;
				code[i++] = ch;
			}
	code[i-1] = 0;
	Serial_Close(&fd);

}
int CheckName(char* name)
{
	int i;
	int flag = 0;
	
	
	for(i = 0; i < LINES; i++)
	{
		if(!strcmp(name, users[i].name))
			flag = 1;
	}
	return flag;	
}
int CheckCode(char* code)
{
	
	int i;
	int flag = 0;
	
	for(i = 0; i < LINES; i++)
	{
		if(!strcmp(code, users[i].code))
			flag = 1;
	}
	return flag;	

	return 0;
}
void RemoveUser(void)
{
	FILE *file;
	char name[100];
	int i;
	
	if(users != NULL)
		free(users);
	FillUsers();
	
	printf("USERNAME: ");
	scanf("%s", name);
	
	if(!CheckName(name))
		printf("No such user!\n");
	else
	{
		if((file = fopen("USERS", "w")) == NULL)
		{
			perror("Failed to open file!\n");
			exit(1);
		}
		else
		{		
			for(i = 0; i < LINES; i++)
			{
				if(!strcmp(name, users[i].name))
					continue;
				fprintf(file, "%s\t\t%s\r\n", users[i].name, users[i].code);
			}
			
			fclose(file);
			
		}
		
	}
	
	
}
void NewUser(void)
{
	FILE *file;
	char name[100];
	char code[100];

	
	
	if(users != NULL)
		free(users);
	FillUsers();
	
	do
	{
		printf("USERNAME: ");
		scanf("%s", name);
	}while(CheckName(name));
	
	do
	{
		ReadCode(code);
		printf("CODE: %s\n", code);

		if(CheckCode(code))
			printf("Code is already in use!\n");
		else
		{
			printf("Done.\n");
			break;
		}
	
		
	}while(1);
	
	
	
	if((file = fopen("USERS", "a+")) == NULL)
	{
		perror("Failed to open file!\n");
		exit(1);
	}
	else
	{		
		fprintf(file, "%s\t\t%s\r\n", name, code);
		fclose(file);
		
	}
}
void GetName(char *name, char *code)
{
	int i;
	if(users != NULL)
		free(users);
	FillUsers();
	
	for(i = 0; i < LINES; i++)
		if(!strcmp(code, users[i].code))
			strcpy(name, users[i].name);
}
void Scan(void)
{
	char code[20];
	char name[20];
	
	FILE *file;
	
	time_t mytime;
	mytime = time(NULL);
	

	
	while(1)
	{
		ReadCode(code);
		GetName(name, code);
		time(&mytime);
		

//		printf("FILE OPENED\n");	
		printf("\n%s\t\t%s", name, ctime(&mytime));
		
		
		
		file = fopen("LOG", "a+");
		fprintf(file, "%s\t\t%s", name, ctime(&mytime));
		fclose(file);
	}
	
}
int main(int argc, char** argv)
{
	if(argc == 1)
		Scan();
	else if(argc == 2)
	{
		if(!strcmp(argv[1], "--add") || !strcmp(argv[1], "-a"))
		{
			NewUser();
		}
		else if(!strcmp(argv[1], "--remove") || !strcmp(argv[1], "-r"))
		{
			RemoveUser();
		}
		else if(!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))
		{
			   printf ("\
Usage: %s [OPTION]\
",	      argv[0]);

 
			fputs ("\
\n\
  -a, --add                   	add user to the list\n\
  -r, --remove                  remove user from the list \n\
", stdout);

 
      fputs (("\
\n\
All user should have unique names and unique codes. Every time a tag\
is detected, a new record is added to the LOG file.\n\
\n\
"), stdout);

      printf (("\nReport bugs to <%s>.\n"), "OLIMEX");
      exit (1);

		}
		else
		{
			printf("Invalid parameter\n");
		}
	}

	else
	{
		printf("Invalid number of parameters. Use -h for more help.\n");
	}
	
	return 0;
}

