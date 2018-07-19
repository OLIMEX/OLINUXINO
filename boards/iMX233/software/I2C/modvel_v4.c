/*
 * Copyright © 2011 Jeroen De Schepper
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <modbus/modbus-tcp.h>
#include <math.h>
#include <pthread.h> 
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h> 
#include <sys/time.h>
#include <ncurses.h>

#define NB_CONNECTION    5

#define MAX_READ_BITS	150
#define MAX_WRITE_BITS	150
#define MAX_READ_REGS	150
#define MAX_WRITE_REGS	150

typedef unsigned char byte;
typedef struct {
			int chip_type;
			int addr;
			int mode;
			int inv_mode;
			int modbus_addr;
			}io_var;
io_var chip[44];

modbus_t *ctx = NULL;
modbus_mapping_t *mb_mapping;
char dev_name[40];
char def_file[18] ="/etc/mod_i2c.conf";
char conf_file[80];
int server_socket;
int port;
int nr_chips;
int max_val[65];
int deviceDescriptor;
int dum1,dum2,dum3, dum4;
unsigned long int start,end, total;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

byte Pcf8574code[]    = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};
byte Pcf8574Acode[]   = {0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F};
byte MAX127code[]     = {0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F};
byte Tda8444code[]    = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};
byte Pcf8591code[]    = {0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F};
byte MAX521code[]     = {0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F};
byte Pwmcode[]	      = {0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F};

/* Function pointer declarations */
void (*I2cSendData)(byte addr, byte *data, int len)=NULL;
void (*I2cReadData)(byte addr,byte *data,int len)=NULL;

inline unsigned long int time_msec ( void ) {
	struct timeval t; struct timezone tz;
	gettimeofday (&t,&tz);
	return (1000*t.tv_sec)+(t.tv_usec/1000);
}

void chomp(const char *s)
{
	char *p;
	while (NULL != s && NULL != (p = strrchr(s, '\n')))
	{
		*p = '\0';
	}
}

void BadFormat(void)
{
    fprintf(stderr, "Bad program format.\n");
    exit(-1);
}

void Read_config(char *fileName)
{
	char line[80];
	int i;
	i=0;

	FILE *f = fopen(fileName, "r");

	if(!f) 
	{
        	fprintf(stderr, "couldn't open file: %s\n",fileName);
	        exit(-1);
	}

	if(!fgets(line, sizeof(line), f)) BadFormat();
	if(strcmp(line, "$$chips\n")!=0) BadFormat();

	while(fgets(line, sizeof line, f))
	{
		if(strcmp(line, "$$settings\n")==0) break;
		if(sscanf(line, "%d,%d,%d,%d,%d", 	&chip[i].chip_type, 
							&chip[i].addr,
							&chip[i].mode,
							&chip[i].inv_mode,
							&chip[i].modbus_addr)==5)
		{
			++i;
		}
	}

	nr_chips = i;


	while(fgets(line, sizeof(line), f)) 
	{
		if(memcmp(line, "device=", 7)==0) 
		{
			memcpy(dev_name,(line+7),38);
	     		chomp(dev_name);
        	}

		if(memcmp(line, "port=", 5)==0) 
		{
            		port = atoi(line+5);
        	}
	}


	fclose(f);
}

static void close_sigint(int dummy)
{
    close(server_socket);
    modbus_free(ctx);
    modbus_mapping_free(mb_mapping);
    endwin();
    exit(dummy);
}

void I2cSendData_device(byte addr,byte *data,int len)
{
	int result=0;
	if(ioctl(deviceDescriptor,I2C_SLAVE, addr))
		printf("I2cSendData_device : IOCTL Problem");
	
	result=write(deviceDescriptor,data,len);
}

void I2cReadData_device(byte addr,byte *data,int len)
{
	int result=0;
	if(ioctl(deviceDescriptor,I2C_SLAVE, addr))
		printf("I2cReadData_device : IOCTL Problem");

	result=read(deviceDescriptor,data,len);
}

void init_i2c(char *DeviceName)
{
	I2cSendData=I2cSendData_device;
	I2cReadData=I2cReadData_device;
	deviceDescriptor=open(DeviceName, O_RDWR);
	if (deviceDescriptor == -1)
	{
		printf("Error opening device '%s'\n",DeviceName);
		exit(-1);
	}
}

