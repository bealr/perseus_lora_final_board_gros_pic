/*
 * File:   main.c
 * Author: BEAL Romain
 *
 * Created on 2016, 12:38
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include "d_lora.h"
#include "d_spi.h"
#include "d_uart.h"

char timeout_var=0;

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
    
extern struct System_state state_struct;

/**
  * @desc Init LoRa module
  * @param char : Rx or Tx mode (read the header file)
  * @return none
 **/
void lora_init(char mode) {
    
    char i;
    
    // Set directions
    TRISDbits.TRISD3 = 0; // lora_rx_sw
    TRISDbits.TRISD2 = 0; // lora_tx_sw
    TRISDbits.TRISD6 = 0; // lora_reset
    
    lora_rf_switch(LORA_TX_MODE);
    
    LATD6 = 1; // reset Y
    __delay_ms(100);
    LATD6 = 0; // Reset N
    
    /* SELF TEST LORA MODULE
     * Blinks led if LoRa chip works
     * */
    LATD7 = 1;
    if (lora_spi_read(REG_VERSION) == 0x22) {
        
        for (i=0;i<7;i++) {
            LATD7 = ~LATD7;
            __delay_ms(20);
        }
        LATD7 = 1;
    }
    
    lora_setMaxCurrent(0x1B); // 0x1B = 100mA
    lora_setLoraMode();       // Explicit ;-)
    
    // Default config
    lora_spi_write(0x0,0x0);
    lora_spi_write(0x1,0x81);
    lora_spi_write(0x2,0x1A);
    lora_spi_write(0x3,0xB);
    lora_spi_write(0x4,0x0);
    lora_spi_write(0x5,0x52);
    lora_spi_write(0x6,0xD8);
    lora_spi_write(0x7,0x99);
    lora_spi_write(0x8,0x99);
    lora_spi_write(0x9,0x0);
    lora_spi_write(0xA,0x9);
    lora_spi_write(0xB,0x3B);
    lora_spi_write(0xC,0x23);
    lora_spi_write(0xD,0x1);
    lora_spi_write(0xE,0x80);
    lora_spi_write(0xF,0x0);
    lora_spi_write(0x10,0x0);
    lora_spi_write(0x11,0x0);
    lora_spi_write(0x12,0x0);
    lora_spi_write(0x13,0x0);
    lora_spi_write(0x14,0x0);
    lora_spi_write(0x15,0x0);
    lora_spi_write(0x16,0x0);
    lora_spi_write(0x17,0x0);
    lora_spi_write(0x18,0x10);
    lora_spi_write(0x19,0x0);
    lora_spi_write(0x1A,0x0);
    lora_spi_write(0x1B,0x0);
    lora_spi_write(0x1C,0x0);
    lora_spi_write(0x1D,0x4A);
    lora_spi_write(0x1E,0x97);
    lora_spi_write(0x1F,0xFF);
    lora_spi_write(0x20,0x0);
    lora_spi_write(0x21,0x8);
    lora_spi_write(0x22,0xFF);
    lora_spi_write(0x23,0xFF);
    lora_spi_write(0x24,0x0);
    lora_spi_write(0x25,0x0);
    lora_spi_write(0x26,0x0);
    lora_spi_write(0x27,0x0);
    lora_spi_write(0x28,0x0);
    lora_spi_write(0x29,0x0);
    lora_spi_write(0x2A,0x0);
    lora_spi_write(0x2B,0x0);
    lora_spi_write(0x2C,0x0);
    lora_spi_write(0x2D,0x50);
    lora_spi_write(0x2E,0x14);
    lora_spi_write(0x2F,0x40);
    lora_spi_write(0x30,0x0);
    lora_spi_write(0x31,0x3);
    lora_spi_write(0x32,0x5);
    lora_spi_write(0x33,0x27);
    lora_spi_write(0x34,0x1C);
    lora_spi_write(0x35,0xA);
    lora_spi_write(0x36,0x0);
    lora_spi_write(0x37,0xA);
    lora_spi_write(0x38,0x42);
    lora_spi_write(0x39,0x12);
    lora_spi_write(0x3A,0x65);
    lora_spi_write(0x3B,0x1D);
    lora_spi_write(0x3C,0x1);
    lora_spi_write(0x3D,0xA1);
    lora_spi_write(0x3E,0x0);
    lora_spi_write(0x3F,0x0);
    lora_spi_write(0x40,0x0);
    lora_spi_write(0x41,0x0);
    lora_spi_write(0x42,0x22);
        
    
    lora_setMode(state_struct.mode); // Read function to understand
    lora_setHeaderOn();
    lora_setChannel(state_struct.channel);
    lora_setCrcOn();
    lora_setPower('M'); // M = Max
    lora_setNodeAddress(state_struct.src_addr); // LORA_ID is set in header file
}

