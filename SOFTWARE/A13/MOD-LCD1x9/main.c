#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#include "LCD1x9.h"
#include "LCD1x9_mapping.h"

#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#include "LCD1x9.h"
#include "LCD1x9_mapping.h"

/* DEFINE LOCAL TYPES HERE */

#define BUF_LEN_INIT 24
#define BUF_LEN_WRITE 22


/* DECLARE EXTERNAL VARIABLES HERE */

/* DEFINE LOCAL MACROS HERE */

/* DEFINE LOCAL VARIABLES HERE */
static char lcdBitmap[20]; // 40segments * 4 = 160px, 160 / 8 = 20bytes

/* DECLARE EXTERNAL VARIABLES HERE */

/* DEFINE FUNCTIONS HERE */

/******************************************************************************
* Description: LCD1x9_Initialize(..) - initializes pins and registers of the LCD1x9
*				Also lights up all segments
* Input: 	none
* Output: 	none
* Return:	0 if sucessfully initialized, -1 if error occured 
*******************************************************************************/
char LCD1x9_Initialize(void)
{


	int file, i, rcv;
	char device[] = {"/dev/i2c-2"};
	char address = 0x38;
	char *buffer;


	fprintf(stdout, "Initiating I2C...\n");
	fflush(stdout);
		
	fprintf(stdout, "Opening device %s...", device);
	fflush(stdout);

	if ((file = open(device, O_RDWR)) < 0)
	{
		fprintf(stdout, "Error\n");
		fflush(stdout);
		return -1;
	}
	else
	{
		fprintf(stdout, "Done\n");
		fflush(stdout);


		fprintf(stdout, "Opening control file...");
		fflush(stdout);

		if(ioctl(file, I2C_SLAVE, address) < 0)
		{
			fprintf(stdout, "Error\n");
			fflush(stdout);
			return -1;
		}
		else
		{
			fprintf(stdout, "Done\n");
			fflush(stdout);
			

			fprintf(stdout, "Allocating memory...");
			fflush(stdout);

			buffer = (char*)malloc(BUF_LEN_INIT * sizeof(char));

			if(device == NULL)
			{
				fprintf(stdout, "Error\n");
				fflush(stdout);
				return -1;
			}
			else
			{
				fprintf(stdout, "Done\n");
				fflush(stdout);
				fprintf(stdout, "Memmory allocated at address: 0x%X\n", (unsigned int)buffer);
				fflush(stdout);
				
					
				buffer[0] = 0xC8;				//Mode register
				buffer[1] = 0xF0;				//Blink register
				buffer[2] = 0xE0;				//Device select register
				buffer[3] = 0x00;				//Point register
				for(i = 4; i < 24 ; i++)
				{
					buffer[i] = 0xFF;
					lcdBitmap[i-4] = 0xFF;
				}


				fprintf(stdout, "Initiating MOD-LCD-1x9...");
				fflush(stdout);

				rcv = write(file, buffer, BUF_LEN_INIT);

				if(rcv != BUF_LEN_INIT)
				{
					fprintf(stdout, "Error\n");
					fflush(stdout);

					fprintf(stdout, "Bytes write: %d of %d\n", (int)rcv, (int)BUF_LEN_INIT);
					fflush(stdout);
					return -1;
				}
				else
				{					
				
					printf("Done\n");
					fprintf(stdout, "Bytes write: %d of %d\n", (int)rcv, (int)BUF_LEN_INIT);
					fflush(stdout);

					free(buffer);
					close(file);

					return 0;
				
				}
			}

		}

	}
}


/******************************************************************************
* Description: LCD1x9_enableSegment(..) - enables a segment in the display buffer
*		Note: Does not actually light up the segment, have to call the 'LCD1x9_Update(..)'
* Input: 	comIndex - backplate index
*			bitIndex - segment index
* Output: 	none
* Return:	none
*******************************************************************************/
void LCD1x9_enableSegment(char comIndex, char bitIndex)
{
	if(bitIndex >= 40)
		return;
		
	comIndex &= 0x3;
	
	if(bitIndex & 0x1)
		comIndex |= 0x4;
		
	bitIndex >>= 1;
	
	lcdBitmap[bitIndex] |= 0x80 >> comIndex;
}

/******************************************************************************
* Description: LCD1x9_disableSegment(..) - disables a segment in the display buffer
*		Note: Does not actually lights off the segment, have to call the 'LCD1x9_Update(..)'
* Input: 	comIndex - backplate index
*			bitIndex - segment index
* Output: 	none
* Return:	none
*******************************************************************************/
void LCD1x9_disableSegment(char comIndex, char bitIndex)
{
	if(bitIndex >= 40)
		return;
		
	comIndex &= 0x3;
	
	if(bitIndex & 0x1)
		comIndex |= 0x4;
		
	bitIndex >>= 1;
	
	lcdBitmap[bitIndex] &= ~(0x80 >> comIndex);
}

