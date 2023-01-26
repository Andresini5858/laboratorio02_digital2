#include <xc.h>
#include <stdint.h>
#include "ADC.h"

#define _XTAL_FREQ 8000000

void ADC_setup(void){
    ADCON0bits.ADCS1 = 1; // Fosc/32        
    ADCON0bits.ADCS0 = 0; //       
    
    ADCON1bits.VCFG1 = 0; // Referencia VSS (0 Volts)
    ADCON1bits.VCFG0 = 0; // Referencia VDD (5 Volts)
    
    ADCON1bits.ADFM = 0;  // Justificado hacia izquierda
    
    ADCON0bits.CHS3 = 0; // Canal AN0
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS0 = 0;        
    
    ADCON0bits.ADON = 1; // Habilitamos el ADC
    __delay_us(100); //delay de 100 us
}

void ADC_config(uint8_t channel){
    if (channel == 0){
        ANSELbits.ANS0 = 1;
    }
    else if (channel == 1){
        ANSELbits.ANS1 = 1;
    }
    else if (channel == 2){
        ANSELbits.ANS2 = 1;
    }
    else if (channel == 3){
        ANSELbits.ANS3 = 1;
    }
    else if (channel == 4){
        ANSELbits.ANS4 = 1;
    }
    else if (channel == 5){
        ANSELbits.ANS5 = 1;
    }
    else if (channel == 6){
        ANSELbits.ANS6 = 1;
    }
    else if (channel == 7){
        ANSELbits.ANS7 = 1;
    }
    else if (channel == 8){
        ANSELHbits.ANS8 = 1;
    }
    else if (channel == 9){
        ANSELHbits.ANS9 = 1;
    }
    else if (channel == 10){
        ANSELHbits.ANS10 = 1;
    }
    else if (channel == 11){
        ANSELHbits.ANS11 = 1;
    }
    else if (channel == 12){
        ANSELHbits.ANS12 = 1;
    }
    else if (channel == 13){
        ANSELHbits.ANS13 = 1;
    }
    else {
        ANSELbits.ANS0 = 1;
    }
}

uint16_t ADC_read(uint16_t can){
    if (can == 0){
        ADCON0bits.CHS = 0b0000;
        valor = ADRESH;
    }
    else if (can == 1){
        ADCON0bits.CHS = 0b0001;
        valor = ADRESH;
    }
    else if (can == 2){
        ADCON0bits.CHS = 0b0010;
        valor = ADRESH;
    }
    else if (can == 3){
        ADCON0bits.CHS = 0b0011;
        valor = ADRESH;
    }
    else if (can == 4){
        ADCON0bits.CHS = 0b0100;
        valor = ADRESH;
    }
    else if (can == 5){
        ADCON0bits.CHS = 0b0101;
        valor = ADRESH;
    }
    else if (can == 6){
        ADCON0bits.CHS = 0b0110;
        valor = ADRESH;
    }
    else if (can == 7){
        ADCON0bits.CHS = 0b0111;
        valor = ADRESH;
    }
    else if (can == 8){
        ADCON0bits.CHS = 0b1000;
        valor = ADRESH;
    }
    else if (can == 9){
        ADCON0bits.CHS = 0b1001;
        valor = ADRESH;
    }
    else if (can == 10){
        ADCON0bits.CHS = 0b1010;
        valor = ADRESH;
    }
    else if (can == 11){
        ADCON0bits.CHS = 0b1011;
        valor = ADRESH;
    }
    else if (can == 12){
        ADCON0bits.CHS = 0b1100;
        valor = ADRESH;
    }
    else if (can == 13){
        ADCON0bits.CHS = 0b1101;
        valor = ADRESH;
    }
    return valor;
}