/**
  * @desc Enable LoRa mode on the chip
  * @param none
  * @return none
 **/
void lora_setLoraMode() {
    
    lora_spi_write(REG_OP_MODE, FSK_SLEEP_MODE);    // Sleep mode (mandatory to set LoRa mode)
	lora_spi_write(REG_OP_MODE, LORA_SLEEP_MODE);   // LoRa sleep mode
	lora_spi_write(REG_OP_MODE, LORA_STDBY_MODE);	// LoRa standby mode

	lora_spi_write(REG_MAX_PAYLOAD_LENGTH, 255); // Set max payload length to max (255)
    
    __delay_ms(100);
}

/**
  * @desc Send byte to LoRa chip
  * @param char : register address
  * @param char : byte to write
  * @return none
 **/
void lora_spi_write(char addr, char data) {
    
    LATD5 = 0; // CS_lora
    spi_w8b(addr | 0x80); // mask to write mode
    spi_w8b(data);        // Send data
    LATD5 = 1; // CS_lora
}

/**
  * @desc Read byte from LoRa chip
  * @param char : register address
  * @return char : byte read
 **/
char lora_spi_read(char addr) {
    
    char tmp;
    
    LATD5 = 0; // CS_lora
    spi_w8b(addr & 0x7F); // read mode
    tmp = spi_r8b();
    
    LATD5 = 1; // CS_lora
    
    return tmp;
}

/**
  * @desc set the mask to register
  * @param char : register address
  * @param char : mask
  * @return none
 **/
void lora_spi_write_mask_set(char addr, char mask) {
    
    char tmp;
    
    tmp = lora_spi_read(addr);        // Read the register and save it
    lora_spi_write(addr, tmp | mask); // Write the masked register
}

/**
  * @desc clear the mask to register
  * @param char : register address
  * @param char : mask
  * @return none
 **/
void lora_spi_write_mask_clr(char addr, char mask) {
    
    char tmp;
    
    tmp = lora_spi_read(addr);           // Read the register and save it
    lora_spi_write(addr, tmp & (~mask)); // Write the masked register
}

/**
  * @desc Set RF mode
  * @param char : mode
  * @return none
 **/
void lora_setMode(char mode) {
    
    switch (mode)
    {
        // mode 1 (better reach, medium time on air)
        case 1:     lora_setCR(LORA_CR_5);        // CR = 4/5
                    lora_setSF(LORA_SF_12);       // SF = 12
                    lora_setBW(LORA_BW_125);      // BW = 125 KHz
                    break;
 
        // mode 2 (medium reach, less time on air)
        case 2:     lora_setCR(LORA_CR_5);        // CR = 4/5
                    lora_setSF(LORA_SF_12);       // SF = 12
                    lora_setBW(LORA_BW_250);      // BW = 250 KHz
                    break;
 
        // mode 3 (worst reach, less time on air)
        case 3:     lora_setCR(LORA_CR_5);        // CR = 4/5
                    lora_setSF(LORA_SF_10);       // SF = 10
                    lora_setBW(LORA_BW_125);      // BW = 125 KHz
                    break;
 
        // mode 4 (better reach, low time on air)
        case 4:     lora_setCR(LORA_CR_5);        // CR = 4/5
                    lora_setSF(LORA_SF_12);       // SF = 12
                    lora_setBW(LORA_BW_500);      // BW = 500 KHz
                    break;
 
        // mode 5 (better reach, medium time on air)
        case 5:     lora_setCR(LORA_CR_5);        // CR = 4/5
                    lora_setSF(LORA_SF_10);       // SF = 10
                    lora_setBW(LORA_BW_250);      // BW = 250 KHz
                    break;
 
        // mode 6 (better reach, worst time-on-air)
        case 6:     lora_setCR(LORA_CR_5);        // CR = 4/5
                    lora_setSF(LORA_SF_11);       // SF = 11
                    lora_setBW(LORA_BW_500);      // BW = 500 KHz
                    break;
 
        // mode 7 (medium-high reach, medium-low time-on-air)
        case 7:     lora_setCR(LORA_CR_5);        // CR = 4/5
                    lora_setSF(LORA_SF_9);        // SF = 9
                    lora_setBW(LORA_BW_250);      // BW = 250 KHz
                    break;
 
        // mode 8 (medium reach, medium time-on-air)
        case 8:     lora_setCR(LORA_CR_5);        // CR = 4/5
                    lora_setSF(LORA_SF_9);        // SF = 9
                    lora_setBW(LORA_BW_500);      // BW = 500 KHz
                    break;
 
        // mode 9 (medium-low reach, medium-high time-on-air)
        case 9:     lora_setCR(LORA_CR_5);        // CR = 4/5
                    lora_setSF(LORA_SF_8);        // SF = 8
                    lora_setBW(LORA_BW_500);      // BW = 500 KHz
                    break;
 
        // mode 10 (worst reach, less time_on_air)
        case 10:    lora_setCR(LORA_CR_5);        // CR = 4/5
                    lora_setSF(LORA_SF_7);        // SF = 7
                    lora_setBW(LORA_BW_500);      // BW = 500 KHz
                    break;
 
    }
    
}

