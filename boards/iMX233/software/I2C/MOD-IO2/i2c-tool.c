/*
 This software uses a BSD license.

Copyright (c) 2010, Sean Cross / chumby industries
All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the
   distribution.  
 * Neither the name of Sean Cross / chumby industries nor the names
   of its contributors may be used to endorse or promote products
   derived from this software without specific prior written
   permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 */
 
#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>





int main(int argc, char **argv) 
{

        int file;
		char *buffer_in, *buffer_out;
		int num_bytes_read,num_bytes_write, address, i; 
		char device[] = "/dev/i2c-0";

		
		if(argc < 2)
		{
			fprintf(stdout, "Invalid number of parameters\n");
			exit(1);
		}
		
		
		if(!strcmp(argv[1], "-w") || !strcmp(argv[1], "w"))
		{
				
				
			num_bytes_write = strtol(argv[4], NULL, 0);
			address = strtol(argv[3], NULL, 0);
			
			if(argc != num_bytes_write + 5)
			{
				fprintf(stdout, "Invalid number of parameters\n");
				exit(1);
			}
			
			if(address < 0x03 || address > 0x77)
			{
				fprintf(stdout, "Slave address out of range! [0x03..0x77]\n");
				exit(1);
			}
			fprintf(stdout, "SLAVE ADDRESS: 0x%02X\n", (unsigned int) address);
			fflush(stdout);
			
			if(num_bytes_write > 255 || num_bytes_write < 1)
			{
				fprintf(stdout, "Invalid number of bytes to write\n");
				exit(1);
			}
			
			fprintf(stdout, "NUMBER OF BYTES TO WRITE: %d\n", (unsigned int) num_bytes_write);
			fflush(stdout);
			
			buffer_out = (char*) malloc(num_bytes_write * sizeof(char));
		
			if(buffer_out == NULL)
			{
				fprintf(stdout, "Out of memory\n");
				exit(1);
			}
			fprintf(stdout, "MEMORY ALLOCATED AT ADDRESS: 0x%X\n", buffer_out);
			fflush(stdout);	
			
			for ( i = 0; i < num_bytes_write; i++)
			{
				buffer_out[i] = strtol(argv[5+i],  NULL, 0);
			}
						
			device[9] = argv[2][0];
			
			if ((file = open(device, O_RDWR)) < 0)
			{
				fprintf(stdout, "%s --> ", device);
				perror("Failed to open control file");
				exit(1);
			}
			
			fprintf(stdout, "%s OPENDED!\n", device);
			fflush(stdout);
			
			if(ioctl(file, I2C_SLAVE, address) < 0)
			{
				perror("Failed to access the bus");
				exit(1);
			}
			
			if(write(file, buffer_out, num_bytes_write) != num_bytes_write)
			{
				perror("Failed writing to the I2C-bus");
				exit(1);
			}
			else
			{
				
				printf("WRITE:SUCCESS\n"); 						
				
			}
				
					
			free(buffer_out);
			close(file);	
		}
		else if(!strcmp(argv[1], "-r") || !strcmp(argv[1], "r"))
		{
				if(argc != 5)
				{
					fprintf(stdout, "Invalid number of parameters\n");
					exit(1);
				}
				
				num_bytes_read = strtol(argv[4], NULL, 0);
				address = strtol(argv[3], NULL, 0);
				
				if(address < 0x03 || address > 0x77)
				{
					fprintf(stdout, "Slave address out of range! [0x03..0x77]\n");
					exit(1);
				}
				
				fprintf(stdout, "SLAVE ADDRESS: 0x%02X\n", (unsigned int) address);
				fflush(stdout);
				
				if(num_bytes_read > 255 || num_bytes_read < 1)
				{
					fprintf(stdout, "Invalid number of bytes to read\n");
					exit(1);
				}
				
				fprintf(stdout, "NUMBER OF BYTES TO READ: %d\n", (unsigned int) num_bytes_read);
				fflush(stdout);
				
				buffer_in = (char*) malloc(num_bytes_read * sizeof(char));
		
				if(buffer_in == NULL)
				{
					fprintf(stdout, "Out of memory\n");
					exit(1);
				}	
					
				fprintf(stdout, "MEMORY ALLOCATED AT ADDRESS: 0x%X\n", buffer_in);
				fflush(stdout);	
				
				device[9] = argv[2][0];
				if ((file = open(device, O_RDWR)) < 0)
				{
					fprintf(stdout, "%s --> ", device);
					perror("Failed to open control file");
					exit(1);
				}
				
				fprintf(stdout, "%s OPENDED!\n", device);
				fflush(stdout);
				
				if(ioctl(file, I2C_SLAVE, address) < 0)
				{
					perror("Failed to access the bus");
					exit(1);
				}
				
				if(read(file, buffer_in, num_bytes_read) != num_bytes_read)
				{
					perror("Failed reading from the I2C-bus");
				}
				else
				{
					for(i = 0; i < num_bytes_read; i++)
					{
						printf("BYTE[%d]: 0x%02X\n", i,(unsigned int)buffer_in[i]); 						
					}
				}
					
					
			free(buffer_in);
			close(file);	
		}
		else if(!strcmp(argv[1], "-rw") || !strcmp(argv[1], "rw"))
		{
			
			
			if(argc < 7)
			{
				fprintf(stdout, "Invalid number of parameters\n");
				exit(1);
			}
					
			address = strtol(argv[3], NULL, 0);
			num_bytes_write = strtol(argv[5], NULL, 0);
			num_bytes_read = strtol(argv[4], NULL, 0);
			
			if(argc != num_bytes_write + 6)
			{
				fprintf(stdout, "Invalid number of parameters\n");
				exit(1);
			}
			fprintf(stdout, "Number of parameters: %d\n", argc);
			fflush(stdout);
			
			
			if(address < 0x03 || address > 0x77)
			{
				fprintf(stdout, "Slave address out of range! [0x03..0x77]\n");
				exit(1);
			}
			fprintf(stdout, "SLAVE ADDRESS: 0x%02X\n", (unsigned int) address);
			fflush(stdout);
			
			
			
			if(!num_bytes_read)
			{
			
				struct i2c_rdwr_ioctl_data packets;
				struct i2c_msg messages[1];
				int user;
				unsigned long funcs;
				
				if(num_bytes_write > 255 || num_bytes_write < 1)
				{
					fprintf(stdout, "Invalid number of bytes to write\n");
					exit(1);
				}
				fprintf(stdout, "NUMBER OF BYTES TO READ: %d\n", (unsigned int) num_bytes_read);
				fflush(stdout);
				fprintf(stdout, "NUMBER OF BYTES TO WRITE: %d\n", (unsigned int) num_bytes_write);
				fflush(stdout);
				
				device[9] = argv[2][0];
				if ((file = open(device, O_RDWR)) < 0)
				{
					fprintf(stdout, "%s --> ", device);
					perror("Failed to open control file");
					exit(1);
				}
				fprintf(stdout, "%s OPENDED!\n", device);
				fflush(stdout);	

				user = ioctl(file,I2C_FUNCS, &funcs);
				fprintf(stdout, "FUNCTIONALITY: 0x%X\n", (unsigned int)funcs);
			
				
				
				buffer_out = (char*) malloc(num_bytes_write * sizeof(char));
		
				if(buffer_out == NULL)
				{
					fprintf(stdout, "Out of memory\n");
					exit(1);
				}
				fprintf(stdout, "MEMORY ALLOCATED AT ADDRESS: 0x%X\n", buffer_out);
				fflush(stdout);	
			
				for ( i = 0; i < num_bytes_write; i++)
				{
					buffer_out[i] = strtol(argv[6+i], NULL, 0);
				}

				messages[0].addr  = address;
				messages[0].flags = 0;
				messages[0].len   = num_bytes_write;
				messages[0].buf   = buffer_out;	

				packets.msgs  = messages;
				packets.nmsgs = 1;
				
				fprintf(stdout, "\nPACKET DATA: \
								\n-------------------- \
								\nADDRESS: 0x%02X \
								\nFLAG: 0x%02X \
								\nLENGHT: 0x%02X \
								\nMESSAGES: %d\n",
								(unsigned int)address, 
								(unsigned int)messages[0].flags,
								(unsigned int)sizeof(num_bytes_write),
								packets.nmsgs);
				fflush(stdout);
				if(ioctl(file, I2C_RDWR, &packets) < 0)
				{
					perror("Error sending data");
					return 1;
				}			
				
				printf("SENDING:DONE\n");
				
				free(buffer_out);
				close(file);
				
			}
			else
			{
				struct i2c_rdwr_ioctl_data packets;
				struct i2c_msg messages[2];
				
				if(num_bytes_write > 255 || num_bytes_write < 1)
				{
					fprintf(stdout, "Invalid number of bytes to write\n");
					exit(1);
				}
				
				
				if(num_bytes_read > 255)
				{
					fprintf(stdout, "Invalid number of bytes to read\n");
					exit(1);
				}
				
				fprintf(stdout, "NUMBER OF BYTES TO READ: %d\n", (unsigned int) num_bytes_read);
				fflush(stdout);
				fprintf(stdout, "NUMBER OF BYTES TO WRITE: %d\n", (unsigned int) num_bytes_write);
				fflush(stdout);
				
				device[9] = argv[2][0];
				if ((file = open(device, O_RDWR)) < 0)
				{
					fprintf(stdout, "%s --> ", device);
					perror("Failed to open control file");
					exit(1);
				}
				fprintf(stdout, "%s OPENDED!\n", device);
				fflush(stdout);	
				
				
				
				
				buffer_out = (char*)malloc(num_bytes_write * sizeof(char));
				buffer_in = (char*)malloc(num_bytes_read * sizeof(char));
				
							
				if(buffer_out == NULL || buffer_in == NULL)
				{
					fprintf(stdout, "Out of memory\n");
					exit(1);
				}
				
				fprintf(stdout, "MEMORY ALLOCATED AT ADDRESS: 0x%X\n", buffer_out);
				fflush(stdout);	
				fprintf(stdout, "MEMORY ALLOCATED AT ADDRESS: 0x%X\n", buffer_in);
				fflush(stdout);	
			
				for ( i = 0; i < num_bytes_write; i++)
				{
					buffer_out[i] = strtol(argv[6+i], NULL, 0);
				}

				messages[0].addr  = address;
				messages[0].flags = 0;
				messages[0].len   = sizeof(buffer_out);
				messages[0].buf   = buffer_out; 
				
				
				messages[1].addr  = address;
				messages[1].flags = I2C_M_RD;
				messages[1].len   = sizeof(buffer_in);
				messages[1].buf   = buffer_in;
				
				
				packets.msgs  = messages;
				packets.nmsgs = 2;
				
				fprintf(stdout, "\nPACKET DATA: \
								\n-------------------- \
								\nADDRESS: 0x%02X \
								\nFLAG: 0x%02X \
								\nLENGHT: 0x%02X \
								\n\nADDRESS: 0x%02X \
								\nFLAG: 0x%02X \
								\nLENGHT: 0x%02X \
								\nMESSAGES: %d\n",
								(unsigned int)messages[0].addr,
								(unsigned int)messages[0].flags,
								(unsigned int)sizeof(num_bytes_write),
								(unsigned int)messages[1].addr,
								(unsigned int)messages[1].flags,
								(unsigned int)sizeof(num_bytes_read),
								packets.nmsgs);
				fflush(stdout);
				
				if(ioctl(file, I2C_RDWR, &packets) < 0)
				{
					perror("Error sending data");
					exit(1);
				}	
				printf("SENDING:DONE\n");
				
				for(i = 0; i <num_bytes_read; i++)
				{
					printf("BYTE[%d]: 0x%02X\n", i,(unsigned int)buffer_in[i]);
					
				}
				
				free(buffer_out);
				free(buffer_in);
				close(file);
				
			}
			
		
		}
		else if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--h") || !strcmp(argv[1], "-help") || !strcmp(argv[1], "h"))
		{
			printf("\nUSAGE:\t%s -r I2CBUS ADDRESS [NUMBER OF BYTES]\n", argv[0]);
			printf("\t%s -w I2CBUS ADDRESS [NUMBER OF BYTES] [DATA0] [DATA1] ...\n", argv[0]);
			printf("\t%s -rw I2CBUS ADDRESS [BYTES TO READ] [BYTES TO WRITE] [DATA0] [DATA1] ...\n\n", argv[0]);
			
			printf("\t -r\tRead <n> bytes from specific address\n");
			printf("\t -w\tWrite <n> bytes to specific address\n");
			printf("\t -rw\tFirst write bytes to an address, followed by restart condition and read of <n> bytes\n\n"); 
		}
		else
		{
			fprintf(stdout, "Invalid operator \"%s\"\n", argv[1]);
			exit(1);
		}

	
    return 0;
}
