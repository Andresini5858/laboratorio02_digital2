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
#include "LCD.h" //librería LCD
#include "ADC.h" //Libreria ADC
#include "UART.h" //Librería UART

#define _XTAL_FREQ 4000000 //Frecuencia 4MHz

/////////////////////////////////Variables//////////////////////////////////////
uint16_t voltaje1; //Variable para guardar el voltaje del canal AN0
uint16_t voltaje2; //Variable para guardar el voltaje del canal AN1
int vol1; //Variable para mapear datos de ADRESH de 0 a 100
int vol2; //Variable para mapear datos de ADRESH de 0 a 100
unsigned int unidad1; //Variable para guardar unidades del voltaje del AN0
unsigned int decima1; //Variable para guardar decimas del voltaje del AN0
unsigned int centesima1; //Variable para guardar centesimas del voltaje del AN0
unsigned int unidad2; //Variable para guardar unidades del voltaje del AN1
unsigned int decima2; //Variable para guardar decimas del voltaje del AN1
unsigned int centesima2; //Variable para guardar centesimas del voltaje del AN0
unsigned char contador = 0; //variable del contador a desplegar en la pantalla
char buffer[20]; //arreglo para guardar caracteres o variables y mostrar en la LCD
char buffer1[4]; //arreglo para guardar el contador y mostrar en la LCD
char pot1[3]; //arreglo para guardar valor del potenciometro y mostrar en terminal UART
char pot2[3]; //arreglo para guardar valor del potenciometro y mostrar en terminal UART

void setup(void); //función con configuración
void cadena(char *cursor); //función para desplegar caracteres en la terminal UART
int map(int value, int inputmin, int inputmax, int outmin, int outmax){ //función para mapear valores
    return ((value - inputmin)*(outmax-outmin)) / (inputmax-inputmin)+outmin;}

void main(void) {
    setup(); //Función de configuración
    ADC_setup(); //Función de configuración del Set-up
    ADC_config(0); //Se configura canal AN0
    ADC_config(1); //Se configura canal AN1
    UART_config(4800); //Se configura UART con baudrate de 4800
    Lcd_Init(); //Iniciar pantalla LCD
    Lcd_Clear(); //Limpiar pantalla
    Lcd_Set_Cursor(1,7); //Cursor en (1,7)
    Lcd_Write_String("S2:"); //Escribir S2 en pantalla
    Lcd_Set_Cursor(1,1); //Cursor en (1,1)
    Lcd_Write_String("S1:"); //Escribir S1 en pantalla
    Lcd_Set_Cursor(1,14); //Cursor en (1,14)
    Lcd_Write_String("S3:"); //Escribir S3 en pantalla
    while(1){
        ADCON0bits.GO = 1; //Mantener la conversión ADC
        if (contador >= 0 && contador < 10){
        Lcd_Set_Cursor(2,16); //Cursor en (2,16)
        sprintf(buffer1, "%d" , contador); //convertir variable a una cadena de caracteres
        Lcd_Write_String(buffer1); //Mostrar cadena de caracteres en pantalla
        Lcd_Set_Cursor(2,14); //Cursor en (2,14)
        Lcd_Write_Char('0'); //Mostrar 0 en pantalla
        Lcd_Set_Cursor(2,15); //Cursor en (2,15)
        Lcd_Write_Char('0'); //Mostrar 0 en pantalla
        }
        else if (contador > 9 && contador < 100){
        Lcd_Set_Cursor(2,15); //Cursor en (1,7)
        sprintf(buffer1, "%d" , contador); //convertir variable a una cadena de caracteres
        Lcd_Write_String(buffer1); //Mostrar cadena de caracteres en pantalla
        Lcd_Set_Cursor(2,14); //Cursor en (1,7)
        Lcd_Write_Char('0'); //Mostrar 0 en pantalla
        }
        else {
            Lcd_Set_Cursor(2,14); //Cursor en (1,7)
            sprintf(buffer1, "%d" , contador); //convertir variable a una cadena de caracteres
            Lcd_Write_String(buffer1); //Mostrar cadena de caracteres en pantalla
        }
        
    }  
}

