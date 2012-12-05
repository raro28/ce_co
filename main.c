/*
 * File:   main.c
 * Author: ekthor
 *
 * Created on November 30, 2012, 9:57 AM
 */

#pragma config BOREN = OFF, CPD = OFF, DEBUG = OFF, WRT = OFF, FOSC = XT, WDTE = OFF, CP = OFF, LVP = OFF, PWRTE = OFF
#include "lcd8bit.h"
#include "keypad.h"
#include "minimal.h"

int main(int argc, char** argv) {
ADCON1 = 0x6;
    TRISA = 0;
    TRISB = 0xF0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;

    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    notOE = 1;
    notWE = 1;

    lcd_init(); // Inicializa el display
    printf("d(^_^)b");

    OPTION_REGbits.nRBPU = 0;
    INTCONbits.RBIE = 1;
    INTCONbits.GIE = 1;

    while (1);
}

void search(unsigned char *inputStream) {
    TRISB = 0;
    PORTB = 0;

    lcd_cls();
    unsigned char addr = 0;
    short count = 0;
    while(addr <= START_ROM_ADDR){
        ADDRBUS = addr;
        TRISD = 0xFF;
        notOE = 0;
        __delay_ms(100);
        FSR = DATABUS;
        notOE = 1;
        TRISD = 0;

        lcd_cmd(LCD_SETDDADDR1);
        printf("[%d]",addr);
        lcd_cmd(LCD_SETDDADDR2);
        printf("[%x]",FSR);

        addr ++;
    }
    
    PORTB = 0;
    TRISB = 0xF0;
}