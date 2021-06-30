    .include "p30F3013.inc"
    .EQU    RS_LCD,	RF4
    .EQU    RW_LCD,	RF5
    .EQU    E_LCD,	RD8

    .GLOBAL _COMANDO_LCD
    .GLOBAL _DATO_LCD
    .GLOBAL _BF_LCD
    .GLOBAL _INI_LCD_8BITS
    .GLOBAL _printLCD
    
; @BRIEF: ESTA RUTINA IMPRIME ARREGLO DE CARACTERES EN EL LCD
; @PARAM: W0, CADENA A IMPRIMIR
; @RETURN: NINGUNO
;    
_printLCD:    
    PUSH    W0
    PUSH    W1
    MOV	    W0,	    W1
IMPRIMIR:
    MOV.B   [W1++], W0	;W1 = *CAD
    CP0.B   W0
    BRA	    Z,	    FIN_printLCD

    
    CALL    _BF_LCD
    CALL    _DATO_LCD
    ;CALL A DATO_LCD
    ;CONTINUARA...
    ;COLOCAR CODIGO DE ENVIO AL LCD
    ;NO OLVIDAR PUSH Y POP DE REGISTROS
    
    GOTO    IMPRIMIR
    
FIN_printLCD:
    POP    W0
    POP    W1
    RETURN
    
    
; @BRIEF: ESTA RUTINA MANDA COMANDOS AL LCD
; @PARAM: W0, COMANDO A ENVIAR
;    
_COMANDO_LCD:
    BCLR    PORTF,	#RS_LCD	    ;RS = 0
    NOP
    BCLR    PORTF,	#RW_LCD	    ;RW = 0
    NOP
    BSET    PORTD,	#E_LCD	    ;E = 1
    NOP
    MOV.B   WREG,	PORTB	    ;PORTB(7:0) = W0(7:0)
    NOP
    BCLR    PORTD,	#E_LCD	    ;E = 0
    NOP
    
    RETURN
; @BRIEF: ESTA RUTINA MANDA DATOS AL LCD
; @PARAM: W0, DATO A ENVIAR
;    
_DATO_LCD:
    BSET    PORTF,	#RS_LCD	    ;RS = 1
    NOP
    BCLR    PORTF,	#RW_LCD	    ;RW = 0
    NOP
    BSET    PORTD,	#E_LCD	    ;E = 1
    NOP
    MOV.B   WREG,	PORTB	    ;PORTB(7:0) = W0(7:0)
    NOP
    BCLR    PORTD,	#E_LCD	    ;E = 0
    NOP
    
    RETURN
; @BRIEF: ESTA RUTINA VERIFICA LA BANDERA BUSY FLAG DEL LCD
;    
_BF_LCD:
    
    PUSH    TRISB		    ;STACK(SP++) = TRISB
    PUSH    W0
    NOP
    
    MOV	    #0x00FF,	W0	    ;W0 = 0xFF		
    IOR	    TRISB	    	    ;TRISB(7:0) = 0xFF
    NOP
    
    BCLR    PORTF,	#RS_LCD	    ;RD = 0
    NOP
    
    BSET    PORTF,	#RW_LCD	    ;RW = 1
    NOP
    
    BSET    PORTD,	#E_LCD	    ;E = 1
    NOP
ESPERA_LCD:
    BTSC    PORTB,	#RB7	    ;POLLING
    GOTO    ESPERA_LCD
    
    BCLR    PORTD,      #E_LCD
    NOP
    
    BCLR    PORTF,      #RW_LCD
    NOP
    
    POP	    W0
    POP	    TRISB
    
    RETURN
; @BRIEF: ESTA RUTINA INICIALIZA EL LCD EN MODO DE 8 BITS
;    
_INI_LCD_8BITS:
    
    DO #2, INI_CICLO    
	    CALL    _RETARDO_100ms
	    MOV	    #0X30,	W0
	    CALL    _COMANDO_LCD
    INI_CICLO:	NOP

;    CALL    _RETARDO_100ms
;    MOV	    #0X30,	W0
;    CALL    _COMANDO_LCD

;    CALL    _RETARDO_100ms
;    MOV	    #0X30,	W0
;    CALL    _COMANDO_LCD

    
    CALL    _BF_LCD
    MOV	    #0X38,	W0
    CALL    _COMANDO_LCD

    CALL    _BF_LCD
    MOV	    #0X08,	W0
    CALL    _COMANDO_LCD

    CALL    _BF_LCD
    MOV	    #0X01,	W0
    CALL    _COMANDO_LCD
    
    CALL    _BF_LCD
    MOV	    #0X06,	W0
    CALL    _COMANDO_LCD
    
    CALL    _BF_LCD
    MOV	    #0X0C,	W0
    CALL    _COMANDO_LCD
    
    RETURN
    