/**
  * @desc Set coding rate
  * @param char : coding rate (read header file)
  * @return none
 **/
void lora_setCR(char cr) {
    
    char save_state;
    
    save_state = lora_spi_read(REG_OP_MODE); // save module state
    
    lora_spi_write(REG_OP_MODE, LORA_STDBY_MODE); // standby mode
    
    lora_spi_write_mask_clr(REG_MODEM_CONFIG1, 0x38);       // Prepare CR
    lora_spi_write_mask_set(REG_MODEM_CONFIG1, cr << 3);	// Set CR

    lora_spi_write(REG_OP_MODE, save_state); // restore module state
}

/**
  * @desc Set bandwidth
  * @param char : bandwidth (read header file)
  * @return none
 **/
void lora_setBW(char bw) {
    
    char save_state;
    
    save_state = lora_spi_read(REG_OP_MODE); // save module state
    
    lora_spi_write(REG_OP_MODE, LORA_STDBY_MODE); // standby mode
    
    lora_spi_write_mask_clr(REG_MODEM_CONFIG1, 0x3F);       // Prepare BW
    lora_spi_write_mask_set(REG_MODEM_CONFIG1, bw << 6);	// set BW

    lora_spi_write(REG_OP_MODE, save_state); // restore module state
}

/**
  * @desc Set Spreading factor
  * @param char : spreading factor (read header file)
  * @return none
 **/
void lora_setSF(char sf) {
    
    char save_state;
    
    save_state = lora_spi_read(REG_OP_MODE); // save module state
    
    lora_spi_write(REG_OP_MODE, LORA_STDBY_MODE); // standby mode
    
    lora_spi_write_mask_clr(REG_MODEM_CONFIG1, 0xF0);       // Prepare SF
    lora_spi_write_mask_set(REG_MODEM_CONFIG1, sf << 4);	// set SF

    lora_spi_write(REG_OP_MODE, save_state); // restore module state
}

/**
  * @desc Set channel
  * @param unsigned long (read the header file)
  * @return none
 **/
