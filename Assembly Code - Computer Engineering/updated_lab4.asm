
; --- lab4spel.asm

.equ	VMEM_SZ     = 5		; #rows on display
.equ	AD_CHAN_X   = 3		; ADC0=PA0, PORTA bit 0 X-led
.equ	AD_CHAN_Y   = 4		; ADC1=PA1, PORTA bit 1 Y-led
.equ	GAME_SPEED  = 70	; inter-run delay (millisecs)
.equ	PRESCALE    = 7		; AD-prescaler value
.equ	BEEP_PITCH  = 20	; Victory beep pitch
.equ	BEEP_LENGTH = 100	; Victory beep length
.def	MUX_INDEX	= r19
	
; ---------------------------------------
; --- Memory layout in SRAM
.dseg
.org	SRAM_START
POSX:	.byte	1		; Own position
POSY:	.byte 	1
TPOSX:	.byte	1		; Target position
TPOSY:	.byte	1
LINE:	.byte	1		; Current line	
VMEM:	.byte	VMEM_SZ ; Video MEMory
SEED:	.byte	1		; Seed for Random

; ---------------------------------------
; --- Macros for inc/dec-rementing
; --- a byte in SRAM
.macro INCSRAM	; inc byte in SRAM
	lds	r16,@0
	inc	r16
	sts	@0,r16
.endmacro

.macro DECSRAM	; dec byte in SRAM
	lds	r16,@0
	dec	r16
	sts	@0,r16
.endmacro

; ---------------------------------------
; --- Code
.cseg
.org 	$0
jmp	START
.org	INT0addr
jmp	MUX

START:
	; Init stack pointer
	ldi		r16, HIGH(RAMEND)
	out		SPH, r16
	ldi		r16, LOW(RAMEND)
	out		SPL, r16
	call	HW_INIT
	call	WARM
RUN:
	call	JOYSTICK
	call	ERASE_VMEM
	call	UPDATE

/**** 	V?nta en stund s? inte spelet g?r f?r fort 	***
	
*** 	Avg?r om tr?ff				 	****/
	call	DELAY

	lds		r16, POSX
	lds		r17, TPOSX
	cp		r16, r17
	breq	Y_COMP
BACK:
	brne	NO_HIT	
	ldi		r16, BEEP_LENGTH
	call	BEEP
	call	WARM
NO_HIT:
	jmp		RUN

Y_COMP:
	lds		r16, POSY
	lds		r17, TPOSY
	cp		r16, r17
	jmp		BACK

	; ---------------------------------------
	; --- Multiplex display
MUX:	

/**** 	skriv rutin som handhar multiplexningen och ***
*** 	utskriften till diodmatrisen. ?ka SEED.		****/
	push	r16
	in		r16, SREG
	push	r16
	push	YL
	push	YH

	clr		r16
	out		PORTB, r16
	cpi		MUX_INDEX, 5
	brne	MUX_CONTINUE		; if index == 5
	clr		MUX_INDEX			; index = 0
MUX_CONTINUE:
	clr		r16
	;clr	r17
	;mov	r17, MUX_INDEX
	;lsl	r17
	;lsl	r17
	;out	PORTA, r17	; A,B = MUX_INDEX (what number index to display)
	out		PORTA, MUX_INDEX
	add		YL, MUX_INDEX
	brcc	NO_CARRY
	inc		YH
NO_CARRY:
	ld		r16, Y
	out		PORTB, r16
	inc		MUX_INDEX
	INCSRAM	SEED

	pop		YH
	pop		YL
	pop		r16
	out		SREG, r16
	pop		r16
	reti
		
	; ---------------------------------------
	; --- JOYSTICK Sense stick and update POSX, POSY
	; --- Uses r16
JOYSTICK:	

/**** 	skriv kod som ?kar eller minskar POSX beroende 	***
*** 	p? insignalen fr?n A/D-omvandlaren i X-led...	***

*** 	...och samma f?r Y-led 				****/
	ldi		r16, AD_CHAN_X
	out		ADMUX, r16
	ldi		r16, (1 << ADEN)
	out		ADCSRA, r16
	sbi		ADCSRA, ADSC