/******************************************************************************
* Description: LCD1x9_Update(..) - disables a segment in the display buffer
*		Note: Does not actually lights off the segment, have to call the 'LCD1x9_Update(..)'
* Input: 	comIndex - backplate index
*			bitIndex - segment index
* Output: 	none
* Return:	none
*******************************************************************************/
void LCD1x9_Update(void)

{

	int file, i, rcv;
	char device[] = {"/dev/i2c-2"};
	char address = 0x38;
	char *buffer;


	fprintf(stdout, "Updating buffer...\n");
	fflush(stdout);
		
	fprintf(stdout, "Opening device %s...", device);
	fflush(stdout);

	if ((file = open(device, O_RDWR)) < 0)
	{
		fprintf(stdout, "Error\n");
		fflush(stdout);
		return;
	}
	else
	{
		fprintf(stdout, "Done\n");
		fflush(stdout);


		fprintf(stdout, "Opening control file...");
		fflush(stdout);

		if(ioctl(file, I2C_SLAVE, address) < 0)
		{
			fprintf(stdout, "Error\n");
			fflush(stdout);
			return;
		}
		else
		{
			fprintf(stdout, "Done\n");
			fflush(stdout);
			

			fprintf(stdout, "Allocating memory...");
			fflush(stdout);

			buffer = (char*)malloc(BUF_LEN_WRITE * sizeof(char));

			if(device == NULL)
			{
				fprintf(stdout, "Error\n");
				fflush(stdout);
				return;
			}
			else
			{
				fprintf(stdout, "Done\n");
				fflush(stdout);
				fprintf(stdout, "Memmory allocated at address: 0x%X\n", (unsigned int)buffer);
				fflush(stdout);
				
					
				buffer[0] = 0xE0;				//Device select register
				buffer[1] = 0x00;				//Point register
				for(i = 2; i < 22 ; i++)
				{
					buffer[i] = lcdBitmap[i-2];
				}


				fprintf(stdout, "Writing to MOD-LCD...");
				fflush(stdout);

				rcv = write(file, buffer, BUF_LEN_WRITE);

				if(rcv != BUF_LEN_WRITE)
				{
					fprintf(stdout, "Error\n");
					fflush(stdout);

					fprintf(stdout, "Bytes write: %d of %d\n", (int)rcv, (int)BUF_LEN_WRITE);
					fflush(stdout);
					return;
				}
				else
				{					
				
					printf("Done\n");
					fprintf(stdout, "Bytes write: %d of %d\n", (int)rcv, (int)BUF_LEN_WRITE);
					fflush(stdout);

					free(buffer);
					close(file);
				
				}
			}

		}

	}
}

/******************************************************************************
* Description: LCD1x9_Write(..) - writes a string to the display
* Input: 	string - the string to write, no more than 9 characters
*			bitIndex - segment index
* Output: 	none
* Return:	none
*******************************************************************************/
void LCD1x9_Write(char *string)
{
	char data, length, index, i;
	int bitfield;
	char com, bit;
	
	length = strlen(string);
	if(length > 9)
		return;
	
	index  = 0;
	/* fill out all characters on display */
	for (index = 0; index < 9; index++) {
		if (index < length) {
			data = string[index];
		} else {
			data = 0x20; // fill with spaces if string is shorter than display
		}

		data -= 0x20;
		bitfield = LCDAlphabet[data];
	
		for (i = 0; i < 16; i++) {
			bit = LCD1x9.Text[index].bit[i];
			com = LCD1x9.Text[index].com[i];
		
			if (bitfield & ((int)1 << i)) {
				LCD1x9_enableSegment(com, bit);
			} else {
				LCD1x9_disableSegment(com, bit);
			}
		}
	}

	LCD1x9_Update();
}
int main(int argc, char **argv[])
{
	

	if(argc != 2)
	{
		fprintf(stdout, "Invalid number of parameters [%d]\n", (int)argc);
		exit(1);
	}
	else
	{
		if(!strcmp((const char*)argv[1],"-h") || !strcmp((const char*)argv[1],"--h") || !strcmp((const char*)argv[1],"-help"))
		{
			fprintf(stdout, "USAGE: %s [...]\n", (char*)argv[0]);
			fprintf(stdout, "\t-init\tInitialize the display controler and lights up all segments\n");
			fprintf(stdout, "\t[SOME TEXT]\tDisplay text on the lcd. Maximum lenght is 9 characters\n");
			fflush(stdout);
	
		}
		else if(!strcmp((const char*)argv[1],"-init"))
		{

			LCD1x9_Initialize();

		}
		else
		{

			LCD1x9_Write((char*)argv[1]);

		}
	}	
}


	
