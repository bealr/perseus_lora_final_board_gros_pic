/* 
 * File:   d_gps.h
 * Author: rbeal
 *
 * Created on February 19, 2017, 10:07 PM
 */

#ifndef D_GPS_H
#define	D_GPS_H

void gps_init();
void gps_receivecar(char car);
void gps_parser(char** buffer_out, char* buffer_in);

#endif	/* D_GPS_H */