WAIT_X:
	sbic	ADCSRA, ADSC
	jmp		WAIT_X
	in		r16, ADCH
	cpi		r16, 3
	breq	INC_X
	cpi		r16, 0
	breq	DEC_X
DONE_X:

	ldi		r16, AD_CHAN_Y
	out		ADMUX, r16
	ldi		r16, (1 << ADEN)
	out		ADCSRA, r16
	sbi		ADCSRA, ADSC
WAIT_Y:
	sbic	ADCSRA, ADSC
	jmp		WAIT_Y
	in		r16, ADCH
	cpi		r16, 3
	breq	INC_Y
	cpi		r16, 0
	breq	DEC_Y

JOY_LIM:
	call	LIMITS		; don't fall off world!
	ret

INC_X:
	INCSRAM POSX
	jmp		DONE_X

DEC_X:
	DECSRAM POSX
	jmp		DONE_X

INC_Y:
	INCSRAM POSY
	jmp		JOY_LIM

DEC_Y:
	DECSRAM POSY
	jmp		JOY_LIM


; ---------------------------------------
; --- LIMITS Limit POSX,POSY coordinates	
; --- Uses r16, r17
LIMITS:
	lds		r16,POSX	; variable
	ldi		r17,7		; upper limit+1
	call	POS_LIM		; actual work
	sts		POSX,r16
	lds		r16,POSY	; variable
	ldi		r17,5		; upper limit+1
	call	POS_LIM		; actual work
	sts		POSY,r16
	ret

POS_LIM:
	ori		r16,0		; negative?
	brmi	POS_LESS	; POSX neg => add 1
	cp		r16,r17		; past edge
	brne	POS_OK
	subi	r16,2
POS_LESS:
	inc		r16	
POS_OK:
	ret

; ---------------------------------------
; --- UPDATE VMEM
; --- with POSX/Y, TPOSX/Y
; --- Uses r16, r17
UPDATE:	
	clr		ZH 
	ldi		ZL,LOW(POSX)
	call 	SETPOS
	clr		ZH
	ldi		ZL,LOW(TPOSX)
	call	SETPOS
	ret

; --- SETPOS Set bit pattern of r16 into *Z
; --- Uses r16, r17
; --- 1st call Z points to POSX at entry and POSY at exit
; --- 2nd call Z points to TPOSX at entry and TPOSY at exit
SETPOS:
	ld		r17,Z+  	; r17=POSX
	call	SETBIT		; r16=bitpattern for VMEM+POSY
	ld		r17,Z		; r17=POSY Z to POSY
	ldi		ZL,LOW(VMEM)
	add		ZL,r17		; *(VMEM+T/POSY) ZL=VMEM+0..4
	ld		r17,Z		; current line in VMEM
	or		r17,r16		; OR on place
	st		Z,r17		; put back into VMEM
	ret
	
; --- SETBIT Set bit r17 on r16
; --- Uses r16, r17
SETBIT:
	ldi		r16,$01		; bit to shift
SETBIT_LOOP:
	dec 	r17			
	brmi 	SETBIT_END	; til done
	lsl 	r16			; shift
	jmp 	SETBIT_LOOP
SETBIT_END:
	ret

; ---------------------------------------
; --- Hardware init
; --- Uses r16
HW_INIT:

/**** 	Konfigurera h?rdvara och MUX-avbrott enligt ***
*** 	ditt elektriska schema. Konfigurera 		***
*** 	flanktriggat avbrott p? INT0 (PD2).			****/
	ldi		r16, $87
	out		DDRA, r16						; Rows and speaker
	ldi		r16, $7F
	out		DDRB, r16						; Columns
	ldi		r16, (1 << PD2)					; 0000 1100, PD2->INT0
	out		PORTD, r16
	ldi		r16, (1 << ISC01 | 1 << ISC00)	; Up trigger (keyup)
	out		MCUCR, r16
	ldi		r16, (1 << INT0)				; Use INT0
	out		GICR, r16
	ldi		YH, HIGH(VMEM)					; Y points to VIDEO MEMORY
	ldi		YL, LOW(VMEM)
	sei		; display on
	ret

