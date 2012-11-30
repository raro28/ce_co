/********************************************************************/
/********************************************************************/
/*****                                                          *****/
/*****        L A B C E N T E R    E L E C T R O N I C S        *****/
/*****                                                          *****/
/*****       LABCENTER intEGRATED SIMULATION ARCHITECTURE       *****/
/*****                                                          *****/
/*****             Header File for 8-Bit LCD Sample             *****/
/*****                                                          *****/
/********************************************************************/
/********************************************************************/

//LCD Wiring
#define RS RA2 //Write Enable
#define RW RA3 //Write Enable
#define E RA5 //Write Enable

//Display Config.
#define MAX_DISPLAY_CHAR 16

//LCD Commands
#define LCD_SETFUNCTION	   0x38
#define LCD_OFF 	   0x08
#define LCD_CURSORON 	   0x0e
#define LCD_ENTRYMODE 	   0x06
#define LCD_SHIFT	   0x14
#define LCD_CLS		   0x01

#define LCD_SETDDADDR1	   0x80
#define LCD_SETDDADDR2	   0xC0

/************************************************************************                   
 ***** FUNCTION PROTOTYPES *****
 ******************************/
void lcd_init();
void lcd_wait();
void clearscreen();
void wrcmd(char data);
void wrdata(char data);