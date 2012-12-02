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

    // Clear the buffer before we start.
    pos = 0;

    for (;;) {
        key = calc_getkey();
        if (calc_testkey(key)) { // Key test positive for digit so we read it into the
            // buffer and then write the buffer to the screen/LCD.
            if (pos != MAX_DISPLAY_char) {
                number[pos++] = key;
                number[pos] = 0;
                lcd_cls();
                printf(number);
            }
        } else {        
        }
    }
}

signed char calc_testkey(char key)
{
    if (((key >= '0') && (key <= '9')))
        return TRUE;
    else
        return FALSE;
}

/************************************************************************
 ***** I/O Routines *****
 ************************/

char calc_getkey()
{
    char mykey;
    while ((mykey = keypadread()) == 0x00)
        /* Poll again */;
    return mykey;
}