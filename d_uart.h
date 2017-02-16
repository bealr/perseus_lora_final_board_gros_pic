/* 
 * File:   d_uart.h
 * Author: rbeal
 *
 * Created on December 8, 2016, 2:09 PM
 */

#ifndef D_UART_H
#define	D_UART_H

void uart_init();

void uart_send_char(char data);
void uart_send_string(const char *data);
void uart_send_string_length(char *data, char length);

void uart_recep_char(char data); // Not used at this time

#endif	/* D_UART_H */

