    .include "p30F3013.inc"

    .GLOBAL __U1RXInterrupt
    .GLOBAL _dato
    .GLOBAL _drcv
;/**@brief ESTA ES LA ISR DEL UART
; *
; */	
   
__U1RXInterrupt:
	PUSH	W0
	MOV	U1RXREG,    W0		;W0 = U1RXREG
	MOV	W0,	    _dato
	MOV	#1,	    W0
	MOV	W0,	    _drcv	;Se activa la bandera de dato recibido
	
FIN_ISR_UART1:
	BCLR	IFS0,	    #U1RXIF	;APAGAR BANDERA DE INTERRUPCION DEL UART
    
	POP	W0
	RETFIE
