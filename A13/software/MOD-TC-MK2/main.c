/*
 * main.c
 * 
 * Copyright 2012 OLIMEX-LTD
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
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include "i2c.h"

int main(int argc, char **argv)
{
	
	/*Check for number of parameters. Should be at least 2*/
	if(argc < 2)
	{
		printf("Invalid number of parameters\n");
		exit(1);
	}
	else
	{
		/*HELP COMMAND: --help*/
		if(!strcmp(argv[1], "--help"))
		{
			     printf ("\
Usage: %s [OPTION]... ADDRESS [COMMAND]\
",	      argv[0]);

 
      fputs ("\
  -d, --tris                   set the TRIS registers\n\
  -l, --lat                    set the LAT registers\n\
", stdout);

      fputs ("\
  -p, --port                   get the PORT registers\n\
  -u, --pullup                 turn PULL-UPs\n\
", stdout);

      fputs ("\
  -an0, --analog0                get value of AN0\n\
  -an1, --analog1                get value of AN1\n\
  -an2, --analog2                get value of AN2\n\
  -an6, --analog6                get value of AN6\n\
  -an7, --analog7                get value of AN7\n\
  -a, --analog                   get value of all analog inputs\n\
", stdout);

      fputs ("\
  -t, --temperature             get readings from thermocouple\n\
  -n, --address					set new address\n\
", stdout);
  
 
      fputs (("\
\n\
GPIO3 is always input. Also pull-ups can be turned on only on PORTA.\
To set a new address put jumper and reset the board. \n\
\n\
"), stdout);

      printf (("\nReport bugs to <%s>.\n"), "OLIMEX");
      exit (1);

		}
		
		/*HELP COMMAND: -temp 0xA0*/
		else if(!strcmp(argv[1], "--temperature") || !strcmp(argv[1], "-t"))
		{
			/*Check parameters*/
			if(argc != 3)
			{
				printf("Invalid number of parameters\n");
				exit(1);
			}
			else
			{
				int fd;		//File descriptor
				int address;
				int i;
				unsigned char in_buffer[5];
				unsigned char out_buffer[10];
				
				unsigned long int open_circuit_mask = 0x00000001;
				unsigned long int gnd_short_mask 	= 0x00000002;
				unsigned long int vcc_short_mask 	= 0x00000004;
				unsigned long int fault_mask 		= 0x00010000;
				unsigned long int sign_int_mask		= 0x00008000;
				unsigned long int sign_ext_mask		= 0x80000000;
				unsigned long int temperature		= 0;
				unsigned long int foobar			= 0;
				
				float external = 0;
				float internal = 0;
				
				
				address = strtol(argv[2], NULL, 0);
				
				
				
				in_buffer[0] = 0x01;	//MOD-TC-MK2 Identificator
				in_buffer[1] = address;
				in_buffer[2] = 0x21;	//GET_TEMP COMMAND
				
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 3);				
				sleep(1);
				/*READ DATA*/				
				I2C_Read(&fd, out_buffer, 4);
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				/*HANDLE DATA*/
				for(i = 0; i < 4; i++)
				{
					
					temperature |= out_buffer[3-i];
					if(i!=3)
						temperature <<= 8;
				} 				
				
				
				/*CHECK FAULT*/
				if(fault_mask & temperature)
				{
					if(open_circuit_mask & temperature)
					{
						fputs("Fault: open circuit!\n",stdout);
						exit(1);
					}
					if(gnd_short_mask & temperature)
					{
						fputs("Fault: GND short!\n",stdout);
						exit(1);
					}
					if(vcc_short_mask & temperature)
					{
						fputs("Fault: VCC short!\n",stdout);
						exit(1);						
					}					
				}
				else
				{
					
					/*PROCESS INTERNAL TEMPERATURE*/
					foobar = temperature;
					foobar &= 0x0000FFFF;
					foobar >>= 4;
					
					internal = (foobar & 0x00F) * 0.0625 + ((foobar >> 4) & 0x7F);
					
					/*CHECK SIGN*/
					if(sign_int_mask & temperature)
					{
						/*NEGATIVE TEMPERATURE*/
						printf("INTERNAL TEMPERATURE: %.4f C\n",  -128+internal);
					}
					else
					{
						/*POSITIVE TEMPERATURE*/
				
						printf("INTERNAL TEMPERATURE: %c%.4f C\n", '+', internal);
					}
					
					
					/*PROCESS EXTERNAL TEMPERATURE*/
					foobar = temperature;
					foobar >>= 16;
					foobar >>= 2;
					
					external = (foobar & 0x0003) * 0.25 + ((foobar >> 2) & 0x7FF);
					
					if(sign_ext_mask & temperature)
					{
						/*NEGATIVE TEMPERATURE*/
						printf("EXTERNAL TEMPERATURE: %.2f C\n",  -2048+external);
					}
					else
					{
						/*POSITIVE TEMPERATURE*/
				
						printf("EXTERNAL TEMPERATURE: %c%.2f C\n", '+', external);
					}
					
				}
				
			}			
		}
		/*SET_TRIS COMMAND: -tris 0xA0 <data>*/
		else if(!strcmp(argv[1], "--tris") || !strcmp(argv[1], "-d"))
		{
			/*Check parameters*/
			if(argc != 4)
			{
				printf("Invalid number of parameters\n");
				exit(1);
			}
			else
			{
				int fd;
				unsigned char in_buffer[4];
				unsigned char temp_buffer[7];
				int address;
				int tris;
				int i;
				
				address	 =	 strtol(argv[2], NULL, 0);
				tris	 =	 strtol(argv[3], NULL, 0);
				
				
				in_buffer[0] = 0x01;	//MOD-TC-MK2 Identificator
				in_buffer[1] = address;
				in_buffer[2] = 0x01;	//SET_TRIS COMMAND
				in_buffer[3] = tris;
				
				
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 4);				
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				for(i = 0; i < 7; i++)
				{
					if(i == 3)
						continue;
					if(tris & (1 << i))
						temp_buffer[i] = 'I';
					else
						temp_buffer[i] = 'O';
				}
				
				printf("\n");
				printf("-------+---+---+---+---+---+---+----\n");
				printf("| GPIO | 6 | 5 | 4 | 3 | 2 | 1 | 0 |\n");
				printf("-------+---+---+---+---+---+---+----\n");
				printf("| TRIS | %c | %c | %c | %c | %c | %c | %c |\n",
					temp_buffer[6], temp_buffer[5], temp_buffer[4], 
					'I', temp_buffer[2], temp_buffer[1], 
					temp_buffer[0]);
				printf("-------+---+---+---+---+---+---+----\n");
				
			}
			
		}
		/*SET_LAT COMMAND: -lat 0xA0 <data>*/
		else if(!strcmp(argv[1], "-l") || !strcmp(argv[1], "--lat"))
		{
			/*Check parameters*/
			if(argc != 4)
			{
				printf("Invalid number of parameters\n");
				exit(1);
			}
			else
			{
				int fd;
				unsigned char in_buffer[4];
				unsigned char temp_buffer[7];
				int address;
				int lat;
				int i;
				
				address	 =	 strtol(argv[2], NULL, 0);
				lat	 =	 strtol(argv[3], NULL, 0);
				
				
				in_buffer[0] = 0x01;	//MOD-TC-MK2 Identificator
				in_buffer[1] = address;
				in_buffer[2] = 0x02;	//SET_LAT COMMAND
				in_buffer[3] = lat;
				
				
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 4);				
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				for(i = 0; i < 7; i++)
				{
					if(lat & (1 << i))
						temp_buffer[i] = 'H';
					else
						temp_buffer[i] = 'L';
				}
				
				printf("\n");
				printf("-------+---+---+---+---+---+---+----\n");
				printf("| GPIO | 6 | 5 | 4 | 3 | 2 | 1 | 0 |\n");
				printf("-------+---+---+---+---+---+---+----\n");
				printf("| LAT  | %c | %c | %c | %c | %c | %c | %c |\n",
					temp_buffer[6], temp_buffer[5], temp_buffer[4], 
					temp_buffer[3], temp_buffer[2], temp_buffer[1], 
					temp_buffer[0]);
				printf("-------+---+---+---+---+---+---+----\n");
				
			}
			
		}
		/*GET_PORT COMMAND: -port 0xA0 <data>*/
		else if(!strcmp(argv[1], "-p") || !strcmp(argv[1], "--port"))
		{
			/*Check parameters*/
			if(argc != 3)
			{
				printf("Invalid number of parameters\n");
				exit(1);
			}
			else
			{
				int fd;
				unsigned char in_buffer[3];
				unsigned char out_buffer[2];
				unsigned char temp_buffer[7];
				int address;
				int i;
				
				address	 =	 strtol(argv[2], NULL, 0);
				
				
				in_buffer[0] = 0x01;	//MOD-TC-MK2 Identificator
				in_buffer[1] = address;
				in_buffer[2] = 0x03;	//GET_PORT COMMAND
				
				
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 3);
				/*READ DATA*/				
				I2C_Read(&fd, out_buffer, 1);				
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				
				for(i = 0; i < 7; i++)
				{
					if(out_buffer[0] & (1 << i))
						temp_buffer[i] = 'H';
					else
						temp_buffer[i] = 'L';
				}
				
				printf("\n");
				printf("-------+---+---+---+---+---+---+----\n");
				printf("| GPIO | 6 | 5 | 4 | 3 | 2 | 1 | 0 |\n");
				printf("-------+---+---+---+---+---+---+----\n");
				printf("| PORT | %c | %c | %c | %c | %c | %c | %c |\n",
					temp_buffer[6], temp_buffer[5], temp_buffer[4], 
					temp_buffer[3], temp_buffer[2], temp_buffer[1], 
					temp_buffer[0]);
				printf("-------+---+---+---+---+---+---+----\n");
				
				
			}
			
		}
		/*SET_PU -pullup 0xA0 <data>*/
		else if(!strcmp(argv[1], "--pullup") || !strcmp(argv[1], "-u"))
		{
			/*Check parameters*/
			if(argc != 4)
			{
				printf("Invalid number of parameters\n");
				exit(1);
			}
			else
			{
				int fd;
				unsigned char in_buffer[4];
				unsigned char temp_buffer[7];
				int address;
				int pu;
				int i;
				
				address	 =	 strtol(argv[2], NULL, 0);
				pu	 =	 strtol(argv[3], NULL, 0);
				
				
				in_buffer[0] = 0x01;	//MOD-TC-MK2 Identificator
				in_buffer[1] = address;
				in_buffer[2] = 0x04;	//SET_PU COMMAND
				in_buffer[3] = pu;
				
				
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 4);				
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				for(i = 0; i < 5; i++)
				{
					if(pu & (1 << i))
						temp_buffer[i] = 'Y';
					else
						temp_buffer[i] = 'N';
				}
				
				printf("\n");
				printf("-------+---+---+---+---+---+---+----\n");
				printf("| GPIO | 6 | 5 | 4 | 3 | 2 | 1 | 0 |\n");
				printf("-------+---+---+---+---+---+---+----\n");
				printf("|  PU  | %c | %c | %c | %c | %c | %c | %c |\n",
					'-', '-',  temp_buffer[4], temp_buffer[3],
					 temp_buffer[2], temp_buffer[1], temp_buffer[0]);
				printf("-------+---+---+---+---+---+---+----\n");
				
			}
			
		}
		
		/*GET_AN0 -an0 0xA0 <data>*/
		else if(!strcmp(argv[1], "-an0") || !strcmp(argv[1], "--analog0"))
		{
			/*Check parameters*/
			if(argc != 3)
			{
				printf("Invalid number of parameters\n");
				exit(1);
			}
			else
			{
				int fd;
				unsigned char in_buffer[3];
				unsigned char out_buffer[2];
				unsigned char temp_buffer[40];
				int i;
				int address;
				int an0;
				float vcc;
				float t = 0;
				
				
				address	 =	 strtol(argv[2], NULL, 0);
				
				
				in_buffer[0] = 0x01;	//MOD-TC-MK2 Identificator
				in_buffer[1] = address;
				in_buffer[2] = 0x10;	//SET_PU COMMAND
				
				
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 3);	
				/*READ DATA*/
				I2C_Read(&fd, out_buffer, 2);			
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				an0 = out_buffer[0];
				an0 |= out_buffer[1]<<8;
				
				vcc = (an0/1023.0)*3.3;
				
				for(i = 0; i < 40; i++)
				{
					t = i*0.0825;
					if(vcc > t)
						temp_buffer[i] = '=';
					else
						temp_buffer[i] = '.';
					
				}
				
				temp_buffer[40] = 0;
				
				printf("\nAN0:[%s]%.3fV\n", temp_buffer, vcc);			
				
				
				
				
				
			}
			
		}
		/*GET_AN1 -an1 0xA0 <data>*/
		else if(!strcmp(argv[1], "-an1") || !strcmp(argv[1], "--analog1"))
		{
			/*Check parameters*/
			if(argc != 3)
			{
				printf("Invalid number of parameters\n");
				exit(1);
			}
			else
			{
				int fd;
				unsigned char in_buffer[3];
				unsigned char out_buffer[2];
				unsigned char temp_buffer[40];
				int i;
				int address;
				int an1;
				float vcc;
				float t = 0;
				
				
				address	 =	 strtol(argv[2], NULL, 0);
				
				
				in_buffer[0] = 0x01;	//MOD-TC-MK2 Identificator
				in_buffer[1] = address;
				in_buffer[2] = 0x11;	//SET_PU COMMAND
				
				
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 3);	
				/*READ DATA*/
				I2C_Read(&fd, out_buffer, 2);			
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				an1 = out_buffer[0];
				an1 |= out_buffer[1]<<8;
				
				vcc = (an1/1023.0)*3.3;
				
				for(i = 0; i < 40; i++)
				{
					t = i*0.0825;
					if(vcc > t)
						temp_buffer[i] = '=';
					else
						temp_buffer[i] = '.';
					
				}
				
				temp_buffer[40] = 0;
				
				printf("\nAN1:[%s]%.3fV\n", temp_buffer, vcc);			
				
				
				
				
				
			}
			
		}
		/*GET_AN2 -an2 0xA0 <data>*/
		else if(!strcmp(argv[1], "-an2") || !strcmp(argv[1], "--analog2"))
		{
			/*Check parameters*/
			if(argc != 3)
			{
				printf("Invalid number of parameters\n");
				exit(1);
			}
			else
			{
				int fd;
				unsigned char in_buffer[3];
				unsigned char out_buffer[2];
				unsigned char temp_buffer[40];
				int i;
				int address;
				int an2;
				float vcc;
				float t = 0;
				
				
				address	 =	 strtol(argv[2], NULL, 0);
				
				
				in_buffer[0] = 0x01;	//MOD-TC-MK2 Identificator
				in_buffer[1] = address;
				in_buffer[2] = 0x12;	//SET_PU COMMAND
				
				
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 3);	
				/*READ DATA*/
				I2C_Read(&fd, out_buffer, 2);			
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				an2 = out_buffer[0];
				an2 |= out_buffer[1]<<8;
				
				vcc = (an2/1023.0)*3.3;
				
				for(i = 0; i < 40; i++)
				{
					t = i*0.0825;
					if(vcc > t)
						temp_buffer[i] = '=';
					else
						temp_buffer[i] = '.';
					
				}
				
				temp_buffer[40] = 0;
				
				printf("\nAN2:[%s]%.3fV\n", temp_buffer, vcc);			
				
				
				
				
				
			}
			
		}
		/*GET_AN6 -an6 0xA0 <data>*/
		else if(!strcmp(argv[1], "-an6") || !strcmp(argv[1], "--analog6"))
		{
			/*Check parameters*/
			if(argc != 3)
			{
				printf("Invalid number of parameters\n");
				exit(1);
			}
			else
			{
				int fd;
				unsigned char in_buffer[3];
				unsigned char out_buffer[2];
				unsigned char temp_buffer[40];
				int i;
				int address;
				int an6;
				float vcc;
				float t = 0;
				
				
				address	 =	 strtol(argv[2], NULL, 0);
				
				
				in_buffer[0] = 0x01;	//MOD-TC-MK2 Identificator
				in_buffer[1] = address;
				in_buffer[2] = 0x16;	//SET_PU COMMAND
				
				
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 3);	
				/*READ DATA*/
				I2C_Read(&fd, out_buffer, 2);			
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				an6 = out_buffer[0];
				an6 |= out_buffer[1]<<8;
				
				vcc = (an6/1023.0)*3.3;
				
				for(i = 0; i < 40; i++)
				{
					t = i*0.0825;
					if(vcc > t)
						temp_buffer[i] = '=';
					else
						temp_buffer[i] = '.';
					
				}
				
				temp_buffer[40] = 0;
				
				printf("\nAN6:[%s]%.3fV\n", temp_buffer, vcc);			
				
				
				
				
				
			}
			
		}
		/*GET_AN7 -an7 0xA0 <data>*/
		else if(!strcmp(argv[1], "-an7") || !strcmp(argv[1], "--analog7"))
		{
			/*Check parameters*/
			if(argc != 3)
			{
				printf("Invalid number of parameters\n");
				exit(1);
			}
			else
			{
				int fd;
				unsigned char in_buffer[3];
				unsigned char out_buffer[2];
				unsigned char temp_buffer[40];
				int i;
				int address;
				int an7;
				float vcc;
				float t = 0;
				
				
				
				
				address	 =	 strtol(argv[2], NULL, 0);
				
				
				in_buffer[0] = 0x01;	//MOD-TC-MK2 Identificator
				in_buffer[1] = address;
				in_buffer[2] = 0x17;	//SET_PU COMMAND
				
				
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 3);	
				/*READ DATA*/
				I2C_Read(&fd, out_buffer, 2);			
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				an7 = out_buffer[0];
				an7 |= out_buffer[1]<<8;
				
				vcc = (an7/1023.0)*3.3;
				
				for(i = 0; i < 40; i++)
				{
					t = i*0.0825;
					if(vcc > t)
						temp_buffer[i] = '=';
					else
						temp_buffer[i] = '.';
					
				}
				
				temp_buffer[40] = 0;
				
				printf("\nAN7:[%s]%.3fV\n", temp_buffer, vcc);			
				
				
				
				
				
			}
			
		}
		/*GET_AN -analog 0xA0 <data>*/
		else if(!strcmp(argv[1], "--analog") || !strcmp(argv[1], "-a"))
		{
			/*Check parameters*/
			if(argc != 3)
			{
				printf("Invalid number of parameters\n");
				exit(1);
			}
			else
			{
				int fd;
				unsigned char in_buffer[3];
				unsigned char out_buffer[2];
				unsigned char temp_buffer[40];
				int i;
				int address;
				int an0, an1, an2, an6, an7;
				float vcc;
				float t = 0;
				
					
				address	 =	 strtol(argv[2], NULL, 0);
				
				in_buffer[0] = 0x01;	//MOD-TC-MK2 Identificator
				in_buffer[1] = address;
				in_buffer[2] = 0x10;	//SET_PU COMMAND
				
				
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 3);	
				/*READ DATA*/
				I2C_Read(&fd, out_buffer, 2);			
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				an0 = out_buffer[0];
				an0 |= out_buffer[1]<<8;
				
				vcc = (an0/1023.0)*3.3;
				
				for(i = 0; i < 40; i++)
				{
					t = i*0.0825;
					if(vcc > t)
						temp_buffer[i] = '=';
					else
						temp_buffer[i] = '.';
					
				}
				
				temp_buffer[40] = 0;
				
				printf("\nAN0:[%s]%.3fV\n", temp_buffer, vcc);	
				
							in_buffer[0] = 0x01;	//MOD-TC-MK2 Identificator
				in_buffer[1] = address;
				in_buffer[2] = 0x11;	//SET_PU COMMAND
				
				
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 3);	
				/*READ DATA*/
				I2C_Read(&fd, out_buffer, 2);			
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				an1 = out_buffer[0];
				an1 |= out_buffer[1]<<8;
				
				vcc = (an1/1023.0)*3.3;
				
				for(i = 0; i < 40; i++)
				{
					t = i*0.0825;
					if(vcc > t)
						temp_buffer[i] = '=';
					else
						temp_buffer[i] = '.';
					
				}
				
				temp_buffer[40] = 0;
				
				printf("AN1:[%s]%.3fV\n", temp_buffer, vcc);	
				
				
				in_buffer[0] = 0x01;	//MOD-TC-MK2 Identificator
				in_buffer[1] = address;
				in_buffer[2] = 0x12;	//SET_PU COMMAND
				
				
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 3);	
				/*READ DATA*/
				I2C_Read(&fd, out_buffer, 2);			
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				an2 = out_buffer[0];
				an2 |= out_buffer[1]<<8;
				
				vcc = (an2/1023.0)*3.3;
				
				for(i = 0; i < 40; i++)
				{
					t = i*0.0825;
					if(vcc > t)
						temp_buffer[i] = '=';
					else
						temp_buffer[i] = '.';
					
				}
				
				temp_buffer[40] = 0;
				
				printf("AN2:[%s]%.3fV\n", temp_buffer, vcc);	
				
				
				
				in_buffer[0] = 0x01;	//MOD-TC-MK2 Identificator
				in_buffer[1] = address;
				in_buffer[2] = 0x16;	//SET_PU COMMAND
				
				
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 3);	
				/*READ DATA*/
				I2C_Read(&fd, out_buffer, 2);			
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				an6 = out_buffer[0];
				an6 |= out_buffer[1]<<8;
				
				vcc = (an6/1023.0)*3.3;
				
				for(i = 0; i < 40; i++)
				{
					t = i*0.0825;
					if(vcc > t)
						temp_buffer[i] = '=';
					else
						temp_buffer[i] = '.';
					
				}
				
				temp_buffer[40] = 0;
				
				printf("AN6:[%s]%.3fV\n", temp_buffer, vcc);		
				
			
				
				
				in_buffer[0] = 0x01;	//MOD-TC-MK2 Identificator
				in_buffer[1] = address;
				in_buffer[2] = 0x17;	//SET_PU COMMAND
				
				
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 3);	
				/*READ DATA*/
				I2C_Read(&fd, out_buffer, 2);			
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				an7 = out_buffer[0];
				an7 |= out_buffer[1]<<8;
				
				vcc = (an7/1023.0)*3.3;
				
				for(i = 0; i < 40; i++)
				{
					t = i*0.0825;
					if(vcc > t)
						temp_buffer[i] = '=';
					else
						temp_buffer[i] = '.';
					
				}
				
				temp_buffer[40] = 0;
				
				printf("AN7:[%s]%.3fV\n", temp_buffer, vcc);				
				
				
			}
						
		}
		else if(!strcmp(argv[1], "-address"))
		{
			/*Check parameters*/
			if(argc != 3)
			{
				printf("Invalid number of parameters\n");
				exit(1);
			}
			else
			{
				int fd;
				unsigned char address;
				unsigned char in_buffer[5];
				
				address = strtol(argv[2], NULL, 0);
				
				in_buffer[0] = 0x01;
				in_buffer[1] = 0xF0;
				in_buffer[2] = 0xB0;
				in_buffer[3] = address;
				
				/*Opening I2C-BUS*/
				I2C_Open(&fd, 0x48);
				/*SEND DATA*/
				I2C_Send(&fd, in_buffer, 4);			
				/*Close I2C-BUS*/
				I2C_Close(&fd);
				
				printf("\nNEW ADDRESS: 0x%02X\n", address);				
				
												
			}
		}
		else
		{
			fputs("Unknown parameter. Type --help for more info\n", stdout);
			exit(0); 
		}
		
	}
	
	return 0;
}

