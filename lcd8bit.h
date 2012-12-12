#define LCD_RS RA3 //Write Enable
#define LCD_E RA4 //Write Enable

//LCD Commands
#define LCD_SETFUNCTION	   0x38
#define LCD_OFF 	   0x08
#define LCD_CURSORON 	   0x0e
#define LCD_ENTRYMODE 	   0x06
#define LCD_SHIFT	   0x14
#define LCD_CLS		   0x01

#define LCD_SETDDADDR1	   0x80
#define LCD_SETDDADDR2	   0xC0

#define E_DELAY_MS 130
#define CMD_DELAY_MS 10

/************************************************************************                   
 ***** FUNCTION PROTOTYPES *****
 ******************************/
void lcd_init();
void lcd_cmd(char data);
void lcd_cls();
void putch(char data);