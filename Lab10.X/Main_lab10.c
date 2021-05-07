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
unsigned char valores_ascii1[] = 
{
       83,69,76,69,67,67,73,79,78,65,32,85,78,65,32,79,80,67,73,79,78
    // S   E  L  E  C  C  I  O  N  E _   U  N  A  _  O  P  C  I  O  N
};
unsigned char valores_ascii2[] = 
{
    49, 41, 32, 80, 79,82,84,32,66
};
  
char valores;
//int contador1 = 0; // varible para incrementar
//int contador2 = 0; // varible para incrementar

/*-----------------------------------------------------------------------------
-------------------------prototipos de funciones-------------------------------
-----------------------------------------------------------------------------*/
void transmision(char datos);   //para funcion de transmision
char recepcion();               // para funcion de recepcion
void cadena_datos(char *str);   // para funcion de enviar caracteres
void setup(void);               //funcion para configuracion de registros del PIC

/*-----------------------------------------------------------------------------
----------------------------- interrupciones ----------------------------------
-----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
    /*//interrupcion de recibir datos
    if(PIR1bits.RCIF)
    {
        PORTB= RCREG;    //PORTB toma el valor de recepcion
        __delay_us(100);
        PORTA= RCREG;
        
    }
    */
}
/*-----------------------------------------------------------------------------
----------------------------- ciclo principal----------------------------------
-----------------------------------------------------------------------------*/

//funcion para transmision
void transmision(char datos)
    {
    while(TXSTAbits.TRMT==0);
    TXREG=datos;
    }

//funcion para recepcion
char recepcion()
    {
    return RCREG;
    }

//funcion para enviar caracteres 
void cadena_datos(char *str)
    {
    while(*str != '\0')
        {
        transmision(*str);
        str++;
        }
    }

//funcion principal
void main(void) 
{  
    setup();                    //Configuraciones generales del PIC
    //printf("\r wenas, solo estoy probando  \r");
    //---------------------loop principal del programa ------------------------
    while(1)            //se hace loop infinito mientras sea  
    {
       cadena_datos("\r  Que quiere hacer  \r");
       cadena_datos(" 1)Poner caracteres \r");
       cadena_datos(" 2) POner valores en POrtA \r");
       cadena_datos(" 3) POner valores en POrtC \r");
       
       while(PIR1bits.RCIF==0);
       valores = recepcion;
       
       switch(valores)
       {
           case ('1'):
               cadena_datos(" Wenas \r");
               break;
           
           case ('2'):
               cadena_datos("Poner los valores en PortA: ");
               while(PIR1bits.RCIF==0);
               PORTA = recepcion;
               cadena_datos("\r Listo compadre \r");
               break;
           
           case ('3'):
               cadena_datos("Poner los valores en PortC: ");
               while(PIR1bits.RCIF==0);
               PORTD = recepcion;
               cadena_datos("\r Listo compadre \r");
               break;   
       }
    }    
        
        /*__delay_ms(200);
        if (PIR1bits.TXIF)
        {
            TXREG = valores_ascii1[contador1] ;
          
            //TXREG = valores_ascii3[contador] ;
            //TXREG = 'A';
            
            contador1++;
            if (contador1==21) 
            {
                TXREG = valores_ascii2[contador2] ;
                contador2++;
            }
            if (contador2==9)
            {
                contador2=9;
            }
        }*/
        
                      
}
/*-----------------------------------------------------------------------------
--------------------------- configuraciones ----------------------------------
-----------------------------------------------------------------------------*/
void setup(void) //FUNCION PARA CONFIGURACION DE ENTRADAS Y SALIDAS
{
    //CONFIGURACION DE ENTRADAS/SALIDAS DIGITALES
    ANSEL = 0;                  //no hay entradas analógicas
    ANSELH = 0;                 //no hay entradas analógicas
    
    //CONFIGURACION DE ENTRADAS/SALIDAS ANLAGÓGICAS
    TRISA = 0;                  //se define PortA como salida
    TRISD = 0;                  // se define PortC como salida
    
    PORTA=0;                    // se limpia PortA
    PORTD=0;                    // se limpia PortC 
    
    //CONFIGURACION DEL OSCILADOR
    OSCCONbits.IRCF=0b111;         //8MHz 111
    OSCCONbits.SCS=1;           //configuracion de oscilador interno
            
    //CONFIGURACION DE TRANSMISION UART
    TXSTAbits.TXEN = 1;         //se habilita la transmisión
    TXSTAbits.SYNC = 0;         //se habilita modo asíncrono
    TXSTAbits.BRGH = 1;         //transmisión rápida
    
    BAUDCTLbits.BRG16 = 1;      //generador de 16bits de Baud rate
    SPBRGH=0x00;
    SPBRG=207;
    
    //CONFIGURACION DE RECEPCION UART
    RCSTAbits.SPEN=1;           //se enciende el modulo
    RCSTAbits.RX9=0;            //tranmision de 8bits
    RCSTAbits.CREN=1;           //se activa la recepcion
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

