/* 
 * File:   d_lora.h
 * Author: rbeal
 *
 * Created on December 8, 2016, 3:07 PM
 */

#ifndef D_LORA_H
#define	D_LORA_H

#define LORA_ID 0x03 // LORA NODE ADDRESS

void lora_init(char mode);
void lora_setLoraMode();

// SPI -> LORA module
void lora_spi_write(char addr, char data);
char lora_spi_read(char addr);
void lora_spi_write_mask_set(char addr, char mask);
void lora_spi_write_mask_clr(char addr, char mask);

// RF settings
void lora_setMode(char mode);
void lora_setCR(char cr);
void lora_setBW(char bw);
void lora_setSF(char sf);
void lora_setChannel(char channel);
void lora_rf_switch(char state);

// Module settings
void lora_setHeaderOn();
void lora_setHeaderOff();
void lora_setCrcOn();
void lora_clrFlags();

// Power
void lora_setPower(char power);
void lora_setMaxCurrent(char current);

// Tx functions
void lora_setPacket(char dest, char *payload);
void lora_sendPacket(char dest, char* payload);
void lora_setNodeAddress(char addr);
void lora_setPacquetLength(char l);

// Rx functions
void lora_receiveMode();
char lora_receivePaquet_tmo1s(char *length, char *paquet);
char lora_availableData();
void lora_getPaquet(char *length, char *paquet);
signed int lora_getRSSI();


char timeout(char sec);



// RX or Tx mode
#define LORA_RF_SWITCH_TX 0x1
#define LORA_RF_SWITCH_RX 0x0

// LORA REGISTERS
#define        REG_FIFO        					0x00
#define        REG_OP_MODE        				0x01
#define        REG_BITRATE_MSB    				0x02
#define        REG_BITRATE_LSB    				0x03
#define        REG_FDEV_MSB   					0x04
#define        REG_FDEV_LSB    					0x05
#define        REG_FRF_MSB    					0x06
#define        REG_FRF_MID    					0x07
#define        REG_FRF_LSB    					0x08
#define        REG_PA_CONFIG    				0x09
#define        REG_PA_RAMP    					0x0A
#define        REG_OCP    						0x0B
#define        REG_LNA    						0x0C
#define        REG_RX_CONFIG    				0x0D
#define        REG_FIFO_ADDR_PTR  				0x0D
#define        REG_RSSI_CONFIG   				0x0E
#define        REG_FIFO_TX_BASE_ADDR 		    0x0E
#define        REG_RSSI_COLLISION    			0x0F
#define        REG_FIFO_RX_BASE_ADDR   			0x0F
#define        REG_RSSI_THRESH    				0x10
#define        REG_FIFO_RX_CURRENT_ADDR   		0x10
#define        REG_RSSI_VALUE_FSK	    		0x11
#define        REG_IRQ_FLAGS_MASK    			0x11
#define        REG_RX_BW		    			0x12
#define        REG_IRQ_FLAGS	    			0x12
#define        REG_AFC_BW		    			0x13
#define        REG_RX_NB_BYTES	    			0x13
#define        REG_OOK_PEAK	    				0x14
#define        REG_RX_HEADER_CNT_VALUE_MSB  	0x14
#define        REG_OOK_FIX	    				0x15
#define        REG_RX_HEADER_CNT_VALUE_LSB  	0x15
#define        REG_OOK_AVG	 					0x16
#define        REG_RX_PACKET_CNT_VALUE_MSB  	0x16
#define        REG_RX_PACKET_CNT_VALUE_LSB  	0x17
#define        REG_MODEM_STAT	  				0x18
#define        REG_PKT_SNR_VALUE	  			0x19
#define        REG_AFC_FEI	  					0x1A
#define        REG_PKT_RSSI_VALUE	  			0x1A
#define        REG_AFC_MSB	  					0x1B
#define        REG_RSSI_VALUE_LORA	  			0x1B
#define        REG_AFC_LSB	  					0x1C
#define        REG_HOP_CHANNEL	  				0x1C
#define        REG_FEI_MSB	  					0x1D
#define        REG_MODEM_CONFIG1	 		 	0x1D
#define        REG_FEI_LSB	  					0x1E
#define        REG_MODEM_CONFIG2	  			0x1E
#define        REG_PREAMBLE_DETECT  			0x1F
#define        REG_SYMB_TIMEOUT_LSB  			0x1F
#define        REG_RX_TIMEOUT1	  				0x20
#define        REG_PREAMBLE_MSB_LORA  			0x20
#define        REG_RX_TIMEOUT2	  				0x21
#define        REG_PREAMBLE_LSB_LORA  			0x21
#define        REG_RX_TIMEOUT3	 				0x22
#define        REG_PAYLOAD_LENGTH_LORA		 	0x22
#define        REG_RX_DELAY	 					0x23
#define        REG_MAX_PAYLOAD_LENGTH 			0x23
#define        REG_OSC		 					0x24
#define        REG_HOP_PERIOD	  				0x24
#define        REG_PREAMBLE_MSB_FSK 			0x25
#define        REG_FIFO_RX_BYTE_ADDR 			0x25
#define        REG_PREAMBLE_LSB_FSK 			0x26
#define        REG_SYNC_CONFIG	  				0x27
#define        REG_SYNC_VALUE1	 				0x28
#define        REG_SYNC_VALUE2	  				0x29
#define        REG_SYNC_VALUE3	  				0x2A
#define        REG_SYNC_VALUE4	  				0x2B
#define        REG_SYNC_VALUE5	  				0x2C
#define        REG_SYNC_VALUE6	  				0x2D
#define        REG_SYNC_VALUE7	  				0x2E
#define        REG_SYNC_VALUE8	  				0x2F
#define        REG_PACKET_CONFIG1	  			0x30
#define        REG_PACKET_CONFIG2	  			0x31
#define        REG_DETECT_OPTIMIZE              0x31
#define        REG_PAYLOAD_LENGTH_FSK			0x32
#define        REG_NODE_ADRS	  				0x33
#define        REG_BROADCAST_ADRS	 		 	0x34
#define        REG_FIFO_THRESH	  				0x35
#define        REG_SEQ_CONFIG1	  				0x36
#define        REG_SEQ_CONFIG2	  				0x37
#define        REG_DETECTION_THRESHOLD          0x37
#define        REG_TIMER_RESOL	  				0x38
#define        REG_TIMER1_COEF	  				0x39
#define        REG_TIMER2_COEF	  				0x3A
#define        REG_IMAGE_CAL	  				0x3B
#define        REG_TEMP		  					0x3C
#define        REG_LOW_BAT	  					0x3D
#define        REG_IRQ_FLAGS1	  				0x3E
#define        REG_IRQ_FLAGS2	  				0x3F
#define        REG_DIO_MAPPING1	  				0x40
#define        REG_DIO_MAPPING2	  				0x41
#define        REG_VERSION	  					0x42
#define        REG_AGC_REF	  					0x43
#define        REG_AGC_THRESH1	  				0x44
#define        REG_AGC_THRESH2	  				0x45
#define        REG_AGC_THRESH3	  				0x46
#define        REG_PLL_HOP	  					0x4B
#define        REG_TCXO		  					0x58
#define        REG_PA_DAC		  				0x5A
#define        REG_PLL		  					0x5C
#define        REG_PLL_LOW_PN	  				0x5E
#define        REG_FORMER_TEMP	  				0x6C
#define        REG_BIT_RATE_FRAC	  			0x70