void PWM(int chip_addr, int modb_addr)
{
	byte total_data[2];
	int i;
//output
	// set direction
	total_data[0] = 0x02;
	total_data[1] = mb_mapping->tab_bits[modb_addr];
	I2cSendData(chip_addr,total_data,2);
	dum3 = total_data[1];
	
	// set speed
	total_data[0] = 0x01;
	total_data[1] = (byte) (0xFF & (mb_mapping->tab_registers[modb_addr]));
	I2cSendData(chip_addr,total_data,2);
	dum2 = total_data[1];
//input
	// read analog input
	total_data[0] = 0x03;
	I2cSendData(chip_addr,total_data,1);
	I2cReadData(chip_addr,total_data,1);
	mb_mapping->tab_input_registers[modb_addr] = total_data[0];
	dum1 = total_data[0];

	// read status bits
	total_data[0] = 0x06;
	I2cSendData(chip_addr,total_data,1);
	I2cReadData(chip_addr,total_data,1);
	dum4 = total_data[0];
	for (i=0;i<=7;i++)
	{
		mb_mapping->tab_input_bits[i+modb_addr] = ((total_data[0] & (0x01 << i)) != 0);
	}
}

void Max127(int chip_addr, int mode, int modb_addr)
{
	byte total_data_an[2];
	int i;
	for(i=0;i<=7;i++)
	{
		total_data_an[0] = (byte) ( (0x80 | ((i * 0x10) & 0x70)) + (0xc & ((byte) (mode * 0x4))));
		I2cSendData(chip_addr,total_data_an,1);
		I2cReadData(chip_addr,total_data_an,2);
		mb_mapping->tab_input_registers[i+modb_addr] = (total_data_an[0] * 0x10) + (total_data_an[1] / 0x10);
	}
}

void Pcf8574(int chip_addr, int mode, int inv, int modb_addr)
{
	int i;
	byte data = 0;
	byte data2;
	if (mode != 0)
	{//input
		data2=0xff;
		I2cSendData(chip_addr,&data2,1);
		I2cReadData(chip_addr,&data,1);
		if (inv) data2 = ~data;
		   else data2 = data;

		for (i=0;i<=7;i++)
		{
			mb_mapping->tab_input_bits[i+modb_addr] = ((data2 & (0x01 << i)) != 0);
		}
	}
	else
	{//output
		for(i=0;i<=7;i++)
		{
			data += mb_mapping->tab_bits[i+modb_addr] * (pow(2,i));
		}
		if (inv) data2 = ~data;
		   else data2 = data;

		I2cSendData(chip_addr,&data2,1);
	}
}

void Tda8444(int chip_addr, int modb_addr)
{
	byte total_data[2];
	int i;
	byte data;
	for(i=0;i<=7;i++)
	{
		data = (byte) (0x3F & (mb_mapping->tab_registers[modb_addr+i]));
		total_data[0] = (byte) (0xF0 | (byte) (i));
		total_data[1] = (byte) (data > 63) ? 63: data;
		I2cSendData(chip_addr,total_data,2);
	}
}

void Pcf8591(int chip_addr, int mode, int modb_addr)
{
	int i;
	int count;
//output
	byte total_data[2];
	total_data[0] = (0x40) | ((byte) ((mode * 0x10) & 0x30));
	total_data[1] = (byte) (0xFF & (mb_mapping->tab_registers[modb_addr]));
	I2cSendData(chip_addr,total_data,2);

//input
	switch(mode)
	{
		case 0 : count=3;
		 break;
		case 1 : count=2;
		 break;
		case 2 : count=2;
		 break;
		case 3 : count=1;
		 break;
		default : count=3;
		 break;
	}
	for(i=0;i<=count;i++)
	{
		total_data[0] = (0x40) | ((byte) ((mode * 0x10) & 0x30)) | i;
		I2cSendData(chip_addr,total_data,1);
		I2cReadData(chip_addr,total_data,2);
		mb_mapping->tab_input_registers[i+modb_addr] = total_data[1];
	}
		
}

void Max521(int chip_addr, int modb_addr)
{
	byte total_data[2];
	int i;
	byte data;
	for(i=0;i<=7;i++)
	{
		data = (byte) (0xFF & (mb_mapping->tab_registers[modb_addr+i]));
		total_data[0] = (byte) (i);
		total_data[1] = (byte) (data > 255) ? 255: data;
		I2cSendData(chip_addr,total_data,2);
	}
}