void lora_setChannel(char channel) {
    char save_state;
    unsigned long ch;
    
    switch (channel) {
        case 1:
            ch = LORA_CH_10_868;
            break;
        case 2:
            ch = LORA_CH_11_868;
            break;
        case 3:
            ch = LORA_CH_12_868;
            break;
        case 4:
            ch = LORA_CH_13_868;
            break;
        case 5:
            ch = LORA_CH_14_868;
            break;
        case 6:
            ch = LORA_CH_15_868;
            break;
        case 7:
            ch = LORA_CH_16_868;
            break;
        case 8:
            ch = LORA_CH_17_868;
            break;
        case 9:
            ch = LORA_CH_00_900;
            break;
        case 10:
            ch = LORA_CH_01_900;
            break;
        case 11:
            ch = LORA_CH_02_900;
            break;
        case 12:
            ch = LORA_CH_03_900;
            break;
        case 13:
            ch = LORA_CH_04_900;
            break;
        case 14:
            ch = LORA_CH_05_900;
            break;
        case 15:
            ch = LORA_CH_06_900;
            break;
        case 16:
            ch = LORA_CH_07_900;
            break;
        case 17:
            ch = LORA_CH_08_900;
            break;
        case 18:
            ch = LORA_CH_09_900;
            break;
        case 19:
            ch = LORA_CH_10_900;
            break;
        case 20:
            ch = LORA_CH_11_900;
            break;
        case 21:
            ch = LORA_CH_12_900;
            break;
    }
    
    unsigned int freq3, freq2;
    char freq1;
    
    save_state = lora_spi_read(REG_OP_MODE); // save module state
    
    lora_spi_write(REG_OP_MODE, LORA_STDBY_MODE); // standby mode
    
    freq3 = ((ch >> 16) & 0x0FF);		// frequency channel MSB
    freq2 = ((ch >> 8) & 0x0FF);		// frequency channel MIB
    freq1 = (ch & 0xFF);				// frequency channel LSB
    
    lora_spi_write(REG_FRF_MSB, freq3); // *
    lora_spi_write(REG_FRF_MID, freq2); // *
    lora_spi_write(REG_FRF_LSB, freq1); // * Write frequency to chip
    
    __delay_ms(100);

    lora_spi_write(REG_OP_MODE, save_state); // restore module state
}

/**
  * @desc toggle Rx / Tx pin on module (?)
  * @param char : state (Read the header file)
  * @return none
 **/
void lora_rf_switch(char state) {
    
    if (state == LORA_MODE_TX) {
        
        LATD3 = 0; // *
        LATD2 = 1; // * Tx mode
    }
    else {
        
        LATD3 = 1; // *
        LATD2 = 0; // * Rx mode
    }
}

/**
  * @desc Set header on
  * @param none
  * @return none
 **/
void lora_setHeaderOn() {
    
    char save_state;
    
    save_state = lora_spi_read(REG_OP_MODE); // save module state
    
    lora_spi_write(REG_OP_MODE, LORA_STDBY_MODE); // standby mode
    
    lora_spi_write_mask_clr(REG_MODEM_CONFIG1, 0x4); // set header on

    lora_spi_write(REG_OP_MODE, save_state); // restore module state
}

/**
  * @desc Set header off
  * @param none
  * @return none
 **/
void lora_setHeaderOff() {
    char save_state;
    
    save_state = lora_spi_read(REG_OP_MODE); // save module state
    
    lora_spi_write(REG_OP_MODE, LORA_STDBY_MODE); // standby mode
    
    lora_spi_write_mask_set(REG_MODEM_CONFIG1, 0x4); // set header off

    lora_spi_write(REG_OP_MODE, save_state); // restore module state
}

/**
  * @desc Set CRC on
  * @param none
  * @return none
 **/
void lora_setCrcOn() {
    
    char save_state;
        
    save_state = lora_spi_read(REG_OP_MODE); // save module state
    
    lora_spi_write(REG_MODEM_CONFIG1, 0x02); // set CRC on

    lora_spi_write(REG_OP_MODE, save_state); // restore module state
}

/**
  * @desc Clear all flags
  * @info "writing a 1 clears the IRQ"
  * @param none
  * @return none
 **/
void lora_clrFlags() {
    
    char save_state;
        
    save_state = lora_spi_read(REG_OP_MODE); // save module state
    
    lora_spi_write(REG_IRQ_FLAGS, 0xFF); // Clear all flags

    lora_spi_write(REG_OP_MODE, save_state); // restore module state
}

/**
  * @desc Set power mode
  * @param char : 'L', 'H' or 'M'
  * @return none
 **/
void lora_setPower(char power) {
    
    char save_state;
    char power_val;
    
    save_state = lora_spi_read(REG_OP_MODE); // save module state
    
    switch (power)
    {
        // L = low
        // H = high
        // M = max

        case 'M':  power_val = 0x0F;
                   break;

        case 'L':  power_val = 0x00;
                   break;

        case 'H':  power_val = 0x07;
                   break;
    }

    lora_spi_write(REG_PA_CONFIG, power_val); // Setting output power value
    lora_spi_write_mask_set(REG_PA_CONFIG, 0x80); // output limited to 20dBm

    lora_spi_write(REG_OP_MODE, save_state);  // restore module state
}

