/* 
 * File:   main.c
 * Author: rbeal
 *
 * Created on February 15, 2017, 8:38 PM
 */

#include <stdio.h>
#include <stdlib.h>


// PIC18F44K22 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config FOSC = INTIO67   // Oscillator Selection bits (Internal oscillator block)
#pragma config PLLCFG = ON      // 4X PLL Enable (Oscillator multiplied by 4)
#pragma config PRICLKEN = OFF   // Primary clock enable bit (Primary clock can be disabled by software)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (Power up timer disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 190       // Brown Out Reset Voltage bits (VBOR set to 1.90 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (Watch dog timer is always disabled. SWDTEN has no effect.)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTB3  // CCP2 MUX bit (CCP2 input/output is multiplexed with RB3)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<5:0> pins are configured as digital I/O on Reset)
#pragma config CCP3MX = PORTE0  // P3A/CCP3 Mux bit (P3A/CCP3 input/output is mulitplexed with RE0)
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up (HFINTOSC output and ready status are not delayed by the oscillator stable status)
#pragma config T3CMX = PORTB5   // Timer3 Clock input mux bit (T3CKI is on RB5)
#pragma config P2BMX = PORTC0   // ECCP2 B output mux bit (P2B is on RC0)
#pragma config MCLRE = EXTMCLR  // MCLR Pin Enable bit (MCLR pin enabled, RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0 (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection Block 1 (Block 1 (002000-003FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (002000-003FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <pic18f44k22.h>

#include "d_uart.h"
#include "d_lora.h"
#include "d_spi.h"
#include "d_gps.h"

char soft_interrupt;

char gps_buffer_parsed[15][15];
char gps_buffer_line[90];
char gps_buffer_pointer;

void interrupt high_priority tc_int(void) // High priority interrupt
{
    char tmp;
    
    GIEH = 0;
    
    if (PIR1bits.RC1IF)
    {
        //gps_receivecar(RCREG1);
        PIR1bits.RC1IF = 0;
        
        tmp = RCREG1;
    }
    
    GIEH = 1;
}
 
void interrupt low_priority LowIsr(void) // Low priority interrupt
{
    char tmp;
    
    GIEL = 0;
    
    if (INTCONbits.TMR0IF) {

        INTCONbits.TMR0IF = 0;
    }
    
    if (PIR1bits.RC1IF)
    {
        //gps_receivecar(RCREG1);
        PIR1bits.RC1IF = 0;
        
        tmp = RCREG1;
    }
    
    GIEL = 1;
}

struct System_state
{
    char mode; // Rx=0, Tx=1, None=2
    char payload[255];
    int reapet_delay;
    char channel;
    char rf_mode;
    char src_addr;
    char dest_addr;
    char display_rx;
};

struct System_state state_struct;

void init();
void load_tab(char* toload, char* tab);

int main() {
    
    char i;
    
    init();
    
    uart_init();
    spi_init();
    gps_init();
    
    state_struct.mode = LORA_MODE_TX; // None mode
    *state_struct.payload = '\0';
    state_struct.reapet_delay = 500;
    state_struct.channel = 5;
    state_struct.rf_mode = 4;
    state_struct.src_addr = LORA_ID;
    state_struct.dest_addr = 0; // broadcast
    state_struct.display_rx = 0;
    
    LATD7 = 0;
    lora_init(state_struct.mode);

    GIEH = 1;
    GIEL = 1;
    
    T0CONbits.PSA = 0;
    T0CONbits.T08BIT = 0;
    T0CONbits.T0PS = 0b111;
    T0CONbits.TMR0ON = 0;
        
    
    
     
    while(1) {
        
        if (soft_interrupt & 0x01) { // GPS GPGSV !
            soft_interrupt &= 0xFE;
            
            load_tab(gps_buffer_line, state_struct.payload);
            lora_sendPacket(0, state_struct.payload);
            
        }
        

    }
    
    return 0;
}

void init() {
   
    OSCCONbits.IRCF = 0b111; // 16MHz
    OSCCONbits.SCS = 0b00; // Primary osc
    
    OSCTUNEbits.TUN = 0b011111;
    OSCTUNEbits.PLLEN = 1;
    
    
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    
    TRISDbits.RD7 = 0; // LED
    TRISCbits.RC2 = 1; // IRQ miniPic
    TRISCbits.RC0 = 0; // Transistor
    LATC0 = 0;
    
    RCONbits.IPEN = 1; // Enable levels interrupt
    soft_interrupt = 0;
}

void load_tab(char* toload, char* tab) {
    char i;
    
    for (i=0;*(toload+i) != '\0';i++)
        *(tab+i) = *(toload+i);
    
    
    *(tab+i) = '\0';
}