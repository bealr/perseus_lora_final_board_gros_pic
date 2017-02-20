/*
 * File:   main.c
 * Author: BEAL Romain
 *
 * Created on 2016, 12:38
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include "d_gps.h"

extern char gps_buffer_parsed[15][15];
extern char gps_buffer_line[90];
extern char gps_buffer_pointer;
extern char soft_interrupt;

void gps_init() {
    
    char i;
    
    gps_buffer_pointer = 0;
    *gps_buffer_line   = 0;
    
    for (i=0;i<10;i++)
        gps_buffer_parsed[i][0] = 0;
}


void gps_receivecar(char car) {
    
    char trame_ok = 0;
    
    *(gps_buffer_line + gps_buffer_pointer) = car;
    gps_buffer_pointer++;
    
    
    if (*(gps_buffer_line + gps_buffer_pointer -1) == 13 && *(gps_buffer_line + gps_buffer_pointer) == 10) {
					*(gps_buffer_line + gps_buffer_pointer -1) = '\0';
					*(gps_buffer_line + gps_buffer_pointer)    = '\0';
                    
                    trame_ok = 1;
                    gps_buffer_pointer = 0;
                    LATC0 =~ LATC0;
	}
    
    if (trame_ok) {
        if ( 
					*(gps_buffer_line + 0) == '$' &&
					*(gps_buffer_line + 1) == 'G' &&
					*(gps_buffer_line + 2) == 'P' &&
					*(gps_buffer_line + 3) == 'G' &&
					*(gps_buffer_line + 4) == 'S' &&
					*(gps_buffer_line + 5) == 'V') {
					
                    soft_interrupt |= 0x01;
				  }
        
        //gps_parser(gps_buffer_parsed, gps_buffer_line);
    }
}


void gps_parser(char** buffer_out, char* buffer_in) {
    
    int i;
	int j=0;
	int k=0;
	
	for (i=0;*(buffer_in+i) != '\0';i++,k++) {
	
		if (*(buffer_in+i) == ',') {
			buffer_out[j][k] = '\0';
			j++;
			i++;
			k=0;
			while (*(buffer_in+i) == ',') {
				i++;
				j++;
			}
		}
		
		buffer_out[j][k] = *(buffer_in+i);
		if (buffer_out[j][k] == '*')
			buffer_out[j][k] = '\0';
	}
}