/**
  * @desc Set Max Current of the module
  * @param char : current
  * @return none
 **/
void lora_setMaxCurrent(char current) {
    
    char save_state;
        
    save_state = lora_spi_read(REG_OP_MODE); // save module state
    
    current |= 0x20; // activate overcurrent protection
    
    lora_spi_write(REG_OP_MODE, LORA_STDBY_MODE);    // Set LoRa Standby mode to write in registers
    
    lora_spi_write(REG_OCP, current); // Set max current

    lora_spi_write(REG_OP_MODE, save_state); // restore module state
}

/**
  * @desc Load packet to the chips buffer
  * @param char : destination address (like LORA_ID)
  * @param char* : pointer to first payload char (last is '\0')
  * @return none
 **/
void lora_setPacket(char dest, char *payload) {
    
    static char pack_nb = 0;
    char i, j;
    
    char save_state;
        
    save_state = lora_spi_read(REG_OP_MODE); // save module state
        
    lora_clrFlags();
    lora_spi_write(REG_OP_MODE, LORA_STDBY_MODE);
    
    lora_spi_write(REG_FIFO_ADDR_PTR, 0x80); // Set pointer to Tx FIFO location
    
    // Write header to buffer:
    lora_spi_write(REG_FIFO, dest);    // Destination ID (or 0 to broadcast)
    lora_spi_write(REG_FIFO, LORA_ID); // Source ID (our ID)
    lora_spi_write(REG_FIFO, pack_nb); // Package number
    
    // Calculate payload length:
    for (i=0;*(payload+i)!='\0' && i<254;i++) ; // find '\0' or max size
    
    lora_spi_write(REG_FIFO, i); // payload length
    
    // Data transfer
    for (j=0;j<i;j++)
        lora_spi_write(REG_FIFO, *(payload+j)); // yeah, just here
        
    
    lora_spi_write(REG_FIFO, 0x00); // retry nb
    /*
     * Always set to 0, because the receiver will not acknowledge
     */
    
    //pack_nb++;
    
    lora_spi_write(REG_OP_MODE, save_state); // restore module state
}

/**
  * @desc Send packet with LoRa: function called by main
  * @param char : destination address (like LORA_ID)
  * @param char* : pointer to first payload char (last is '\0')
  * @return none
 **/
void lora_sendPacket(char dest, char* payload) {
    

    
    lora_setPacket(dest, payload); // Load packet in chip buffer
    lora_clrFlags();               // Clear flags to prepare sending
    
    lora_spi_write(REG_OP_MODE, LORA_TX_MODE); // go Tx mode ! (SEND !!)
    
    // Wait until the packet is not sent ( by checking TX_Done_flag)
    while (!(lora_spi_read(REG_IRQ_FLAGS) & 0x08)) ; // TODO: timeout
}

/**
  * @desc Write node address (LORA_ID) to the  chip
  * @param char : node addresse (read header file)
  * @return none
 **/
void lora_setNodeAddress(char addr) {
    
    // must be less than 255 !!!!
    char save_state;
        
    save_state = lora_spi_read(REG_OP_MODE); // save module state
    
    lora_spi_write(REG_OP_MODE, LORA_STANDBY_FSK_REGS_MODE); // Go in FSK mode to write the node addr
    lora_spi_write(REG_NODE_ADRS, addr); // Write node address
	lora_spi_write(REG_BROADCAST_ADRS, 0x00); // Write broadcast address

    lora_spi_write(REG_OP_MODE, save_state); // restore module state
}

/**
  * @desc Set packet max length
  * @param char : max length
  * @return none100
 **/
void lora_setPacquetLength(char l) {
    
    char save_state;
        
    save_state = lora_spi_read(REG_OP_MODE); // save module state
    
    lora_spi_write(REG_OP_MODE, LORA_STDBY_MODE); // Set LoRa Standby mode to write in registers
    lora_spi_write(REG_PAYLOAD_LENGTH_LORA, l);   // Set length to chip

    lora_spi_write(REG_OP_MODE, save_state); // restore module state
}

/**
  * @desc Switch chip to receive mode
  * @param none
  * @return none
 **/