// LORA MODE
#define LORA_MODE_RX   0
#define LORA_MODE_TX   1
#define LORA_MODE_NONE 2

// LORA POWER MODE
#define LORA_STDBY_MODE              0x81
#define LORA_STANDBY_FSK_REGS_MODE   0xC1
#define LORA_TX_MODE                 0x83
#define LORA_RX_MODE                 0x85
#define LORA_SLEEP_MODE              0x80
#define FSK_SLEEP_MODE               0x80

// CODING RATE
#define LORA_CR_5     0x1 // 4/5
#define LORA_CR_6     0x2 // 4/6
#define LORA_CR_7     0x3 // 4/7
#define LORA_CR_8     0x4 // 4/8

// BANDWITDH (in KHz)
#define LORA_BW_125     0x0
#define LORA_BW_250     0x1
#define LORA_BW_500     0x2

// SPREADING FACTOR
#define LORA_SF_6     6  // 64   chips / symbol
#define LORA_SF_7     7  // 128  chips / symbol
#define LORA_SF_8     8  // 256  chips / symbol
#define LORA_SF_9     9  // 512  chips / symbol
#define LORA_SF_10    10 // 1024 chips / symbol
#define LORA_SF_11    11 // 2048 chips / symbol
#define LORA_SF_12    12 // 4096 chips / symbol

// SCALING FREQUENCY
#define LORA_CH_10_868  0xD84CCC // channel 10, central freq  865.20MHz
#define LORA_CH_11_868  0xD86000 // channel 11, central freq  865.50MHz
#define LORA_CH_12_868  0xD87333 // channel 12, central freq  865.80MHz
#define LORA_CH_13_868  0xD88666 // channel 13, central freq  866.10MHz
#define LORA_CH_14_868  0xD89999 // channel 14, central freq  866.40MHz
#define LORA_CH_15_868  0xD8ACCC // channel 15, central freq  866.70MHz
#define LORA_CH_16_868  0xD8C000 // channel 16, central freq  867.00MHz
#define LORA_CH_17_868  0xD90000 // channel 16, central freq  868.00MHz
#define LORA_CH_00_900  0xE1C51E // channel 00, central freq  903.08MHz
#define LORA_CH_01_900  0xE24F5C // channel 01, central freq  905.24MHz
#define LORA_CH_02_900  0xE2D999 // channel 02, central freq  907.40MHz
#define LORA_CH_03_900  0xE363D7 // channel 03, central freq  909.56MHz
#define LORA_CH_04_900  0xE3EE14 // channel 04, central freq  911.72MHz
#define LORA_CH_05_900  0xE47851 // channel 05, central freq  913.88MHz
#define LORA_CH_06_900  0xE5028F // channel 06, central freq  916.04MHz
#define LORA_CH_07_900  0xE58CCC // channel 07, central freq  918.20MHz
#define LORA_CH_08_900  0xE6170A // channel 08, central freq  920.36MHz
#define LORA_CH_09_900  0xE6A147 // channel 09, central freq  922.52MHz
#define LORA_CH_10_900  0xE72B85 // channel 10, central freq  924.68MHz
#define LORA_CH_11_900  0xE7B5C2 // channel 11, central freq  926.84MHz
#define LORA_CH_12_900  0xE4C000 // default channel 915MHz, the module is configured with it

#endif	/* D_LORA_H */

