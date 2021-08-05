/*
 * Archivo:   lab3_main.c
 * Dispositivo: PIC16F887
 * Autor: Brandon Garrido 
 * 
 * Programa: Comunicación esclavo-maestro SPI / USART
 * Hardware: 
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
 #include "USART.h"

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
uint8_t pot = 1;
uint8_t pot1; //Para ADC
uint8_t pot2;
uint8_t RC_temp; //Variable que recibe los datos del USART
  
uint8_t RC_temp;
uint8_t u_flag = 1;
uint8_t d_flag = 0;
uint8_t c_flag = 0;
uint8_t unidad = 0;
uint8_t decena = 0;

uint8_t contador=0; 
uint8_t cont_temp=0;
uint8_t cont; 

uint8_t disp__unidad; //Para desplegar el valor de los potenciómetros con
uint8_t disp__decimal;//dos decimales
uint8_t disp__decimal_2;
uint8_t disp__unidad_u2;
uint8_t disp__decimal_u2;
uint8_t disp__decimal2_u2;
int8_t flag; //Bandera para saber que dato enviar por el USART
//------------------------------------------------------------------------------
//                          Prototipos
//------------------------------------------------------------------------------
void setup(void);  //Configuración

//------------------------------------------------------------------------------
//                          Código Principal
//------------------------------------------------------------------------------
void main(void) {
    setup(); //Configuración
    while(1){
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(pot);
       if (pot == 1) {
       pot2 = spiRead();
       pot = 2;
       }
       else if (pot == 2) {
       pot1 = spiRead();
       pot = 1;
       }
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect       
       __delay_ms(250);
       
    if(cont > 15){ //Se reinicia el contador después de 45ms y se enciende
         cont = 0; //el enable para enviar datos via USART
         TXIE = 1; 
     }
     
    //Conversiones para mostrar contador de leds
    disp__unidad = pot1 / 51;
    disp__decimal = ((pot1 * 100 / 51) - (disp__unidad*100))/10;
    disp__decimal_2 = ((pot1 * 100 / 51) - (disp__unidad*100) - (disp__decimal*10));  
    
    disp__unidad_u2 = pot2 / 51;
    disp__decimal_u2 = (((pot2 * 100) / 51) - (disp__unidad_u2*100))/10;
    disp__decimal2_u2 = (((pot2 * 100) / 51) - (disp__unidad_u2*100) - (disp__decimal_u2*10));

    
    if (PORTAbits.RA1 == 1){
        cont_temp = 0;
        contador = 0;
        PORTA = 0;
        u_flag = 1;
        d_flag = 0;
        c_flag = 0;
    }
    }
    return;
}

//------------------------------------------------------------------------------
//                          Interrupciones
//------------------------------------------------------------------------------
void __interrupt() isr(void){
    if (INTCONbits.T0IF){           // INTERRUPCION TMR0
        cont++;
        INTCONbits.T0IF = 0;        // Limpiar la bandera de interrupción TMR0
    }
        
    if(PIR1bits.RCIF == 1){ //Empieza a recibir datos del USART
        //0x0A para el salto de linea \n
        if (RCREG ==  0x0D){
        PORTD = contador; 
        PORTA =2;
        }
        
        if (RCREG !=  0x0D){
        RC_temp = RCREG;
        
        switch(RC_temp){
            case 48:
                cont_temp = 0;
                break;
            case 49:
                cont_temp = 1;
                break;
            case 50:
                cont_temp = 2;
                break;
            case 51:
                cont_temp = 3;
                break;
            case 52:
                cont_temp = 4;
                break;
            case 53:
                cont_temp = 5;
                break;
            case 54:
                cont_temp = 6;
                break;
            case 55:
                cont_temp = 7;
                break;
            case 56:
                cont_temp = 8;
                break;       
            case 57:
                cont_temp = 9;
                break;
        }
            
        
        if (u_flag == 1){
            contador = cont_temp;
            unidad = cont_temp;
            u_flag = 0;
            d_flag = 1;
        }
        else if (d_flag == 1){
            contador = (unidad*10)+cont_temp;
            decena = cont_temp;
            d_flag = 0;
            c_flag = 1;
        }
        else if (c_flag == 1){
            contador = (unidad*100)+(decena*10)+cont_temp;
            d_flag = 0;
            c_flag = 1;
        }     
        }}
    
        
    //recibir datos del pot
    if (TXIF == 1){
        if (flag == 0){//Envía los datos al USART de los dos potenciómetros
            TXREG = disp__unidad + 48; //Envía las unidades del POT1
            flag = 1;
        } else if (flag == 1){
            TXREG = 0x2E;
            flag = 2;
        } else if (flag == 2){
            TXREG = disp__decimal + 48; //Envía el primer decimal del POT1
            flag = 3;
        } else if (flag == 3){
            TXREG = disp__decimal_2 + 48; //Envía el segundo decimal del POT1
            flag = 4;
        } else if (flag == 4){
            TXREG = 0x2D;
            flag = 5;
        }
        else if (flag == 5){
            TXREG = disp__unidad_u2 + 48;//Envía las unidades del POT2
            flag = 6;
        } else if (flag == 6){
            TXREG = 0x2E;
            flag = 7;
        } else if (flag == 7){
            TXREG = disp__decimal_u2 + 48; //Envía el primer decimal del POT2
            flag = 8;
        } else if (flag == 8){
            TXREG = disp__decimal2_u2 + 48; //Envía el segundo decimal del POT2
            flag = 9;
        } else if (flag == 9){
            TXREG = 0x0D;
            flag = 0;
        }       
    TXIF = 0; //Se limpia la bandera
    }   
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
    ANSEL = 0x00; //Pin analógico para POT
    
    TRISA = 0x00; //Para salida del contador
    TRISB = 0x00;
    TRISC = 0x90; //Para salida del display
    TRISD = 0x00; //Para transistores
    TRISE = 0x00; //Para led de alarma y potenciometro
    
    PORTA = 0x00; //Se limpian los puertos
    PORTB = 0x00;
    PORTC = 0x00;    
    PORTD = 0x00;
    PORTE = 0x00;
        
   //Configurar la interrupcion
    INTCONbits.GIE = 1;  //Enable interrupciones globales
    INTCONbits.T0IE = 1;           
    INTCONbits.T0IF = 0; 
        
    //Configuración de TX y RX
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 207;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;//Configuración del USART y Baud Rate
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    TXSTAbits.TXEN = 1; 
    
    //Configurar TMR0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1; //Prescaler 1:256
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    TMR0 = 10;  //Se reinicia el TMR0
    
    TRISC2 = 0;
    PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}
