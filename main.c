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
    while (ADDRBUS <= END_ROM_ADDR && hitCount < 4) {
        DATABUS = 0;
        TRISB = 0xFF;
        notOE = 0;
        FSR = DATABUS;
        notOE = 1;

        int next = nextByte(inputStream);
        if(FSR = next){
            hitCount++;
        }else
        {
            inputStream = 0;
            hitCount = 0;
        }

        ADDRBUS++;
    }

    lcd_cmd(LCD_SETDDADDR2);

    if(hitCount == 8){                
        for(short dataCount= 0; dataCount < 8; dataCount ++)
        {
            ADDRBUS++;

            DATABUS = 0;
            TRISB = 0xFF;
            notOE = 0;
            FSR = DATABUS;
            notOE = 1;

            unsigned char ldata = FSR & 0x0F;
            ldata += 0x57;
            unsigned char hdata = FSR >> 4;
            hdata += 0x57;

            printf("%c", hdata);
            printf("%c", ldata);

            ADDRBUS++;
        }
    }else
    {
        printf("No Data U_U");
    }

    lcd_cmd(LCD_SETDDADDR1);
}

unsigned char nextByte(unsigned char *ptr)
{
    unsigned char byte = *(ptr++) - 0x57;
    byte <<= 4;
    byte |= *(ptr++);

    return byte;
}