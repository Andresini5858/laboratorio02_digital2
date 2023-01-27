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

#define _XTAL_FREQ 4000000

uint16_t voltaje1;
uint16_t voltaje2;
uint8_t vol1;
unsigned int vol2;
unsigned int unidad1;
unsigned int decima1;
unsigned int centesima1;
unsigned int unidad2;
unsigned int decima2;
unsigned int centesima2;
char buffer[20];


void setup(void);
uint8_t map(int value, int inputmin, int inputmax, int outmin, int outmax){ //función para mapear valores
    return ((value - inputmin)*(outmax-outmin)) / (inputmax-inputmin)+outmin;}

void main(void) {
    setup();
    ADC_setup();
    ADC_config(0);
    ADC_config(1);
    Lcd_Init();
    while(1){
        Lcd_Set_Cursor(1,8);
        Lcd_Write_String("S2:");
        ADCON0bits.GO = 1;
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("S1:");
    }
}

void __interrupt() isr(void){
    if (PIR1bits.ADIF == 1){
        if (ADCON0bits.CHS == 0b0000){
        voltaje1 = ADC_read(0);
        vol1 = map(voltaje1, 0, 255, 0, 100);
        unidad1 = (vol1*5)/100;
        decima1 = ((vol1*5)/10)%10;
        centesima1 = (vol1*5)%10;
        Lcd_Set_Cursor(2,1);
        sprintf(buffer, "%d.%d%dV", unidad1, decima1, centesima1);
        Lcd_Write_String(buffer);
        ADCON0bits.CHS = 0b0001;}
        else if (ADCON0bits.CHS == 0b0001){
        voltaje2 = ADC_read(1);
        vol2 = map(voltaje2, 0, 255, 0, 100);
        unidad2 = (vol2*5)/100;
        decima2 = ((vol2*5)/10)%10;
        centesima2 = (vol2*5)%10;
        Lcd_Set_Cursor(2,7);
        sprintf(buffer, "%d.%d%dV", unidad2, decima2, centesima2);
        Lcd_Write_String(buffer);
        ADCON0bits.CHS = 0b0000;}
        PIR1bits.ADIF = 0; 
    }
}

void setup(void){
    ANSELH = 0;
    
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    
    INTCONbits.GIE = 1; //Activar interrupciones globales
    INTCONbits.PEIE = 1; //Activar interrupciones periféricas
    PIE1bits.ADIE = 1; // Habiliar interrupcion del conversor ADC
    
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.SCS = 1;
}