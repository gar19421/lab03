/*
 * Archivo:   Lab03_main_master.c
 * Dispositivo: PIC16F887
 * Autor: Brandon Garrido 
 * 
 * Programa: Comunicación esclavo - maestro SPI / ADC
 * 
 * Creado: Agosto 02, 2021
 * Última modificación: Agosto 03, 2021
 */

//------------------------------------------------------------------------------
//                          Importación de librerías
//------------------------------------------------------------------------------    
 #include <xc.h>
 #include <stdint.h>
 #include "SPI.h"
 #include "ADC.h"

//------------------------------------------------------------------------------
//                          Directivas del compilador
//------------------------------------------------------------------------------
#define _XTAL_FREQ 8000000 //Para delay

//------------------------------------------------------------------------------
//                          Palabras de configuración
//------------------------------------------------------------------------------    
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT//Oscillator Selection bits(INTOSCIO 
                              //oscillator: I/O function on RA6/OSC2/CLKOUT pin, 
                              //I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT disabled and 
                          //can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR  
                                //pin function is digital input, MCLR internally 
                                //tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code 
                                //protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code 
                                //protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit 
                                //Internal/External Switchover mode is disabled
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
                                //(Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         //Low Voltage Programming Enable bit(RB3/PGM pin 
                                //has PGM function, low voltage programming 
                                //enabled)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out 
                                //Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 
                                //(Write protection off)

//------------------------------------------------------------------------------
//                          Variables
//------------------------------------------------------------------------------
uint8_t temporal = 0;
uint8_t POT1; //Para ADC
uint8_t POT2;
uint8_t flag_1; //Bandera para el ADC

//------------------------------------------------------------------------------
//                          Prototipos
//------------------------------------------------------------------------------
void setup(void);  //Configuración

//------------------------------------------------------------------------------
//                          Interrupciones
//------------------------------------------------------------------------------
void __interrupt() isr(void){
   
   if(ADIF == 1){
        if (flag_1 == 1){ 
            POT1 = ADRESH; //Se almacena el valor del primer potenciómetro
            ADCON0bits.CHS0 = 1;//Cambiar de canal
            flag_1 = 0;
        } else{
            POT2 = ADRESH; //Se almacena el valor del segundo potenciómetro
            ADCON0bits.CHS0 = 0;//Cambiar de canal
            flag_1 = 1;
        }
        
        ADIF = 0; //Limpiar la bandera de ADC
        __delay_us(60);
        ADCON0bits.GO = 1; //Inicia la conversión de ADC
    }  
   
   if(SSPIF == 1){
        temporal = spiRead();
        if(temporal == 1){
            spiWrite(POT1);
        }
        else if(temporal == 2){
            spiWrite(POT2);
        }
        SSPIF = 0;
    }
}
//------------------------------------------------------------------------------
//                          Código Principal
//------------------------------------------------------------------------------
void main(void) {
    setup();
    while(1){

    }
    return;
}
//------------------------------------------------------------------------------
//                          Configuración
//------------------------------------------------------------------------------
void setup(void){
    //Configuracion reloj
    OSCCONbits.IRCF2 = 1; //Frecuencia a 8MHZ
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
    
    //Configurar entradas y salidas
    ANSELH = 0x00;//Pines digitales
    ANSEL = 0x03; //Pin analógico para POT
    
    TRISA = 0x23; //Para salida del contador
    TRISC = 0x18; //Para salida del display
    TRISD = 0x00; //Para transistores
    TRISE = 0x00; //Para led de alarma y potenciometro
    
    PORTA = 0x00; //Se limpian los puertos
    PORTC = 0x00;    
    PORTD = 0x00;
    PORTE = 0x00;
    
    //Configurar ADC
    ADC();
    
   //Configurar la interrupcion
    INTCONbits.GIE = 1;  //Enable interrupciones globales
    INTCONbits.T0IE = 1;           
    INTCONbits.T0IF = 0; 
    INTCONbits.PEIE = 1; //Enable interrupciones periféricas
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
    PIE1bits.ADIE = 1;   //Enable interrupción ADC
    PIR1bits.ADIF = 0;   //Se limpia bandera de interrupción ADC
        
    //Configurar TMR0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1; //Prescaler 1:256
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    TMR0 = 10;  //Se reinicia el TMR0
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);  
}
