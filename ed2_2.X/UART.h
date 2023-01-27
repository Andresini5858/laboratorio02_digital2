#ifndef UART
#define	UART

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h>

void UART_config (uint16_t baudrate); 
void UART_write_char (char character); 
char UART_read_char (void); 

#endif	/* XC_HEADER_TEMPLATE_H */

