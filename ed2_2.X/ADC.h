#ifndef ADC
#define	ADC

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

#define _XTAL_FREQ 8000000

uint16_t valor;

void ADC_setup(void);
void ADC_config(uint8_t channel);
uint16_t ADC_read(uint16_t can);

#endif	/* XC_HEADER_TEMPLATE_H */

