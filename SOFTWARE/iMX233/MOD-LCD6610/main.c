
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <termios.h>
#include "main.h"
#include "s1d15g00.h"


//message begin-end pixels
#define BEGIN	22
#define END		106

int Serial_Open(int* fd, char *serial_Name, speed_t baudrate)
{
	
	struct termios serCfg;

	memset(&serCfg, 0, sizeof(serCfg));

	if((*fd = open(serial_Name, O_RDWR)) < 0)
	{
		printf("Error\n");
		return -1; /* Can't open port */
	}
	else
	{	
		
		/* Get the actual configuration */
		if(tcgetattr(*fd, &serCfg) != 0)
		{
			return -1; /* Can't get port parms */
		}
	}
		

	/* Set the baud rates to baudrate (e.g. 115200-> baudrate=B115200) */	
	cfsetispeed(&serCfg, baudrate);
	cfsetospeed(&serCfg, baudrate);

	/* Set 8N1 config and other params. See cfmakeraw() description */
	cfmakeraw(&serCfg);
	

	/* Let the configuration take effect */
	if(tcsetattr(*fd, TCSANOW, &serCfg) != 0)
	{
		return -1; /* Can't set port params */
	}
	return 0;
}

int Serial_Send(int *fd, char *string)
{
	char buffer[2];
	
	buffer[0] = 0x55;
	buffer[1] = 0;
	write(*fd, buffer, strlen(buffer));
	return 0;		
}

int Serial_Close(int *fd)
{		
		if(close (*fd) < 0)
		{
			return -1;
		}
		else
		{			
			return 0;
			
		}		
}
void Initialization (void)
{
	
	InitLcd ();	
	return;
}

