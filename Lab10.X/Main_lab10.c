/*------------------------------------------------------------------------------
Archivo: mainsproject.s
Microcontrolador: PIC16F887
Autor: Andy Bonilla
Compilador: pic-as (v2.30), MPLABX v5.45
    
Programa: Laboratorio 10, comunicación serial con computadora y puertos
Hardware: PIC16F887
    
Creado: 3 de mayo de 2021    
Descripcion: portA y POrtD con Leds y puertos Tx y RX con terminal 
------------------------------------------------------------------------------*/
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// se declara oscilador interno
#pragma config WDTE = OFF       // Watchdog Timer apagado
#pragma config PWRTE = OFF      // Power-up Timer apagado
#pragma config MCLRE = OFF      // MCLRE apagado
#pragma config CP = OFF         // Code Protection bit apagado
#pragma config CPD = OFF        // Data Code Protection bit apagado
#pragma config BOREN = OFF      // Brown Out Reset apagado
#pragma config IESO = OFF       // Internal External Switchover bit apagado
#pragma config FCMEN = ON      // Fail-Safe Clock Monitor Enabled bit apagado
#pragma config LVP = ON        // low voltaje programming prendido

// CONFIG2
#pragma config BOR4V = BOR40V   // configuración de brown out reset
#pragma config WRT = OFF        // apagado de auto escritura de cÃ?Â³digo

/*-----------------------------------------------------------------------------
------------------------librerias a implementar ------------------------------
-----------------------------------------------------------------------------*/
#include <xc.h>
#include <stdint.h>
#define  _XTAL_FREQ 8000000  //se define el delay con FreqOsc 4Mhz

/*-----------------------------------------------------------------------------
-------------------------------- variables -----------------------------------
-----------------------------------------------------------------------------*/
unsigned char valores_ascii[] = 
{
    65, //A
    66, //B
    67, //C
    68, //D
    69, //E
    70, //F
    71, //G
    72, //H
    73, //I
    74, //J
    75, //K
    76, //L
    77, //M
    78, //N
    79, //O
    80, //P
    81, //Q
    82, //R
    83, //S
    84, //T
    85, //U
    86, //V
    87, //W
    88, //X
    89, //Y
    90 //Z    
};

unsigned int contador = 0; // varible para incrementar

/*-----------------------------------------------------------------------------
-------------------------prototipos de funciones-------------------------------
-----------------------------------------------------------------------------*/
void setup(void);  //funcion para configuracion de registros del PIC
void __interrupt() isr(void) //funcion de interrupciones
{
    //interrupcion de recibir datos
    if(PIR1bits.RCIF)
    {
        PORTB= RCREG;    //PORTB toma el valor de recepcion
        __delay_us(100);
        PORTA= RCREG;
        
    }
    
}
/*-----------------------------------------------------------------------------
----------------------------- ciclo principal----------------------------------
-----------------------------------------------------------------------------*/
void main(void)  //funcion principal sin retorno
{  
    setup();                    //Configuraciones generales del PIC
    //---------------------loop principal del programa ------------------------
    while(1)            //se hace loop infinito mientras sea  
    {
        __delay_ms(200);
        if (PIR1bits.TXIF)
        {
            TXREG = valores_ascii[contador] ;
            //TXREG = 'A';
            contador++;
            if (contador==26)
            {
                contador=0;
            }
        }
        
    }                  
}
/*-----------------------------------------------------------------------------
--------------------------- configuraciones ----------------------------------
-----------------------------------------------------------------------------*/
void setup(void) //FUNCION PARA CONFIGURACION DE ENTRADAS Y SALIDAS
{
    //CONFIGURACION DE ENTRADAS/SALIDAS DIGITALES
    ANSEL = 0;                  //no hay entradas analógicas
    
    //CONFIGURACION DE ENTRADAS/SALIDAS ANLAGÓGICAS
    TRISA = 0;                  //se define PortA como salida
    TRISD = 0;                  // se define PortC como salida
    PORTA=0;                    // se limpia PortA
    PORTD=0;                    // se limpia PortC 
    
    //CONFIGURACION DEL OSCILADOR
    OSCCONbits.IRCF=0b111;         //8MHz 111
    OSCCONbits.SCS=1;           //configuracion de oscilador interno
            
    //CONFIGURACION DE TRANSMISION UART
    //TXSTAbits.TXEN = 1;         //se habilita la transmisión
    TXSTAbits.SYNC = 0;         //se habilita modo asíncrono
    TXSTAbits.BRGH = 1;         //transmisión rápida
    
    BAUDCTLbits.BRG16 = 1;      //generador de 16bits de Baud rate
    SPBRGH=0x00;
    SPBRG=207;
    
    //CONFIGURACION DE RECEPCION UART
    RCSTAbits.SPEN=1;           //
    RCSTAbits.RX9=0;
    RCSTAbits.CREN=1;
    TXSTAbits.TXEN = 1;         //se habilita la transmisión
    
    //CONFIGURACION DE INTERRUPCIONES
    //interrupcioness globales
    INTCONbits.GIE=1;           //se habilitan las interrupciones globales
    INTCONbits.PEIE=1 ;         //se prende interrupcion por perifericos
    //CONFIGURACION INTERRUPCION DE TRANSMISION
    PIE1bits.RCIE = 1 ;         //se habilita interrupcion por ADC
    PIR1bits.RCIF = 0;          // se baja bandera de transmision
    return;
}

