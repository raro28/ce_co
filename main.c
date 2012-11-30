/* 
 * File:   main.c
 * Author: ekthor
 *
 * Created on November 30, 2012, 9:57 AM
 */
#pragma config BOREN = OFF, CPD = OFF, DEBUG = OFF, WRT = OFF, FOSC = XT, WDTE = OFF, CP = OFF, LVP = OFF, PWRTE = OFF
#include "lcd8bit.h"
#include "minimal.h"

char text[] = "123456789ABCDEFGHIJKLMNOPQRSTUVWXZ\n";

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

    lcd_init(); // Initialise the LCD Display

    char *textptr = text;

    textptr = text;
    while (*textptr != '\n') // Write the String.
    {
        wrdata(*textptr++);
    }
}