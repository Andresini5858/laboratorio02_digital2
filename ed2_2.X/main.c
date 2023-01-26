/*
 * File:   main.c
 * Author: Andrés Lemus
 *
 * Created on January 25, 2023, 8:41 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "lcd.h"
#include "ADC.h"

#define _XTAL_FREQ 8000000

uint16_t voltaje1;
uint16_t voltaje2;

void setup(void);

void main(void) {
    setup();
    ADC_setup();
    ADC_config(0);
    ADC_config(1);
    Lcd_Init();
    while(1){
       if (ADCON0bits.GO == 0){ //Chequear si la conversión ya termino
            ADCON0bits.GO = 1;} //iniciar conversion
       Lcd_Set_Cursor(1,1);
       Lcd_Write_String("S1:");
       Lcd_Set_Cursor(1,5);
       Lcd_Write_String("S2:");
    }
}

void __interrupt() isr(void){
    if (PIR1bits.ADIF == 1){
        PIR1bits.ADIF = 0;
        voltaje1 = ADC_read(0);
        __delay_us(100);
        voltaje2 = ADC_read(1); 
        __delay_us(100);
    }
}

void setup(void){
    ANSELH = 0;
    
    //TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    
    INTCONbits.GIE = 1; //Activar interrupciones globales
    INTCONbits.PEIE = 1; //Activar interrupciones periféricas
    PIE1bits.ADIE = 1; // Habiliar interrupcion del conversor ADC
    PIR1bits.ADIF = 0; // Limpiar bandera de interrupción del ADC
    
    OSCCONbits.IRCF2 = 1; //Oscilador a 8MHz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1; //Oscialdor interno
}