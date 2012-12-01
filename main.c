/* 
 * File:   main.c
 * Author: ekthor
 *
 * Created on November 30, 2012, 9:57 AM
 */

#pragma config BOREN = OFF, CPD = OFF, DEBUG = OFF, WRT = OFF, FOSC = XT, WDTE = OFF, CP = OFF, LVP = OFF, PWRTE = OFF
#include "lcd8bit.h"
#include "minimal.h"

int main(int argc, char** argv) {
    /* Inicializacion del PIC*/
    STATUSbits.RP0 = 0;
    STATUSbits.RP1 = 0;

    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

    STATUSbits.RP0 = 1;

    ADCON1 = 0x6;
    TRISA = 0;

    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;

    STATUSbits.RP0 = 0;
    /*Estado inicial del PIC*/
    notOE = 1;
    notWE = 1;

    lcd_init(); // Inicializa el display

    printf("hola mundo");
    pause(100000);
    lcd_cls();

    lcd_cmd(LCD_SETDDADDR2);
    printf("cp ROM -> RAM");
    
    lcd_cmd(LCD_SETDDADDR1);
    pause(100000);

    unsigned int addr = 0;
    unsigned short count = 0;

    while(addr <= END_ROM_ADDR){
        STATUSbits.RP0 = 1;
        TRISD = 0xFF;
        STATUSbits.RP0 = 0;

        ADDRBUS = addr;
        notOE = 0;
        pause(10);

        FSR = PORTD;

        notOE = 1;
        PORTD = 0;
        
        STATUSbits.RP0 = 1;
        TRISD = 0x0;
        STATUSbits.RP0 = 0;

        ADDRBUS |= START_RAM_ADDR;

        PORTD = FSR;
        notWE = 0;
        pause(10);
        notWE = 1;

        if(count++ == 16){
            lcd_cmd(LCD_SETDDADDR2);
        } else if(count == 32){    
            lcd_cls();
            count = 0;            
        }

        printf("%x",FSR);

        addr++;
    }

    while (1);
}

void pause(unsigned int mseconds) {
    unsigned long count = mseconds * MHZ * 100;
    while (count--);
}