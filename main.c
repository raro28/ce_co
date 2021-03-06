/* 
 * File:   main.c
 * Author: 3kthor3adward
 *
 * Created on October 5, 2012, 12:30 PM
 */
#include "extra.h"
#include "lcd8bit.h"
#include "stdio.h"

#pragma config FOSC = XT, WDTE = OFF, CP = OFF, PWRTE = OFF


void loadAddress(unsigned int address) {
    AL0 = 1;
    AL1 = 1;
    //---bits bajos de la direccion
    UNIVERSAL_BUS = address;
    __delay_ms(100);
    AL0 = 0;
    //---bits altos de la direccion
    UNIVERSAL_BUS = address >> 8;
    __delay_ms(100);
    AL1 = 0;
}

int main(void) {
    //----configuraciones iniciales----//
    PORTA = 0x4;
    PORTB = 0x0;
    
    TRISA = 0x0;
    TRISB = 0x0;

    lcd_init();

    printf("Hi there ^_^");
    __delay_us(1000);
    lcd_cls();

    unsigned int romAddres = 0x5c0; //Primer direccion;
    
    while (romAddres <= END_ROM_ADDR) {
        //---Condiciones iniciales
        UNIVERSAL_BUS = 0;
        AL0 = 1;
        AL1 = 1;
        notOE = 1;

        //----carga el dato desde la ROM

        loadAddress(romAddres);

        TRISB = 0xFF; //PortB como entrada
        notOE = 0; //--Coloca el dato de la ROM en el bus universal
        __delay_us(1);
        FSR = PORTB; //--PIC guarda el dato
        notOE = 1;
        TRISB = 0; //PortB como salida
        printf("%x", FSR);

        romAddres++;
    }

    lcd_cls();

    printf("fin");

    while (1);
}