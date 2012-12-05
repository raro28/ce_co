/******************************************************************************            
 ************             LABCENTER ELECTRONICS                      ***********
 ************	       Proteus VSM Sample Design Code               ***********
 ************	           IAR 'C' Calculator                 	    ***********
 ******************************************************************************/
//Display Config.
#define MAX_DISPLAY_char 16

// Keypad Config.
#define KEYP_NUM_ROWS 4
#define KEYP_NUM_COLS 4

#define FALSE 0
#define TRUE  1                                                       

//Error handling status.

enum ERROR {
    OK = 0, SLEEP = 1, ERROR = 2
};

/************************************************************************                   
 ***** FUNCTION PROTOTYPES *****
 ******************************/
void keypad_init();
char keypad_getkey();

//Utility functions.                                                                        
signed char keypad_testkey(char);

// Keypad Functions.
char keypadread();
char scankeypad();