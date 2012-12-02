/******************************************************************************            
 ************                 LABCENTER ELECTRONICS                  ************
 ************           Proteus VSM Sample Design Code               ************
 ************                   IAR 'C' Calculator                   ************
 *******************************************************************************/

#include "calc.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "lcd8bit.h"

void calc_evaluate() {
    char number[MAX_DISPLAY_char + 1], key;
    signed char pos;
    float tmp;

    // Clear the buffer before we start.
    pos = 0;

    for (;;) {
        key = calc_getkey();
        if (calc_testkey(key)) { // Key test positive for digit so we read it into the
            // buffer and then write the buffer to the screen/LCD.
            // Size limit the number of digits - allow for termination
            // and possible negative results.
            if (pos != MAX_DISPLAY_char - 2) {
                number[pos++] = key;
                number[pos] = 0;
                printf(number);
            }
        } else {        
        }
    }
}

signed char calc_testkey(char key)
// Test whether the key is a digit, a decimal point or an operator. 
// Return 1 for digit or decimal point, 0 for op.
{
    if (((key >= '0') && (key <= '9')))
        return TRUE;
    else
        return FALSE;
}

/************************************************************************
 ***** I/O Routines *****
 ************************/

char calc_getkey(void)
// Use the input routine from the *Keypad_Read* assembly file to 
// Scan for a key and return ASCII value of the Key pressed.
{
    char mykey;
    while ((mykey = keypadread()) == 0x00)
        /* Poll again */;
    return mykey;
}