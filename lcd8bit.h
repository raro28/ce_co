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
#define LCD_SETVISIBLE	   0x08
#define LCD_CLS		   0x01
#define LCD_HOME	   0x02
#define LCD_SETMODE	   0x04
#define LCD_SHIFT	   0x10
#define LCD_SETCGADDR	   0x40
#define LCD_SETDDADDR	   0x80                                                    

//Error handling status.

enum ERROR {
    OK = 0, SLEEP = 1, ERROR = 2
};

/************************************************************************                   
 ***** FUNCTION PROTOTYPES *****
 ******************************/
void lcd_init();
void lcd_wait();
void clearscreen();
void wrcmd(char data);
void wrdata(char data);
void wrcgchr(unsigned char *arrayptr, int offset);