int main (int argc, char **argv)
{

	int fd;
	
	SPI_Init ();
	
	
/*	
	while(1)
	{
		for(i = 0; i < 66; i++)
		{
			LCDSetRect(65-i, 65-i, 65+i, 65+i, BLACK, RED);
		}
		for(i = 0; i < 66; i++)
		{
			LCDSetRect(65-i, 65-i, 65+i, 65+i, BLACK, WHITE);
		}
		for(i = 0; i < 66; i++)
		{
			LCDSetRect(65-i, 65-i, 65+i, 65+i, BLACK, GREEN);
		}
		for(i = 0; i < 66; i++)
		{
			LCDSetRect(65-i, 65-i, 65+i, 65+i, BLACK, PINK);
		}
	}
*/

	if(argc < 2)
	{
		printf("Too few arguments\n");
		exit(1);
	}
	
	if(!strcmp(argv[1], "-clear"))
	{
		LCDClearScreen();
	}
	else if(!strcmp(argv[1], "-init"))
	{
		Serial_Open(&fd, "/dev/ttySP1", B9600);
		Serial_Send(&fd, "UUUUU");
		Serial_Close(&fd);
		Initialization();
		LCDClearScreen();			
	}
	
	else if(!strcmp(argv[1], "-pixel"))
	{
		if(argc != 5)
		{
			printf("Invalid number of parameters\n");
			exit(1);
		}
		
		int x, y, color;
		x = strtol(argv[2], NULL, 0);
		y = strtol(argv[3], NULL, 0);
		
		if(!strcmp(argv[4], "BLACK"))
			color = BLACK;
		else if(!strcmp(argv[4], "RED"))
			color = RED;
		else if(!strcmp(argv[4], "GREEN"))
			color = GREEN;
		else if(!strcmp(argv[4], "BLUE"))
			color = BLUE;
		else if(!strcmp(argv[4], "CYAN"))
			color = CYAN;
		else if(!strcmp(argv[4], "MAGENTA"))
			color = MAGENTA;
		else if(!strcmp(argv[4], "YELLOW"))
			color = YELLOW;
		else if(!strcmp(argv[4], "BROWN"))
			color = BROWN;
		else if(!strcmp(argv[4], "ORANGE"))
			color = ORANGE;
		else if(!strcmp(argv[4], "PINK"))
			color = PINK;
		else
			color = strtol(argv[4], NULL, 0);
			
				
		LCDSetPixel(x, y, color);		
	}

	else if(!strcmp(argv[1], "-line"))
	{
		if(argc != 7)
		{
			printf("Invalid number of parameters\n");
			exit(1);
		}
		
		int x1, y1, x2, y2, color;
		x1 = strtol(argv[2], NULL, 0);
		y1 = strtol(argv[3], NULL, 0);
		x2 = strtol(argv[4], NULL, 0);
		y2 = strtol(argv[5], NULL, 0);
		if(!strcmp(argv[6], "BLACK"))
			color = BLACK;
		else if(!strcmp(argv[6], "RED"))
			color = RED;
		else if(!strcmp(argv[6], "GREEN"))
			color = GREEN;
		else if(!strcmp(argv[6], "BLUE"))
			color = BLUE;
		else if(!strcmp(argv[6], "CYAN"))
			color = CYAN;
		else if(!strcmp(argv[6], "MAGENTA"))
			color = MAGENTA;
		else if(!strcmp(argv[6], "YELLOW"))
			color = YELLOW;
		else if(!strcmp(argv[6], "BROWN"))
			color = BROWN;
		else if(!strcmp(argv[6], "ORANGE"))
			color = ORANGE;
		else if(!strcmp(argv[6], "PINK"))
			color = PINK;
		else
			color = strtol(argv[6], NULL, 0);		
		LCDSetLine(x1, y1, x2, y2, color);
	}	
	else if(!strcmp(argv[1], "-rect"))
	{
		if(argc != 8)
		{
			printf("Invalid number of parameters\n");
			exit(1);
		}		
		int x1, y1, x2, y2, color, fill;
		x1 = strtol(argv[2], NULL, 0);
		y1 = strtol(argv[3], NULL, 0);
		x2 = strtol(argv[4], NULL, 0);
		y2 = strtol(argv[5], NULL, 0);
		
		
		fill = strtol(argv[6], NULL, 0);
		
			
		if(!strcmp(argv[7], "BLACK"))
			color = BLACK;
		else if(!strcmp(argv[7], "RED"))
			color = RED;
		else if(!strcmp(argv[7], "GREEN"))
			color = GREEN;
		else if(!strcmp(argv[7], "BLUE"))
			color = BLUE;
		else if(!strcmp(argv[7], "CYAN"))
			color = CYAN;
		else if(!strcmp(argv[7], "MAGENTA"))
			color = MAGENTA;
		else if(!strcmp(argv[7], "YELLOW"))
			color = YELLOW;
		else if(!strcmp(argv[7], "BROWN"))
			color = BROWN;
		else if(!strcmp(argv[7], "ORANGE"))
			color = ORANGE;
		else if(!strcmp(argv[7], "PINK"))
			color = PINK;
		else
			color = strtol(argv[7], NULL, 0);		
		LCDSetRect(x1,y1, x2, y2, fill, color);
	}
	else if(!strcmp(argv[1], "-circle"))
	{
		if(argc != 6)
		{
			printf("Invalid number of parameters\n");
			exit(1);
		}		
		int x, y, r, color;
		x = strtol(argv[2], NULL, 0);
		y = strtol(argv[3], NULL, 0);
		r = strtol(argv[4], NULL, 0);
		
		if(!strcmp(argv[5], "BLACK"))
			color = BLACK;
		else if(!strcmp(argv[5], "RED"))
			color = RED;
		else if(!strcmp(argv[5], "GREEN"))
			color = GREEN;
		else if(!strcmp(argv[5], "BLUE"))
			color = BLUE;
		else if(!strcmp(argv[5], "CYAN"))
			color = CYAN;
		else if(!strcmp(argv[5], "MAGENTA"))
			color = MAGENTA;
		else if(!strcmp(argv[5], "YELLOW"))
			color = YELLOW;
		else if(!strcmp(argv[5], "BROWN"))
			color = BROWN;
		else if(!strcmp(argv[5], "ORANGE"))
			color = ORANGE;
		else if(!strcmp(argv[5], "PINK"))
			color = PINK;
		else
			color = strtol(argv[5], NULL, 0);
		
		LCDSetCircle(x, y, r, color);
	}
	else if(!strcmp(argv[1], "-string"))
	{
		if(argc != 8)
		{
			printf("Invalid number of parameters\n");
			exit(1);
		}
				
		int x, y, size, fcolor, bcolor;
		x = strtol(argv[3], NULL, 0);
		y = strtol(argv[4], NULL, 0);
		size = strtol(argv[5], NULL, 0);
		
		if(!strcmp(argv[6], "BLACK"))
			fcolor = BLACK;
		else if(!strcmp(argv[6], "RED"))
			fcolor = RED;
		else if(!strcmp(argv[6], "GREEN"))
			fcolor = GREEN;
		else if(!strcmp(argv[6], "BLUE"))
			fcolor = BLUE;
		else if(!strcmp(argv[6], "CYAN"))
			fcolor = CYAN;
		else if(!strcmp(argv[6], "MAGENTA"))
			fcolor = MAGENTA;
		else if(!strcmp(argv[6], "YELLOW"))
			fcolor = YELLOW;
		else if(!strcmp(argv[6], "BROWN"))
			fcolor = BROWN;
		else if(!strcmp(argv[6], "ORANGE"))
			fcolor = ORANGE;
		else if(!strcmp(argv[6], "PINK"))
			fcolor = PINK;
		else
			fcolor = strtol(argv[6], NULL, 0);
		
		
		if(!strcmp(argv[7], "BLACK"))
			bcolor = BLACK;
		else if(!strcmp(argv[7], "RED"))
			bcolor = RED;
		else if(!strcmp(argv[7], "GREEN"))
			bcolor = GREEN;
		else if(!strcmp(argv[7], "BLUE"))
			bcolor = BLUE;
		else if(!strcmp(argv[7], "CYAN"))
			bcolor = CYAN;
		else if(!strcmp(argv[7], "MAGENTA"))
			bcolor = MAGENTA;
		else if(!strcmp(argv[7], "YELLOW"))
			bcolor = YELLOW;
		else if(!strcmp(argv[7], "BROWN"))
			bcolor = BROWN;
		else if(!strcmp(argv[7], "ORANGE"))
			bcolor = ORANGE;
		else if(!strcmp(argv[7], "PINK"))
			bcolor = PINK;
		else
			bcolor = strtol(argv[7], NULL, 0);
		
		LCDPutStr(argv[2], x, y, size, fcolor, bcolor);
	}
	else
	{
		printf("HELP:\n");
		printf("USAGE:\t [-COMMAND] [-PARAMS]\n");
		printf("\n\r\"-init\"\t\t-initialize the display.\n");
		printf("\"-clear\"\t-clear the screen.\n");
		printf("\"-pixel\"\t-pixel [X] [Y] [COLOR].\n");
		printf("\"-line\"\t-\t-line [X1] [Y1] [X2] [Y2] [COLOR].\n");
		printf("\"-rect\"\t-\t-rect [X1] [Y1] [X2] [Y2] [FILL] [COLOR].\n");
		printf("\"-circle\"\t-circle [X] [Y] [R] [COLOR].\n");
		printf("\"-string\"\t-string [TEXT] [X] [Y] [SIZE] [FCOLOR] [BCOLOR].\n");
		
	}
	
		


	return 0;
}
