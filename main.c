/* 
 * File:   main.c
 * Author: ekthor
 *
 * Created on November 30, 2012, 9:57 AM
 */
#pragma config BOREN = OFF, CPD = OFF, DEBUG = OFF, WRT = OFF, FOSC = XT, WDTE = OFF, CP = OFF, LVP = OFF, PWRTE = OFF

#include <stdlib.h>
#include <xc.h>
#include "lcd4bit.h"

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

#define RS RA2 //Write Enable
#define RW RA3 //Write Enable
#define E RA5 //Write Enable

#define DATABUS PORTD //Bus de datos conformado por el puerto b y c

/*
 * Bus de direcciones conformado por el puerto b y c
 */
extern volatile unsigned int ADDRBUS @ 0x006;


BYTE pacmanopen[] = {0x0E, 0x07, 0x03, 0x01, 0x03, 0x07, 0x0E, 0x00, '\n'};
BYTE pacmanshut[] = {0x00, 0x0F, 0x1F, 0x01, 0x1F, 0x0F, 0x00, 0x00, '\n'};

void lcd_init()
// Initialise the LCD Display.
{
    wrcmd(LCD_SETFUNCTION); // 8-bit mode - 2 line - 5x7 font.
    wrcmd(LCD_SETVISIBLE + 0x04); // Display no cursor - no blink.
    wrcmd(LCD_SETMODE + 0x02); // Automatic Increment - No Display shift.

    // Write Custom character to CG RAM.
    wrcgchr(pacmanopen, 0); // Offset 0 - 40h-47h in CGRAM.
    wrcgchr(pacmanshut, 8); // Offset 8 - 48h-4Fh in CGRAM.

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

void wrcgchr(BYTE *arrayptr, int offset)
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
    BYTE status = 0;
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

void eat()
// Show the pacman and 'eat' the text.
{
    int count = MAX_DISPLAY_CHAR - 1, eatflag = 0, tmp;
    while (count >= 0) {
        wrcmd(LCD_SETDDADDR + count); // Work from right to left.
        eatflag ? wrdata(0x00) : wrdata(0x01); // Toggle the custom graphics between mouth open and mouth shut.
        eatflag ^= 1;

        for (tmp = count; tmp <= MAX_DISPLAY_CHAR; ++tmp) // Anything to the right of pacman is wiped.
            wrdata(' ');

        count--; // Decrement the count (moving us 1 to the left).
        pause(4000);
    }
    clearscreen(); // Clear the final pacman graphic when we are done.
}

void pause(int num)
// Utility routine to pause for
// a period of time.
{
    while (num--) {/*do nothing */
    }
}

char text[] = {'4','-','B','i','t',' ','L','C','D',' ','M','o','d','e', ' ','\n'};

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

    for (;;) // Loop Forever.
    {
        textptr = text;
        while (*textptr != '\n') // Write the String.
        {
            wrdata(*textptr++);
            pause(2000);
        }
        pause(5000);
        eat(); // Show and Animate the pacman.
        pause(5000);
    }
}