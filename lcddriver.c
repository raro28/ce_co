#include "extra.h"
#include "lcd8bit.h"

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
    lcd_cmd(LCD_SETDDADDR1);
}

void lcd_cmd(char cmdcode) {
    TRISB = 0;
    UNIVERSAL_BUS = cmdcode;

    LCD_RS = 0;

    LCD_E = 1;
    __delay_ms(E_DELAY_MS);
    LCD_E = 0;

    __delay_ms(CMD_DELAY_MS);
}

void putch(char c) {
    TRISB = 0;
    UNIVERSAL_BUS = c;

    LCD_RS = 1;

    LCD_E = 1;
    __delay_us(E_DELAY_MS);
    LCD_E = 0;

    UNIVERSAL_BUS = 0;
    __delay_ms(CMD_DELAY_MS);
}