void *read_io()
{
	int i;
	
	while(1)
	{
		start = time_msec();
		for(i=0;i<nr_chips;i++)
		{
			switch( chip[i].chip_type )
     			{
        		case 0 : Pcf8574(	Pcf8574code[chip[i].addr],
							chip[i].mode,
							chip[i].inv_mode,
							chip[i].modbus_addr);
							break;
				case 1 : Pcf8574(	Pcf8574Acode[chip[i].addr],
							chip[i].mode,
							chip[i].inv_mode,
							chip[i].modbus_addr);
							break;
				case 2 : Max127(	MAX127code[chip[i].addr],
							chip[i].mode,
							chip[i].modbus_addr);
							break;
				case 3 : Tda8444(	Tda8444code[chip[i].addr], 
							chip[i].modbus_addr);
							break;
				case 4 : Pcf8591(	Pcf8591code[chip[i].addr], 
							chip[i].mode, 
							chip[i].modbus_addr);
							break;
				case 5 : Max521(	MAX521code[chip[i].addr], 
							chip[i].modbus_addr);
							break;
				case 6 : PWM(	    Pwmcode[chip[i].addr], 
							chip[i].modbus_addr);
							break;
				default : break;
			}
		}
		end = time_msec();
		total = end - start;
	}
}

void *display_data()
{
//initscr();
while(1)
{
	mvprintw(0,0,"tijd: %5i",total);
	mvprintw(1,0,"%2X %2X %2X %2X",dum1,dum2,dum3,dum4);
	refresh();
	sleep(1);
}
//endwin();
}

int main(int argc, char **argv)
{
	int master_socket;
	int rc;
    
	fd_set refset;
	fd_set rdset;
	pthread_t thread1,thread2;
	int  iret1,iret2;

	if(argc < 2) 
	{
		sscanf(def_file,"%s",conf_file);
	}
	else
	{
		sscanf(argv[1],"%s",conf_file);
	}

	Read_config(conf_file);
	init_i2c(dev_name);

	/* Maximum file descriptor number */
	int fdmax;

	ctx = modbus_new_tcp("127.0.0.1", port);
	//modbus_set_debug(ctx, 1); 

	mb_mapping = modbus_mapping_new(MAX_READ_BITS, MAX_WRITE_BITS,
	                                MAX_READ_REGS, MAX_WRITE_REGS);
    
	if (mb_mapping == NULL) 
	{
	        fprintf(stderr, "Failed to allocate the mapping: %s\n",
                modbus_strerror(errno));
	        modbus_free(ctx);
	        return -1;
	}

	server_socket = modbus_tcp_listen(ctx, NB_CONNECTION);

	signal(SIGINT, close_sigint);

	/* Clear the reference set of socket */
	FD_ZERO(&refset);
	/* Add the server socket */
	FD_SET(server_socket, &refset);

	/* Keep track of the max file descriptor */
	fdmax = server_socket;

	/*start i/o thread*/
initscr();
	iret2 = pthread_create( &thread2, NULL, display_data, NULL);
	iret1 = pthread_create( &thread1, NULL, read_io, NULL);

	for (;;) 
	{
        	rdset = refset;
	        if (select(fdmax+1, &rdset, NULL, NULL, NULL) == -1) 
		{
			perror("Server select() failure.");
			close_sigint(1);
		}
	
		/* Run through the existing connections looking for data to be
		 * read */
		for (master_socket = 0; master_socket <= fdmax; master_socket++) 
		{
			if (FD_ISSET(master_socket, &rdset)) 
			{
				if (master_socket == server_socket) 
				{
					/* A client is asking a new connection */
					socklen_t addrlen;
					struct sockaddr_in clientaddr;
					int newfd;

					/* Handle new connections */
					addrlen = sizeof(clientaddr);
					memset(&clientaddr, 0, sizeof(clientaddr));
					newfd = accept(server_socket, (struct sockaddr *)&clientaddr, &addrlen);
					if (newfd == -1) 
					{
						perror("Server accept() error");
					} 
					else 
					{
						FD_SET(newfd, &refset);

						if (newfd > fdmax) 
						{
							/* Keep track of the maximum */
							fdmax = newfd;
						}
						//printf("New connection from %s:%d on socket %d\n",
						//inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port, newfd);
					}
				} 
				else 
				{
					/* An already connected master has sent a new query */
					uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
					//pthread_mutex_lock( &mutex1 );
					rc = modbus_receive(ctx, master_socket, query);
					//pthread_mutex_unlock( &mutex1 );
					if (rc != -1) 
					{
						//pthread_mutex_lock( &mutex1 );
						modbus_reply(ctx, query, rc, mb_mapping);
						//pthread_mutex_unlock( &mutex1 );
					} 
					else 
					{
						/* Connection closed by the client, end of server */
						//printf("Connection closed on socket %d\n", master_socket);
						close(master_socket);

						/* Remove from reference set */
						FD_CLR(master_socket, &refset);

						if (master_socket == fdmax) 
						{
							fdmax--;
						}
					}
				}
			}
		}
	}

	pthread_join( thread2, NULL);
	pthread_join( thread1, NULL);
	endwin();
	return 0;
}
