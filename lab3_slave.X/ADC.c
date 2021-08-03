/*
 * Archivo:   ADC.c
 * Dispositivo: PIC16F887
 * Autor: Brandon Garrido
 */

#include <xc.h>
#include <stdint.h>
#include "ADC.h"

//------------------------------------------------------------------------------
//                          Directivas del compilador
//------------------------------------------------------------------------------
#define _XTAL_FREQ 8000000 //Para delay

void ADC() {
        //Configurar ADC
        ADCON1bits.ADFM = 0; //Justificar a la izquierda
        ADCON1bits.VCFG0 = 0; //Vss
        ADCON1bits.VCFG1 = 0; //VDD

        ADCON0bits.ADCS = 0b10; //ADC oscilador -> Fosc/32
        ADCON0bits.CHS = 0;     //Comenzar en canal 0       
        ADCON0bits.ADON = 1;    //Habilitar la conversión ADC
        __delay_us(50); 
        ADCON0bits.GO = 1;
    return;
}

