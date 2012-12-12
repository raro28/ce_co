
/*
 *Intervalo de direcciones de la memoria RAM.
 */
#define START_ROM_ADDR 0b0000000000000000
#define END_ROM_ADDR   0b0000011111111111

/*
 * Bus usado pra las partes alta y baja del bus de direcciones
 *  y los datos.
 */
#define UNIVERSAL_BUS PORTB
#define AL0 RA0 //Address Latch 0
#define AL1 RA1 //Address Latch 1
#define notOE RA2 //Output Enable

#define _XTAL_FREQ 4000000 //4MHz