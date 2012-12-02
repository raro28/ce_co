/******************************************************************************            
 ************                 LABCENTER ELECTRONICS                  ************
 ************           Proteus VSM Sample Design Code               ************
 ************                   IAR 'C' Calculator                   ************
 *******************************************************************************/

#include <xc.h>
#include "keypad.h"
#include "stdio.h"
#include "lcd8bit.h"

// Rows are connected to PortD[0..3]
// Columns are connected to PortD[4..7] with external pull-up resistors.

char keycodes[16] = {'7', '8', '9', '/', '4', '5', '6', '*', '1', '2', '3', '-', '.', '0', '=', '+'};

void keypad_init() {
    char number[MAX_DISPLAY_char + 1], key;
    signed char pos = 0;

    for (;;) {
        key = keypad_getkey();
        if (keypad_testkey(key)) { // Key test positive for digit so we read it into the
            // buffer and then write the buffer to the screen/LCD.
            if (pos != MAX_DISPLAY_char) {
                number[pos++] = key;
                number[pos] = 0;
                lcd_cls();
                printf(number);
            }
        } else {
            lcd_cls();
            printf("funcion %d", key);
        }
    }
}

signed char keypad_testkey(char key)
{
    if (((key >= '0') && (key <= '9')))
        return TRUE;
    else
        return FALSE;
}

/************************************************************************
 ***** I/O Routines *****
 ************************/

char keypad_getkey()
{
    char mykey;
    while ((mykey = keypadread()) == 0x00)
        /* Poll again */;
    return mykey;
}


char keypadread()
{
    char key = scankeypad();
    if (key)
        while (scankeypad() != 0)
            /* Nothing */;
    return key;
}

char scankeypad()
{
    signed char row, col, tmp;
    char key = 0;

    // Initialise PortD[4..7] for input, and PORTD[0..3] for output
    STATUSbits.RP0 = 1;
    TRISD = 0xF0;
    STATUSbits.RP0 = 0;
    PORTD = 0x0F;

    for (row = 0; row < KEYP_NUM_ROWS; row++) { // Drive appropriate row low and read columns:
        PORTD = ~(1 << row);
        asm ( "NOP");
        tmp = PORTD >> 4;

        // See if any column is active (low):
        for (col = 0; col < KEYP_NUM_COLS; ++col)
            if ((tmp & (1 << col)) == 0) {
                signed char idx = (row * KEYP_NUM_COLS) + col;
                key = keycodes[idx];
                break;
            }
    }

    STATUSbits.RP0 = 1;
    TRISD = 0x00;
    STATUSbits.RP0 = 0;
    PORTD = 0;

    return key;
}