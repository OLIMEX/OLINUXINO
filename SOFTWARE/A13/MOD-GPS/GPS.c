//      GPS.c
//      
//      Copyright 2013 Stefan Mavrodiev <support@olimex.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.


#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <curses.h>

#include "uart.h"

//define some global variables

char port[30];

int main(int argc, char **argv)
{
    WINDOW *mainwin;
    WINDOW *win1, *win2, *win3, *win4;
    
    int fd;
    char data[100];
    char ch[2];
    int i;
    int j;
    char *str_ptr;
    char list_s[20][20];
    char c;
    int row, col;
    
    
    while(1)
    {
        static struct option long_options[]=
		{
			{"tty", required_argument, 0, 't'},
			{0,		0, 			0,		0}
		};
        
        int option_index = 0;
		int c = getopt_long(argc, argv, "t:", long_options, &option_index);
		
		if (c == -1)
		{
			break;
		}
		
		switch(c)
		{				
			case 't':
                strcpy(port, optarg);
                break;				
				
			case '?':
                return EXIT_FAILURE;
				break;
				
			default:
				break;
		}
    }
    
    //Read data, no escape from here
    OpenSerial(&fd, port, B19200);
    
    if ( (mainwin = initscr()) == NULL ) {
	fprintf(stderr, "Error initialising ncurses.\n");
	exit(EXIT_FAILURE);
    }
    
    noecho();
    cbreak();
	keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    getmaxyx(stdscr, row, col);


    win1 = subwin(mainwin, 3, col,  0, 0);
    box(win1, 0, 0);
    mvwaddstr(win1, 1, (col-strlen("A13 + MOD-GPS DEMO"))/2, "A13 + MOD-GPS DEMO");
    
    win2 = subwin(mainwin, 9, col, 3, 0);
    box(win2, 0, 0);    
    mvwaddstr(win2, 1, 3, "Date: ");
    mvwaddstr(win2, 2, 3, "Latitude:");
    mvwaddstr(win2, 3, 3, "Longitude:");
    mvwaddstr(win2, 4, 3, "Altitude:");
    mvwaddstr(win2, 5, 3, "Speed:");
    mvwaddstr(win2, 6, 3, "Heading:");
    mvwaddstr(win2, 7, 3, "Status:");
    
    mvwaddstr(win2, 1, 18, "n/a");
    mvwaddstr(win2, 2, 18, "n/a");
    mvwaddstr(win2, 3, 18, "n/a");
    mvwaddstr(win2, 4, 18, "n/a");
    mvwaddstr(win2, 5, 18, "n/a");
    mvwaddstr(win2, 6, 18, "n/a");
    mvwaddstr(win2, 7, 18, "n/a");
    
    win3 = subwin(mainwin, 3, col, 12, 0);
    box(win3, 0, 0);
    mvwaddstr(win3, 1, (col-strlen("CTRL+C - Exit"))/2, "CTRL+C - Exit");
    
    win4 = subwin(mainwin, 3, col, 15, 0);
    box(win4, 0, 0);
    
    refresh();
    
    while((c=getch()) != 'q')
    {
        
        i = 0;
        ch[1] = 0;
        
        //Read line
        while(1)
        {
            ReadSerial(&fd, ch, 1);
            if(ch[0] == 10)
                break;
            data[i++] = ch[0];
        }
        data[i-1] = 0;
        
        mvwprintw(win4, 1, 1, "Status: %s",data);
        wrefresh(win4);        
        
        //Split line
        str_ptr = strtok(data, ",");
        j = 0;
        while(str_ptr != NULL)
        {
            strcpy(list_s[j++], str_ptr);
            str_ptr = strtok(NULL, ",");
        }
        
        
        
        if(!strcmp("$GPRMC", list_s[0]))
        {
            char date[15];
            date[0] = list_s[9][0];
            date[1] = list_s[9][1];
            date[2] = '.';
            date[3] = list_s[9][2];
            date[4] = list_s[9][3];
            date[5] = '.';
            date[6] = list_s[9][4];
            date[7] = list_s[9][5];
            date[8] = 0;
            
            mvwaddstr(win2, 1, 18, date);
        }
                    
        else if(!strcmp("$GPVTG", list_s[0]))
        {
            mvwaddstr(win2, 5, 18, list_s[6]);
            mvwaddstr(win2, 6, 18, list_s[1]);
        }
        else if(!strcmp("$GPGSA", list_s[0]))
        {
            if(list_s[2][0] == '1')
                mvwaddstr(win2, 7, 18, "NO FIX");
            else if(list_s[2][0] == '2')
                mvwaddstr(win2, 7, 18, "2D FIX");
            else if(list_s[2][0] == '3')
                mvwaddstr(win2, 7, 18, "3D FIX");
        }
                
        else if(!strcmp("$GPGGA", list_s[0]))
        {
            double lon, lat;
            lon = strtod(list_s[2], (char **) NULL);
            lat = strtod(list_s[4], (char **) NULL);
            mvwprintw(win2, 2, 18, "%.6f %s", lon/100.0, list_s[3]);
            mvwprintw(win2, 3, 18, "%.6f %s", lat/100.0, list_s[5]);
            mvwprintw(win2, 4, 18, "%s %s",list_s[8], list_s[9]);
        }
        
        
        wrefresh(win2);
        sleep(1);
    }   
    
        
    
    
    CloseSerial(&fd);
    delwin(win1);
    delwin(win2);
    delwin(win3);
    delwin(win4);
    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;

}

