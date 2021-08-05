
/*
 * Archivo:   PUSH.c
 * Dispositivo: PIC16F887
 * Autor: Brandon Garrido
 */

#include <xc.h>
#include <stdint.h>
#include "PUSH.h"

//------------------------------------------------------------------------------
//                          Directivas del compilador
//------------------------------------------------------------------------------
#define _XTAL_FREQ 8000000 //Para delay

void init_push() {
    
    OPTION_REGbits.nRBPU =  0 ; // se habilita el pull up interno en PORTB
    WPUB = 0x03;  // se habilita los pull ups para los pines RB0 y RB1 
    
    INTCONbits.RBIE = 1; // habilitar banderas de interrupción puertos B
    INTCONbits.RBIF = 0; 	
    
    IOCB = 0x03; // setear interrupciones en los pines RB0 y RB1 
    
    return;
}