void __interrupt() isr(void){ //INTERRUPCIONES
    if (PIR1bits.ADIF == 1){  //Revisar interrupcion del ADC
        if (ADCON0bits.CHS == 0b0000){ //Revisar si canal AN0 está seleccionado
        voltaje1 = ADC_read(0); //Guardar valor del voltaje en AN0
        vol1 = map(voltaje1, 0, 255, 0, 100); //mapear valor del voltaje de 0 a 100
        unidad1 = (vol1*5)/100; //Separar las unidades del valor del voltaje
        decima1 = ((vol1*5)/10)%10; //Separar las decimas del valor del voltaje
        centesima1 = (vol1*5)%10; //Separar las centesimas del valor del voltaje
        Lcd_Set_Cursor(2,1); //Cursor en (1,7)
        sprintf(buffer, "%d.%d%dV" , unidad1 , decima1 , centesima1 ); //convertir variable a una cadena de caracteres
        Lcd_Write_String(buffer); //Mostrar cadena de caracteres en pantalla
        ADCON0bits.CHS = 0b0001;} //Cambiar el canal a AN1
        else if (ADCON0bits.CHS == 0b0001){ //Revisar si canal AN1 está seleccionado
        voltaje2 = ADC_read(1); //Guardar valor del voltaje en AN1
        vol2 = map(voltaje2, 0, 255, 0, 100); //mapear valor del voltaje de 0 a 100
        unidad2 = (vol2*5)/100; //Separar las unidades del valor del voltaje
        decima2 = ((vol2*5)/10)%10; //Separar las decimas del valor del voltaje
        centesima2 = (vol2*5)%10; //Separar las centesimas del valor del voltaje
        Lcd_Set_Cursor(2,7); //Cursor en (1,7)
        sprintf(buffer, "%d.%d%dV" , unidad2 , decima2 , centesima2 ); //convertir variable a una cadena de caracteres
        Lcd_Write_String(buffer); //Mostrar cadena de caracteres en pantalla
        ADCON0bits.CHS = 0b0000;} //Cambiar el canal a AN0
        PIR1bits.ADIF = 0; //Limpiar la bandera de interrupcion del ADC
    }
    if (PIR1bits.RCIF == 1){ //Revisar interrupcion del Receptor del UART
        PIR1bits.RCIF = 0; //Limpiar la bandera de interrupcion del receptor del UART
        if (RCREG == '+'){ //Revisar si se recibió un '+'
            contador++; //Aumentar contador
        }
        if (RCREG == '-'){ //Revisar si se recibió un '-'
            contador--; //Decrementar contador
        }
        if (RCREG == 13){
            cadena("POT1: \r\n"); //Escribir en la terminal UART
            sprintf(pot1, "%u", voltaje1); //convertir variable a una cadena de caracteres
            cadena(pot1); //Mostrar variable en la terminal
            cadena("\n\r"); //Enter
            cadena("POT2: \n\r");
            sprintf(pot2, "%u", voltaje2); //convertir variable a una cadena de caracteres
            cadena(pot2); //Mostrar variable en la terminal
            cadena("\n\r"); //Enter    
        }
    }
}

void setup(void){
    ANSELH = 0; //Puertos como I/O digitales
    
    TRISB = 0; //Puerto B como salida
    TRISC = 0; //Puerto C como salida
    TRISD = 0; //Puerto D como salida
    
    PORTA = 0; //Limpiar Puerto A
    PORTB = 0; //Limpiar Puerto B
    PORTC = 0; //Limpiar Puerto C
    PORTD = 0; //Limpiar Puerto D
    
    INTCONbits.GIE = 1; //Activar interrupciones globales
    INTCONbits.PEIE = 1; //Activar interrupciones periféricas
    PIE1bits.ADIE = 1; // Habiliar interrupcion del conversor ADC
    PIE1bits.RCIE = 1; // Habiliar interrupcion del receptor UART
    
    OSCCONbits.IRCF2 = 1; //Oscilador interno a 4MHz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.SCS = 1; //Utilizar oscilador interno
}

//Funcion para mostrar texto
void cadena(char *cursor){
    while (*cursor != '\0'){//mientras el cursor sea diferente a nulo
        while (PIR1bits.TXIF == 0); //mientras que se este enviando no hacer nada
            TXREG = *cursor; //asignar el valor del cursor para enviar
            *cursor++;//aumentar posicion del cursor
    }
}