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

//Variables
static float lvalue;
static float rvalue;
static char lastop;

void calc_evaluate() {
    char number[MAX_DISPLAY_char + 1], key;
    signed char pos;
    float tmp;

    // Initialize static values:
    lvalue = 0;
    rvalue = 0;
    lastop = 0;

    // Display a Zero to start:
    calc_format(0);

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
                calc_display(number);
            }
        } else { // If a number has been entered, then evaulate it.
            // The number is stored to lvalue if we have no current operator,
            // or else rvalue if we do.
            if (pos != 0) {
                tmp = atof(number);
                if (lastop == 0)
                    lvalue = tmp;
                else
                    rvalue = tmp;
            }

            // Reset the input buffer.
            pos = 0;

            // Process the Command - LastOp holds the last actual operator;
            if (lastop != 0)
                calc_opfunctions(lastop);
            if (key != '=')
                lastop = key;
            else
                lastop = 0;
        }
    }
}

void calc_opfunctions(char token)
// Handle the operations. Lvalue holds the result and we test for
// consecutive operator presses.
{
    signed char result = OK;
    switch (token) {
            case '+': lvalue += rvalue;
            break;
        case '-': lvalue -= rvalue;
            break;
        case '*': lvalue *= rvalue;
            break;
        case '/':
            if (rvalue != 0)
                lvalue /= rvalue;
            else
                result = ERROR;
            break;
    }

    if (result == OK)
        calc_format(lvalue);
    else if (result == ERROR)
        calc_display("*ERROR*");
}

/************************************************************************
 ***** Utility Routines *****
 ***************************/

void calc_format(float f) {
    char buf [MAX_DISPLAY_char + 1];
    float divisor = 100000000;
    float digit;
    signed char pad = 0, p = 0;

    // Sort out minus sign:
    if (f >= 0)
        buf[p++] = ' ';
    else {
        buf[p++] = '-';
        f = -f;
    }

    if (f >= divisor)
        buf[p++] = 'E';
    else
        while (p < MAX_DISPLAY_char && (divisor > 1 || f >= 0.0000001)) {
            divisor /= 10;
            digit = floor(f / divisor);
            if (divisor < 1 && divisor > 0.01)
                buf[p++] = '.';
            if (digit != 0 || divisor < 10) {
                buf[p++] = digit + '0';
                pad = TRUE;
            } else if (pad)
                buf[p++] = '0';
            f -= digit*divisor;
        }
    buf[p] = 0;
    calc_display(buf);
}

signed char calc_testkey(char key)
// Test whether the key is a digit, a decimal point or an operator. 
// Return 1 for digit or decimal point, 0 for op.
{
    if ((key == '.') || ((key >= '0') && (key <= '9')))
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

void calc_display(char *buf)
// Use the Output and Clearscreen routines from the 
// *LCD_Write* assembly file to output ASCII values to the LCD.
{
    signed char i;
    lcd_cls();
    for (i = 0; buf[i] != 0; i++)
        //    { if (buf[calc_testkey(buf[i]) || buf[i] == 0x2D)
    {
        putch(buf[i]);
    }
    //    }
}