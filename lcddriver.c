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

void lcd_init()
// Initialise the LCD Display.
{
    wrcmd(LCD_SETFUNCTION); // 8-bit mode - 2 line - 5x7 font.
    wrcmd(LCD_SETVISIBLE + 0x04); // Display no cursor - no blink.
    wrcmd(LCD_SETMODE + 0x02); // Automatic Increment - No Display shift.

    wrcmd(LCD_SETDDADDR); // Address DDRAM with 0 offset 80h.
}

void clearscreen()
// Clear the LCD Screen and reset
// initial position.
{
    wrcmd(LCD_CLS);
    wrcmd(LCD_SETDDADDR + 0x00);
}

void wrcmd(char cmdcode) {
    DATABUS = cmdcode;
    RS = RW = 0; // Specify a command write operation.
    E = 1; // Toggle the 'E' pin to send the command.
    asm("NOP");
    E = 0;

    lcd_wait(); // Call the wait routine.
}

void wrdata(char data)
// Write a character to the LCD Display.
// Then call the wait routine to hold
// until the busy flag is cleared.
{
    DATABUS = data;
    RS = 1;
    RW = 0; // Specify a data write operation.
    E = 1; // Toggle the 'E' pin to send the command.
    asm("NOP");
    E = 0;

    lcd_wait(); // Call the wait routine.
}

void wrcgchr(unsigned char *arrayptr, int offset)
// Subroutine to write a custom graphic character
// into CGRAM. We take a pointer to the array of bytes
// and an offset into CGRAM at which to place the character.
{
    wrcmd(LCD_SETCGADDR + offset); // Set the CG RAM address.

    while (*arrayptr != '\n') {
        wrdata(*arrayptr++);
    }
}

void lcd_wait()
// Wait for the LCD busy flag to clear.
{
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
