/* 
 * File:   main.c
 * Author: 3kthor3adward
 *
 * Created on October 5, 2012, 12:30 PM
 */

#include <xc.h>
#pragma config FOSC = XT, WDTE = OFF, CP = OFF, PWRTE = OFF

/*
 *Intervalo de direcciones de la memoria RAM.
 */
#define START_ROM_ADDR 0b0000000000000000
#define END_ROM_ADDR   0b0000011111111111

/*
 * Bus usado pra las partes alta y baja del bus de direcciones
 *  y los datos.
 */
#define UNIVERSAL_BUS PORTB
#define AL0 RA0 //Address Latch 0
#define AL1 RA1 //Address Latch 1
#define notOE RA2 //Output Enable

#define _XTAL_FREQ 4000000 //4MHz

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
    PORTA = 0x0;
    PORTB = 0x0;
    
    TRISA = 0x0;
    TRISB = 0x0;

    unsigned int ramAddress = START_ROM_ADDR; //Primer direccion;

    while (ramAddress <= END_ROM_ADDR) {
        //---Condiciones iniciales
        UNIVERSAL_BUS = 0;
        AL0 = 1;
        AL1 = 1;
        notOE = 1;

        unsigned int romAddres = START_ROM_ADDR;

        //----carga el dato desde la ROM

        loadAddress(romAddres);

        TRISB = 0xFF; //PortB como entrada
        notOE = 0; //--Coloca el dato de la ROM en el bus universal
        __delay_ms(100);
        FSR = PORTB; //--PIC guarda el dato
        notOE = 1;
        TRISB = 0; //PortB como salida     

        //----mostrar en el display contenido de FSR

        ramAddress++;
    }

    while (1);
}