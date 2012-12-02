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
void calc_evaluate();
char calc_getkey();

//Utility functions.                                                                        
signed char calc_testkey(char ch);

// Keypad Functions.
char keypadread();
char scankeypad();