void lora_receiveMode() {
    
    // Setting Testmode
    lora_spi_write(0x31,0x43); // (??)
    
    lora_spi_write(REG_PA_RAMP, 0x09); // set rise/fall ramp to 40us

    lora_spi_write(REG_LNA, 0x23);			  // set LNA gain to: G3 + LNA_Boot: ON (150%)
    lora_spi_write(REG_FIFO_ADDR_PTR, 0x00);  // setting FIFO addr pointer to Rx
    
    lora_spi_write(REG_SYMB_TIMEOUT_LSB, 0xFF); // set timeout to max
    lora_spi_write(REG_FIFO_RX_BYTE_ADDR, 0x00); // set Rx addr to 0
    
    lora_setPacquetLength(255); // set packet length to max
    
    lora_spi_write(REG_OP_MODE, LORA_RX_MODE); // receive mode: continuous
}

/**
  * @desc Receive packet (function called by main)
  * @info !!! BLOCKING fUNCTION !!!
  * @param char* : pointer to length received packet
  * @param char* : pointer to received packet
  * @return none
 **/
char lora_receivePaquet_tmo1s(char *length, char *paquet) {
    
    lora_receiveMode(); // switch to receive mode
    if (lora_availableData()) { // data is for me ? (or broadcast)
        LATA1 = 1; // Set Rx Led: ON
        lora_getPaquet(length, paquet); // Download Packet from chip
        LATA1 = 0; // Set Tx Led: OFF
        return 0;
    }
    
    return 1;
}

/**
  * @desc Check if packet as been received
  * @param none
  * @return none
 **/
char lora_availableData() {
    
    char me;
    char dest;
    
    T0CONbits.TMR0ON = 1;
    
    TMR0 = 0;
    //
    while(!(lora_spi_read(REG_IRQ_FLAGS) & 0x10)) {

        if(timeout(3))
             return 0; // If no packets received during a 3s timeout
    }

    // Waiting to read first payload bytes from packet
    TMR0 = 0;
    while (!lora_spi_read(REG_FIFO_RX_BYTE_ADDR)) {

        if(timeout(3))
            return 0; // If no packets received during a 3s timeout
    }
    
    T0CONbits.TMR0ON = 0;
    
    me = LORA_ID; // My ID
    dest = lora_spi_read(REG_FIFO); // Get destination byte (first packet byte)
    if (dest == me || dest == 0) { // My ID or Broadcast
        return 1; // Data ok !
    }
    
    return 0; // Not for me...
}

/**
  * @desc Download packet from chip
  * @param char* : pointer to length received packet
  * @param char* : pointer to received packet
  * @return none
 **/
void lora_getPaquet(char *length, char *paquet) {
    
    char i;
    
    T0CONbits.TMR0ON = 1;
    
    TMR0 = 0;
    while (!(lora_spi_read(REG_IRQ_FLAGS) & 0x40) && !timeout(3)) ; // if Rx done
    
    T0CONbits.TMR0ON = 0;
    
    lora_spi_write(REG_FIFO_ADDR_PTR, 0x00); // return to 0x00 FIFO (dest)
    *(paquet + 0) = lora_spi_read(REG_FIFO); // dest save
    *(paquet + 1) = lora_spi_read(REG_FIFO); // src save
    *(paquet + 2) = lora_spi_read(REG_FIFO); // packnum save
    *(paquet + 3) = lora_spi_read(REG_FIFO); // length save
    
    for (i=0; i<*(paquet + 3);i++) {
        *(paquet + 4+i) = lora_spi_read(REG_FIFO); // payload save
    }
    
    *(paquet + 4+i) = lora_spi_read(REG_FIFO); // retry nb save
    
    lora_spi_write(REG_FIFO_ADDR_PTR, 0x00); // clean fct out
    
    lora_clrFlags(); // Clear flags (if not received will be done 2x)
    
    *length = *(paquet + 3); // set length byte to length pointer
}

/**
  * @desc Get RSSI level for the last packet
  * @param none
  * @return RSSI level
 **/
signed int lora_getRSSI() {
    
    signed int rssi;
    
    rssi = lora_spi_read(REG_RSSI_VALUE_LORA);
    rssi -= 139;
    
    return rssi;
}
char timeout(char sec) {
    if (INTCONbits.T0IF) {
        INTCONbits.T0IF = 0;
        timeout_var++;
        if (timeout_var == sec) {
            timeout_var = 0;
            return 1;
        }
    }
    
    return 0;
}