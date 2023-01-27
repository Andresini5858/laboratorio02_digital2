/*
 * File:   UART.c
 * Author: Andres
 *
 * Created on January 27, 2023, 8:55 AM
 */


#include <xc.h>
#include "UART.h"

void UART_config(uint16_t baudrate){
    if (baudrate == 4800){
    SPBRG = 12; //valor para 9600 de baud rate

    TXSTAbits.SYNC = 0;         // Modo Asíncrono
    RCSTAbits.SPEN = 1;         // Habilitar UART

    TXSTAbits.TXEN = 1;         // Habilitamos la transmision
    PIR1bits.TXIF = 0;
    RCSTAbits.CREN = 1;         // Habilitamos la recepcion
    }
}
