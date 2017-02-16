/* 
 * File:   SPI.h
 * Author: BEAL Romain
 *
 * Created on 17 janvier 2015, 13:14
 */

#ifndef SPI_H
#define	SPI_H

void spi_init();
void spi_w8b(char byte);
char spi_r8b();

#endif	/* SPI_H */

