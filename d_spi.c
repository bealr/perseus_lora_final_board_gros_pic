/*
 * File:   main.c
 * Author: BEAL Romain
 *
 * Created on 2016, 12:38
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include "d_spi.h"

/**
  * @desc Send SPI byte to buffer
  * @param unsigned char : byte to send
  * @return none
 **/
void spi_w8b(char byte) {
    
    PIR3bits.SSP2IF = 0;       // clear interrupt flag
    SSP2BUF = byte;            // send byte
    while(!PIR3bits.SSP2IF) ;  // Wait until flag set
}

/**
  * @desc Read SPI byte from buffer
  * @param none
  * @return char : buffer content
 **/
char spi_r8b() {

    PIR3bits.SSP2IF = 0;
    SSP2BUF = 0x00;
    while(!PIR3bits.SSP2IF) ;
    return SSP2BUF; // get byte
}

/**
  * @desc SPI module init
  * @param none
  * @return none
 **/
void spi_init() {
            
    // Set directions
    TRISDbits.TRISD0 = 0;   // CLK
    TRISDbits.TRISD1 = 1;   // SDI
    TRISDbits.TRISD4 = 0;   // SDO
    TRISDbits.TRISD5 = 0;   // CS_lora
    
    LATD5 = 1; // CS_lora

    // --------------
    // 0011 = SPI Master mode, clock = TMR2 output/2  [ ]
    // 0010 = SPI Master mode, clock = FOSC/64        [#]
    // 0001 = SPI Master mode, clock = FOSC/16        [ ]
    // 0000 = SPI Master mode, clock = FOSC/4         [ ]
    // *
    SSP2CON1bits.SSPM0 = 1;
    SSP2CON1bits.SSPM1 = 0;
    SSP2CON1bits.SSPM2 = 0;
    SSP2CON1bits.SSPM3 = 0;
    // --------------

    // SPI MODE
    SSP2CON1bits.CKP=0; // Idle state for clock is a low level
    SSP2STATbits.CKE=1; // Transmit occurs on transition from active to Idle clock state
    
    SSP2CON1bits.SSPEN2 = 1; // start SPI

    SSP2IF=0; // clear interrupt flag
    
    
}
