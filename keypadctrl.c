/*********************************************************************
 **********************************************************************
 *****                                                             ****
 *****         L A B C E N T E R    E L E C T R O N I C S          ****
 *****                                                             ****
 *****                  PIC 16F87X Keypad Scanner                  ****
 *****                                                             ****
 **********************************************************************
 *********************************************************************/

// Rows are connected to Port C 
// Columns are connected to Port A with external pull-up resistors.

#include <xc.h>
#include "calc.h"

char keycodes[16] = {'7', '8', '9', '/', '4', '5', '6', '*', '1', '2', '3', '-', '.', '0', '=', '+'};

char keypadread()
// Find a key, wait for
// it to be released and return.
{
    char key = scankeypad();
    if (key)
        while (scankeypad() != 0)
            /* Nothing */;
    return key;
}

char scankeypad()
// Scan the keypad for a keypress.
// Return 0 for no press or the char pressed.
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
    
    return key;
}