; ---------------------------------------
; --- WARM start. Set up a new game
WARM:

/**** 	S?tt startposition (POSX,POSY)=(0,2)		****/
	ldi		r16, 0
	sts		POSX, r16
	ldi		r16, 2
	sts		POSY, r16
	push	r0		
	push	r0		
	call	RANDOM		; RANDOM returns x,y on stack
	pop		r16
	sts		TPOSX, r16
	pop		r16
	sts		TPOSY, r16
	;ldi		r16, 4
	;sts		TPOSX, r16
	;sts		TPOSY, r16

/**** 	S?tt startposition (TPOSX,POSY)				****/

	call	ERASE_VMEM
	ret

	; ---------------------------------------
	; --- RANDOM generate TPOSX, TPOSY
	; --- in variables passed on stack.
	; --- Usage as:
	; ---	push r0 
	; ---	push r0 
	; ---	call RANDOM
	; ---	pop TPOSX 
	; ---	pop TPOSY
	; --- Uses r16
RANDOM:
	in		r16,SPH
	mov		ZH,r16
	in		r16,SPL
	mov		ZL,r16
	lds		r16,SEED
	
/**** 	Anv?nd SEED f?r att ber?kna TPOSX		***
*** 	Anv?nd SEED f?r att ber?kna TPOSX		****/
	andi	r16, $07	; 7 = 0000 0111 which gives the last 3 bits
	cpi		r16, 5
	brlo	VALID_NO	; pos y less than 5
	subi	r16, 4		; higher than 5, subtract 4
VALID_NO:
	;ldd	r16, Z+4
	std		Z+4, r16
	;***		; store TPOSX	2..6
	;***		; store TPOSY   0..4

	lds		r16, SEED
	andi	r16, $07
	cpi		r16, 7
	brsh	HIGH_NO
SECOND_COMP:
	cpi		r16, 2
	brlo	LOW_NO
COMP_DONE:
	;ldd	r16, Z+3
	std		Z+3, r16

	ret

HIGH_NO:
	subi	r16, 6
	jmp		SECOND_COMP

LOW_NO:
	inc		r16
	inc		r16
	jmp		COMP_DONE
	; ---------------------------------------
	; --- Erase Videomemory bytes
	; --- Clears VMEM..VMEM+4
	
ERASE_VMEM:
	;push YL
	;push YH
/**** 	Radera videominnet						****/
	;ldi		r16, VMEM_SZ
	ldi		r20, $00
ERASE_JMP:
	st		Y, r20
	;inc		YL
	std		Y+1, r20
	std		Y+2, r20
	std		Y+3, r20
	std		Y+4, r20
	;brcc	HOPP
	;inc		YH
HOPP:
	;dec		r16
	;brne	ERASE_JMP
	;pop YH
	;pop YL
	ret

	; ---------------------------------------
	; --- BEEP(r16) r16 half cycles of BEEP-PITCH
BEEP:	
/**** skriv kod f?r ett ljud som ska markera tr?ff 	****/
	; out till C7	
	ldi r25, BEEP_LENGTH
BLOOP:
	sbi PORTA, 7	; Send out on B7
	rcall SOUND_DELAY		; Frequency delay
	cbi PORTA, 7	; Stop sending out on B7
	rcall SOUND_DELAY
	dec r25
	brne BLOOP
	ret

SOUND_DELAY:
	ldi r22, BEEP_PITCH
SOUND_DELAY_OUTER_LOOP:
;	ldi r21, 10
;SOUND_DELAY_INNER_LOOP:
;	dec r21
;	brne SOUND_DELAY_INNER_LOOP
	dec r22
	brne SOUND_DELAY_OUTER_LOOP
	ret


DELAY:
	ldi r22, $E0
DELAY_OUTER_LOOP:
	ldi r21, $FF
DELAY_INNER_LOOP:
	dec r21
	brne DELAY_INNER_LOOP
	dec r22
	brne DELAY_OUTER_LOOP
	ret

			