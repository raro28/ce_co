#include <xc.h>
#include "keypad.h"
#include "stdio.h"
#include "lcd8bit.h"
#include "minimal.h"

char keyvalues[16] = {'7', '4', '1', 'e', '8', '5', '2', 0x30 , '9', '6', '3', 'f', 'a', 'b', 'c', 'd'};
char number[MAX_DISPLAY_char];
char pos = 0;

void interrupt keypad_int() {
    unsigned char read = PORTB;
    PORTB = read;
    if (INTCONbits.RBIF) {
        INTCONbits.RBIF = 0;
        __delay_ms(10);
        unsigned char key = scankeypad();

        if (key != 0x10) {
            if (pos != MAX_DISPLAY_char) {
                number[pos++] = key;
                number[pos] = 0;
                lcd_cls();
                printf(number);
            } else {
                search(number);
                pos = 0;
            }
        }
    }

    PORTB &= 0xF0;
}

char scankeypad() {
    signed char row, col, tmp;
    unsigned char key = 0x10;

    for (row = 0; row < KEYP_NUM_ROWS; row++) { // Drive appropriate row low and read columns:
        PORTB = ~(1 << row);
        asm ( "NOP");
        tmp = PORTB >> 4;

        // See if any column is active (low):
        for (col = 0; col < KEYP_NUM_COLS; ++col)
            if ((tmp & (1 << col)) == 0) {
                signed char idx = (row * KEYP_NUM_COLS) + col;
                key = keyvalues[idx];
                break;
            }
    }

    return key;
}