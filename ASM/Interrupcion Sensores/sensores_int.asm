; SENSORES ANALOGOS MEDIANTE INTERRUPCIÓN
; Luigi Lopez Buitrago - 201813525
; Esteban Martinez Camargo - 201820396

		LIST		P=16F877A
		INCLUDE		<P16F877A.inc>
		__CONFIG	_HS_OSC & _WDT_OFF & _PWRTE_ON & _LVP_OFF & _CP_OFF

		CBLOCK	0x20
		bin_adc
		suma_dec
		binario		
		ENDC

		ORG 	0
		GOTO 	INICIO
		ORG		4
		GOTO	INTERRUPCION

; CONFIGURACION
INICIO
		; Interrupciones: INT
		BANKSEL	INTCON			
		MOVLW	b'10010000'
		MOVWF	INTCON
		BANKSEL	OPTION_REG
		BSF		OPTION_REG,6
		; ENTRADAS:
		; Temperatura (RA0)
		; Calidad Aire (RA1)
		; Pulsador (RB0)
		; SALIDAS:
		; LED Temperatura (RB1)
		; LED Calidad Aire (RB2)
		; Display Decenas (RC6:RC0)
		; Display Unidades (RD6:RD0)
		BANKSEL	TRISA
		MOVLW	B'00001011'
		MOVWF	TRISA
		MOVLW	0x01
		MOVWF	TRISB
		CLRF    TRISC
		CLRF    TRISD
		; AN0, AN1 y AN3(a tierra) : analogos
		MOVLW	B'10000100'
		MOVWF	ADCON1
		; F_osc/8, Channel 0, A/D on bit:on
		BANKSEL	ADCON0
		MOVLW	B'01000001'	
		MOVWF	ADCON0
		CLRF	contador
		
; ¿CONTADOR ES IMPAR?
SEL_CANAL
	    BTFSS	contador,0	
	    GOTO	CANAL0

; LECTURA CANAL ANALOGO 1
CANAL1
	    BCF		PORTB,1
	    BSF		PORTB,2
		BSF		ADCON0,CHS0
		GOTO	ADC_CONVERT

; LECTURA CANAL ANALOGO 0
CANAL0
	    BSF		PORTB,1
	    BCF		PORTB,2
		BCF		ADCON0,CHS0
		GOTO	ADC_CONVERT

; CONVERSION ANALOGO-DIGITAL
ADC_CONVERT
		BSF		ADCON0,GO

ADC_LOOP
		BTFSC	ADCON0,GO
		GOTO	ADC_LOOP
		BANKSEL	ADRESL
		MOVF	ADRESL,W
		BANKSEL	ADCON0
		MOVWF	bin_adc
		CALL	Retardo_500ms
		GOTO	ADECUACION_TEMPERATURA

ADECUACION_TEMPERATURA
		BTFSC	PORTA,2
		GOTO	INICIO_CODIFICACION
		RRF		bin_adc,F
		MOVLW	0x7F
		ANDWF	bin_adc,F
		
; CODIFICAR BINARIO BCD A
; LA SALIDA EN DISPLAYS 7 SEGMENTOS
INICIO_CODIFICACION
        CLRF    suma_dec
        MOVF    bin_adc,W
        MOVWF   binario
        MOVLW   0xFF
        ANDWF   binario,F
        MOVLW   0x0A
        SUBWF   binario,W
        BTFSS   STATUS,C
        GOTO    UNIDADES
        GOTO    DECENAS

; VISUALIZAR SALIDAS EN DISPLAY 7 SEGMENTOS    
UNIDADES
		MOVLW	0xC0
		MOVWF	PORTC
        MOVF    binario,W
        CALL    DISPLAY

        MOVWF   PORTD
		GOTO	SEL_CANAL
		
DECENAS
        MOVLW   0x01
        ADDWF   suma_dec, W
        MOVWF   suma_dec
        MOVLW   0x0A
        SUBWF   binario, W
        MOVWF   binario
        BTFSS   STATUS,C
        GOTO    MOSTRAR_DEC
        GOTO    DECENAS

; VISUALIZAR SALIDAS EN DISPLAY 7 SEGMENTOS
MOSTRAR_DEC
        MOVLW   0x0A
        ADDWF   binario, W
        MOVWF   binario
        CALL    DISPLAY
        MOVWF   PORTD

        MOVLW   0x01
        SUBWF   suma_dec, W
        MOVWF   suma_dec
        CALL    DISPLAY
        MOVWF   PORTC
        GOTO    SEL_CANAL

DISPLAY
        ADDWF	PCL,F	;Para anodo comun
        RETLW	0xC0	;0
        RETLW	0xF9	;1
        RETLW	0xA4	;2
        RETLW	0xB0	;3
        RETLW	0x99	;4
        RETLW	0x92	;5
        RETLW	0x82	;6
        RETLW	0xF8	;7
        RETLW	0x80	;8
        RETLW	0x90	;9
   
   		INCLUDE	<INTERRUPCION.inc>
		INCLUDE	<RETARDO.inc>
		END