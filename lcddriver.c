/*******************************************************************
 ********************************************************************
 ********************************************************************
 *****                                                          *****
 *****        L A B C E N T E R    E L E C T R O N I C S        *****
 *****                                                          *****
 *****       LABCENTER INTEGRATED SIMULATION ARCHITECTURE       *****
 *****                                                          *****
 *****          PIC1687* Driver for LCD in 8-bit mode           *****
 *****                                                          *****
 ********************************************************************
 ********************************************************************/
// These driver routines implement the functionality necessary to drive
// a HD44780 controlled Alphameric LCD in 8-bit mode.
// PORT D pins 19-30 are used for data transmission and PORT A pins 4,5,7 are used
// for command control.

#include "lcd8bit.h"
#include "minimal.h"

void lcd_init() {
    lcd_cmd(LCD_SETFUNCTION);
    lcd_cmd(LCD_OFF);
    lcd_cmd(LCD_CURSORON);
    lcd_cmd(LCD_ENTRYMODE);
    lcd_cmd(LCD_SHIFT);
    lcd_cmd(LCD_CLS);

    lcd_cmd(LCD_SETDDADDR1);
}

void lcd_cls() {
    lcd_cmd(LCD_CLS);
    lcd_cmd(LCD_SETDDADDR1 + 0x00);
}

void lcd_cmd(char cmdcode) {
    TRISB = 0;
    DATABUS = cmdcode;

    LCD_RS = LCD_RW = 0;

    LCD_E = 1;
    my_msdelay(130);
    LCD_E = 0;

    lcd_wait();
}

void putch(char c) {
    TRISB = 0;
    DATABUS = c;

    LCD_RS = 1;
    LCD_RW = 0;

    LCD_E = 1;
    my_usdelay(130);
    LCD_E = 0;

    DATABUS = 0;
    lcd_wait();
}

void lcd_wait() {
    unsigned char status = 0;

    TRISB = 0xFF;

    do {
        LCD_RS = 0;
        LCD_RW = 1;

        LCD_E = 1;
        my_usdelay(130);
        status = DATABUS; // read the status
        LCD_E = 0;
        
    } while (status & 0x80); // test busy flag.

    TRISB = 0xF0;
}