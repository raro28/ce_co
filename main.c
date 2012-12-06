/*
 * File:   main.c
 * Author: ekthor
 *
 * Created on November 30, 2012, 9:57 AM
 */

#pragma config BOREN = OFF, CPD = OFF, DEBUG = OFF, WRT = OFF, FOSC = XT, WDTE = OFF, CP = OFF, LVP = OFF, PWRTE = OFF
#include <xc.h>
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

    lcd_init();

    OPTION_REGbits.nRBPU = 0;
    INTCONbits.GIE = 1;
    INTCONbits.RBIE = 1;

    while (1);
}

void search(unsigned char *inputStream) {
    ADDRBUS = 0;
    short hitCount = 0;
    short hitLimit = (MAX_DISPLAY_char) / 2;

    while (ADDRBUS <= END_ROM_ADDR && (hitCount < hitLimit)) {
        DATABUS = 0;
        TRISB = 0xFF;
        notOE = 0;
        unsigned char data = DATABUS;
        notOE = 1;

        unsigned char next = nextByte(inputStream, hitCount);

        if (data == next) {
            hitCount++;
        } else {
            hitCount = 0;
        }

        ADDRBUS++;
    }

    lcd_cls();
    
    if (hitCount == 4) {
        printf("Found (^_^)");
        lcd_cmd(LCD_SETDDADDR2);
        for (short dataCount = 0; dataCount < 8; dataCount++) {
            ADDRBUS++;

            DATABUS = 0;
            TRISB = 0xFF;
            notOE = 0;
            unsigned char data = DATABUS;
            notOE = 1;

            printf("%c", data);
        }
    } else {
        printf("Not Found (U_u)");
    }

    __delay_ms(5000);
    lcd_cls();
}

unsigned char nextByte(unsigned char *ptr, short hitCount) {
    unsigned char shift = 2 * hitCount;

    unsigned char high = ptr[shift];
    high -= ((high > 0x39) ? 0x57 : 0x30);
    high <<= 4;

    unsigned char low = ptr[shift +1];
    low -= ((low > 0x39) ? 0x57 : 0x30);

    return high | low;
}