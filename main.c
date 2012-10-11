/* 
 * File:   main.c
 * Author: 3kthor3adward
 *
 * Created on October 5, 2012, 12:30 PM
 */

#include <xc.h>
#pragma config FOSC = XT, WDTE = OFF, CP = OFF, PWRTE = OFF

/*
 *Intervalo de direcciones de la memoria ROM.
 */
#define START_ROM_ADDR 0b0010000000000000
#define END_ROM_ADDR   0b0011111111111111

/*
 *Intervalo de direcciones de la memoria RAM.
 */
#define START_RAM_ADDR 0b0000000000000000
#define END_RAM_ADDR   0b0000011111111111

/*
 * Bus usado pra las partes alta y abjas del bus de direcciones
 *  y los datos.
 */
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

void portBAsInput() {
    RB0 = 1;
    TRISB = 0xFF;
    RB0 = 0;
}

void portBAsOutput() {
    RB0 = 1;
    TRISB = 0;
    RB0 = 0;
}

void loadAddress(unsigned int address) {
    AL0 = 1;
    AL1 = 1;
    //---bits bajos de la direccion
    UNIVERSAL_BUS = address;
    dowait();
    AL0 = 0;
    dowait();
    //---bits altos de la direccion
    UNIVERSAL_BUS = address >> 8;
    dowait();
    AL1 = 0;
    dowait();
}

/*
 * Accesso secuencial en un espacio de memoria de 16 bits
 *
 * param action: accion a relizar en determinada localidad de memoria
 */
void copyAll(unsigned int from, unsigned int to) {
    unsigned int ramAddress = from; //Primer direccion;

    while (ramAddress <= to) {
        //---Condiciones iniciales
        UNIVERSAL_BUS = 0;
        AL0 = 1;
        AL1 = 1;
        notWE = 1;
        notOE = 1;

        dowait();

        unsigned int romAddres = ramAddress | START_ROM_ADDR;

        //----carga el dato desde la ROM

        loadAddress(romAddres);
        portBAsInput();
        notOE = 0; //--Coloca el dato de la ROM en el bus universal
        dowait();
        FSR = PORTB; //--PIC guarda el dato
        PORTB = 0;
        portBAsOutput();
        notOE = 1;
        dowait();

        //----guarda el dato del bus en la RAM

        loadAddress(ramAddress);
        UNIVERSAL_BUS = FSR; //--Recuperamos el dato del PIC y lo colocamos en el bus universal
        dowait();
        notWE = 0; //Se guarda en la RAM
        dowait();

        ramAddress++;
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

    copyAll(START_RAM_ADDR, END_RAM_ADDR); //Copia de la ROM a la RAM

    while (1);
}