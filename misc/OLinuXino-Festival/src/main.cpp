/*
 * untitled.cxx
 * 
 * Copyright 2013 Stefan Mavrodiev <support@olimex.com>
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
 */


#include <iostream>
#include <iomanip>
#include "festival.h"
#include <getopt.h>

int main(int argc, char **argv)
{
    static int verbose_flag = 0;
    static int file_flag = 0;
    static int text_flag = 0;
    int c;
    
    
    int heap_size  = 210000;
    int load_init_files = 1;
    char file_name[200];
    char text[200];
    
    while(1){
        static struct option long_options[] = 
        {
            {"verbose", no_argument,    0,  'v'},
            {"heap",    required_argument,  0,  'h'},
            {"file",    required_argument,  0,  'f'},
            {"text",    required_argument,  0,  't'},
            {"no-init-files",   no_argument,    &load_init_files,   0},
            {0, 0,  0,  0}
        };
        
        int option_index = 0;
        
        c = getopt_long(argc, argv, "h:f:t:v", long_options, &option_index);
        
        if(c == -1)
            break;
            
        switch(c)
        {
            case 0:
                break;
                
            case 'h':
                heap_size = strtol(optarg, NULL, 10);
                break;
                
            case 'f':
                file_flag = 1;
                strcpy(file_name, optarg);
                break;
                
            case 't':
                text_flag = 1;
                strcpy(text, optarg);
                break;
                
            case 'v':
                verbose_flag = 1;
                break;
                
            case '?':
                break;
                
            default:
                break;
        }
        
    }
    if(verbose_flag){
        cout << "Load init files: ";
        if(load_init_files)
            cout << "yes" << endl;
        else
            cout << "no" << endl;
            
        cout << "Heap size: " << heap_size << endl;
        
        cout << "Using text file: ";
        if(file_flag)
            cout << file_name << endl;
        else
            cout << "none" << endl;
    }
    
    // Initialize festival
    if(verbose_flag)
        cout << "Initializating: ";
    festival_initialize(load_init_files, heap_size);
    if(verbose_flag)
        cout << "OK" << endl;
        
    
        
    //Load file
    if(file_flag){
        
        // Reading file
        if(verbose_flag)
            cout << "Reading file: ";
        if(!festival_say_file(file_name))
            return -1;
        if(verbose_flag)
            cout << "OK" << endl;
        
        // Waiting for spooler    
        if(verbose_flag)
            cout << "Waiting spooler: ";
        festival_wait_for_spooler();
        if(verbose_flag)
            cout << "OK" << endl;
        
        return 0;
    }   
    
    //Say text
    if(text_flag){
        if(verbose_flag)
            cout << "Saying: " << text << endl;
        if(!festival_say_text(text))
            return -1;
        return 0;
    }
	
	return 0;
}

