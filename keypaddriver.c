/******************************************************************************            
 ************                 LABCENTER ELECTRONICS                  ************
 ************           Proteus VSM Sample Design Code               ************
 ************                   IAR 'C' Calculator                   ************
 *******************************************************************************/

#include <xc.h>
#include "keypad.h"
#include "stdio.h"
#include "lcd8bit.h"
#include "minimal.h"

// Rows are connected to PortD[0..3]
// Columns are connected to PortD[4..7] with external pull-up resistors.

char keyvalues[16] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};

void interrupt keypad_int() {
    unsigned char read = PORTB;
    if (INTCONbits.RBIF) {
        INTCONbits.RBIF = 0;
        __delay_us(100);
        unsigned char key = scankeypad();

        if (key != 0x10) {
            printf("%x", key);
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