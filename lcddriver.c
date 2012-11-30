/*******************************************************************
 ********************************************************************
 ********************************************************************
 *****                                                          *****
 *****        L A B C E N T E R    E L E C T R O N I C S        *****
 *****                                                          *****
 *****       LABCENTER INTEGRATED SIMULATION ARCHITECTURE       *****
 *****                                                          *****
 *****          PIC1687* Driver for LCD in 4-bit mode           *****
 *****                                                          *****
 ********************************************************************
 ********************************************************************/
// These driver routines implement the functionality necessary to drive
// a HD44780 controlled Alphameric LCD in 4-bit mode.
// PORT C pins 4-7 are used for data transmission and PORT B pins 0-2 are used
// for command control.

// The Custom Graphic characters are written in 8-bytes blocks to the CGRAM. Subsequent
// to writing they are accessed via an index in DDRAM - that is, the custom graphic located
// at CGRAM address 40h-47h is accessed by writing to character font location 0x00 in DDRAM,
// the custom graphic located at 48-4F is accessed by writing to character font location 0x01
// in DDRAM and so on.

#include "lcd8bit.h"
#include "minimal.h"

void lcd_init() {
    wrcmd(LCD_SETFUNCTION);
    wrcmd(LCD_OFF);
    wrcmd(LCD_CURSORON);
    wrcmd(LCD_ENTRYMODE);
    wrcmd(LCD_SHIFT);
    wrcmd(LCD_CLS);

    wrcmd(LCD_SETDDADDR1);
}

void clearscreen() {
    wrcmd(LCD_CLS);
    wrcmd(LCD_SETDDADDR1 + 0x00);
}

void wrcmd(char cmdcode) {
    DATABUS = cmdcode;

    RS = RW = 0;

    E = 1;
    asm("NOP");
    E = 0;

    lcd_wait();
}

void wrdata(char data) {
    DATABUS = data;

    RS = 1;
    RW = 0;

    E = 1;
    asm("NOP");
    E = 0;

    lcd_wait();
}

void wrcgchr(unsigned char *arrayptr, int offset) {
    wrcmd(LCD_SETCGADDR + offset); // Set the CG RAM address.

    while (*arrayptr != '\n') {
        wrdata(*arrayptr++);
    }
}

void lcd_wait() {
    unsigned char status = 0;

    STATUSbits.RP0 = 1;
    TRISD = 0xFF;
    STATUSbits.RP0 = 0;

    do {
        RS = 0;
        RW = 1;
        E = 1;
        asm("NOP");
        status = DATABUS; // read the status
        E = 0;
    } while (status & 0x80); // test busy flag.

    STATUSbits.RP0 = 1;
    TRISD = 0;
    STATUSbits.RP0 = 0;
}