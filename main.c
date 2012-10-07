/* 
 * File:   main.c
 * Author: 3kthor3adward
 *
 * Created on October 5, 2012, 12:30 PM
 */

#include <xc.h>
#pragma config FOSC = XT, WDTE = OFF, CP = OFF, PWRTE = OFF
/*
 * Bus usado pra las partes alta y abjas del bus de direcciones
 *  y los datos.
 */
#define START_ROM_ADDR 0x2000
#define END_ROM_ADDR 0x3FFF

#define START_RAM_ADDR 0x0
#define END_RAM_ADDR 0x7FF

#define UNIVERSAL_BUS PORTB
#define AL0 RA0 //Address Latch 0
#define AL1 RA1 //Address Latch 1
#define notOE RA2 //Output Enable de la RAM
#define notWE RA3 //Write Enable de la RAM

/**
 * Un retraso de aproximadamente 3*count ciclos
 */
void dowait() {
    int count = 200;
    while (count--);
}

/**
 * Almacena 10101010 en la direccion presente en los latches
 */
void store()
{
    //--datos
    UNIVERSAL_BUS = 0b10101010;
    notWE = 0;

    dowait();
    
    notWE = 1;
}

/**
 * Unicamente pone en el bus universal los datos de
 * la direccion presente en los latches
 */
void display()
{
    RB0 = 1;
    TRISB = 0xFF;
    RB0 = 0;

    notOE = 0;
    dowait();
    notOE = 1;

    RB0 = 1;
    TRISB = 0;
    RB0 = 0;
}

/*
 * Accesso secuencial en un espacio de memoria de 16 bits
 *
 * param action: accion a relizar en determinada localidad de memoria
 */
void process(void (*action)(), unsigned int from, unsigned int to)
{
    unsigned int address = from; //Primer direccion;

    while (address <= to) {
        //---Condiciones iniciales
        UNIVERSAL_BUS = 0;
        AL0 = 1;
        AL1 = 1;
        notWE = 1;
        notOE = 1;

        //---bits bajos de la direccion
        UNIVERSAL_BUS = address;
        AL0 = 0;
        dowait();
        //---bits altos de la direccion
        UNIVERSAL_BUS = address >> 8;
        AL1 = 0;
        dowait();

        action();

        address++;
    }
}

int main(void) {
    //----configuraciones iniciales----//
    RP0 = 0x0;
    PORTA = 0x0;
    PORTB = 0x0;
    RP0 = 0x1;
    TRISA = 0x0;
    TRISB = 0x0;
    RP0 = 0x0;

    process(store,START_RAM_ADDR, END_RAM_ADDR); //Guarda datos en RAM
    process(display,START_RAM_ADDR, END_RAM_ADDR); //Muestra datos de la RAM
    process(display,START_ROM_ADDR, END_ROM_ADDR); //Muestra datos de la ROM

    while (1);
}