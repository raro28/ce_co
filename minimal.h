/* 
 * File:   minimal.h
 * Author: ekthor
 *
 * Created on November 30, 2012, 4:10 PM
 */

#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 20000000

/*
 *intervalo de direcciones de la memoria ROM.
 */
#define START_ROM_ADDR 0b0000000000000000
#define END_ROM_ADDR   0b0001111111111111

/*
 *intervalo de direcciones de la memoria RAM.
 */
#define START_RAM_ADDR 0b0010000000000000
#define END_RAM_ADDR   0b0010011111111111

#define notOE RA0 //Output Enable
#define notWE RA1 //Write Enable

#define DATABUS PORTB //Bus de datos conformado por el puerto b y c

/*
 * Bus de direcciones conformado por el puerto b y c
 */
extern volatile unsigned int ADDRBUS @ 0x007;

void search(char *);