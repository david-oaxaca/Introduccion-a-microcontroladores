#include "p30F3013.h"


//_FOSC(CSW_FSCM_OFF & FRC); 
#pragma config FOSFPR = FRC             // Oscillator (Internal Fast RC (No change to Primary Osc Mode bits))
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)
/********************************************************************************/
/* SE DESACTIVA EL WAvoid habilitarPerifericos()TCHDOG														*/
//_FWDT(WDT_OFF); 
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)
/********************************************************************************/
//_FBORPOR( PBOR_ON & BORV27 & PWRT_16 & MCLR_EN ); 
// FBORPOR
#pragma config FPWRT  = PWRT_16          // POR Timer Value (16ms)
#pragma config BODENV = BORV20           // Brown Out Voltage (2.7V)
#pragma config BOREN  = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE  = MCLR_EN          // Master Clear Enable (Enabled)
/********************************************************************************/
//_FGS(CODE_PROT_OFF)
// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

/********************************************************************************/
/* SECCI?N DE DECLARACI?N DE CONSTANTES CON DEFINE								*/
/********************************************************************************/
#define EVER 1
//#define MUESTRAS 64

/********************************************************************************/
/* DECLARACIONES GLOBALES														*/
/********************************************************************************/
/*DECLARACI?N DE LA ISR DEL TIMER 1 USANDO __attribute__						*/
/********************************************************************************/
void __attribute__((__interrupt__)) _INT0Interrupt( void );

void configurarPuertos();  
void configurarTimer();
void configurarInt();
void iniReloj();

extern void INI_LCD_8BITS( );
extern void BF_LCD( );
extern void DATO_LCD( unsigned short int Dato );
extern void COMANDO_LCD(unsigned short int Dato);
extern void habilitarLPOSC();
extern void printLCD(char *cad);

short int useg, dseg, umin, dmin, uhr, dhr;
char RTC [9];

int main(void) 
{    
    configurarPuertos();
    
    iniReloj();
    configurarTimer();
    habilitarLPOSC();
    INI_LCD_8BITS();
    BF_LCD();
    COMANDO_LCD(0X85);
    printLCD("RELOJ:");
    configurarInt();
    
    //Se prende el timer
    
    T1CONbits.TON = 1;      //Se activa el timer
    
   
    for(;EVER;)
    {
        BF_LCD();
        COMANDO_LCD(0xC4);
        
        RTC[0] = dhr + 0X30;
        RTC[1] = uhr + 0X30;
        RTC[2] = ':';
        RTC[3] = dmin + 0X30;
        RTC[4] = umin + 0X30;
        RTC[5] = ':';
        RTC[6] = dseg + 0X30;
        RTC[7] = useg + 0X30;
        RTC[8] = 0;
        
        printLCD(RTC);
         
        asm("nop");
    }
    
    return 0;
}

void iniReloj(){
    dhr = 2;
    uhr = 3;
    
    dmin = 5;
    umin = 9;
    
    dseg = 5;
    useg = 0;
    
}

void configurarInt(){

    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
}

void configurarTimer(){
    
    TMR1 = 0;
    PR1 = 0X8000;
    T1CON = 0X002;


    
}

void configurarPuertos()
{
    PORTC = 0;
    asm("nop");
    LATC = 0;
    asm("nop");
    TRISCbits.TRISC13 = 1;
    asm("nop");
    TRISCbits.TRISC14 = 1;
    asm("nop");
    
    PORTD = 0;              //CLR       PORTD
    asm("nop");             //ENSAMBLADOR EN LINEA
    LATD = 0;               //CLR       LATD
    asm("nop");
    TRISDbits.TRISD8 = 0;   //BCLR TRISD, #TRISD8
    asm("nop");

    PORTF = 0;
    asm("nop");
    LATF = 0;
    asm("nop");
    TRISFbits.TRISF4 = 0;
    asm("nop");
    TRISFbits.TRISF5 = 0;
    asm("nop");
    
    PORTB = 0;
    asm("nop");
    LATB = 0;
    asm("nop");
    TRISB = 0;
    asm("nop");
    ADPCFG = 0XFFFF;
    
}
/**  @brief: CONFIGURACION DEL UART 1. EL UART 1 SE COMUNICA CON 
 *           EL FT232 PARA COMUNICACION CON PC
 *   @baudrate: 115200 BAUDIOS
 *   @frame: 8 BITS X DATO, SIN PARIDAD, 1 BIT DE PARO
 */

/********************************************************************************/
/* DESCRICION:	ISR (INTERRUPT SERVICE ROUTINE) DE RECEPCION DEL UART 2			*/
/* LA RUTINA TIENE QUE SER GLOBAL PARA SER UNA ISR								*/	
/* SE USA PUSH.S PARA GUARDAR LOS REGISTROS W0, W1, W2, W3, C, Z, N Y DC EN LOS */
/* REGISTROS SOMBRA																*/
/********************************************************************************/
void __attribute__((__interrupt__, no_auto_psv)) _INT0Interrupt( void )
{
  
    IFS1bits.U2RXIF = 0;
}

/*
 void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt( void )
{
  
 *  asm("BTG LATD,  #LATD9");
 *   
    IFS0bits.T1IF = 0;
}
 
 */
