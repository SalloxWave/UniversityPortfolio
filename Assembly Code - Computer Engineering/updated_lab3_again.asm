/*
 * AssemblerApplication1.asm
 *
 *  Created: 12/4/2017 2:54:22 PM
 *   Author: miksj784, alejo720
 */ 

.def TMP = r16
.def ZERO = r17
.def COUNTER = r18
.equ TIME = $60
.def MUX_INDEX = r19

.macro PUSH_ALL
	push TMP
	in TMP, SREG
	push TMP
	push YL
	push YH
.endmacro

.macro POP_ALL
	pop YH
	pop YL
	pop TMP
	out SREG, TMP
	pop TMP	
.endmacro

.org $00 jmp MAIN 
.org $02 jmp BCD	; INT0, 1HZ
.org $04 jmp MUX	; INT1, 1000HZ

MAIN:
	; Init stack
	ldi r16, HIGH(RAMEND)
	out SPH, r16
	ldi r16, LOW(RAMEND)
	out SPL, r16

	call INIT
	sts TIME, ZERO		; Reset counter
	std Y+1, ZERO
	std Y+2, ZERO
	std Y+3, ZERO
	ldi MUX_INDEX, $00	; Set index to 0
	sei					; Set enable interrupt

LOOP: jmp LOOP			; Let program run while listenting for interrupts	

INIT:
	clr ZERO
	clr COUNTER
	ldi TMP, $03
	out DDRA, TMP													; 0000 0011
	ldi TMP, $7F
	out DDRB, TMP													; 0111 1111
	out DDRD, ZERO													; In, all 0's
	ldi TMP, (1 << PD2 | 1 << PD3)									; 0000 1100, PD2->INT0, PD3->INT1
	out PORTD, TMP
	ldi TMP, (1 << ISC01 | 1 << ISC00 | 1 << ISC11 | 1 << ISC10)	; Up trigger (keyup)
	out MCUCR, TMP
	ldi TMP, (1 << INT0 | 1 << INT1)								; Enable INT0 and INT1
	out GICR, TMP													
	ldi YH, HIGH(TIME)												; Let X point to current time
	ldi YL, LOW(TIME)
	ldi ZH, HIGH(NUMTAB*2)											; Let Z point to table
	ldi ZL, LOW(NUMTAB*2)
	ret

BCD:
	PUSH_ALL

	mov COUNTER, ZERO
INCR:
	ld TMP, Y
	inc TMP				; TIME+=1
	cpi TMP, 10
	brne CONTINUE		; if TIME != 10
	inc COUNTER
	st Y+, ZERO			; Clear time
	cpi COUNTER, 2		; Index 2 means need to check for 60 instead of 10
	brne INCR_SIX
	cpi COUNTER, 4		; Index 4 means need to check for 60 instead of 10
	brne INCR_SIX	
	rjmp INCR			; Call recursively to check for next number
CONTINUE:
	st Y, TMP

	POP_ALL
	reti

INCR_SIX:
	ld TMP, Y		
	inc TMP				; TIME+=1
	cpi TMP, 6		
	brne CONTINUE		; Not a 6, continue
	st Y+, ZERO			; Is a 6, reset current number and point to next number
	cpi COUNTER, 4		; If we're on display position 4, reset the counter to 0
	breq RESET_COUNTER
	rjmp INCR			; Call recursively to check for next number

RESET_COUNTER:
	clr COUNTER
	rjmp INCR

MUX:
	PUSH_ALL
	push ZL
	push ZH

	out PORTB, ZERO			; Make sure nothing is displayed on old
	cpi MUX_INDEX, 4
	brne MUX_CONTINUE		; if index == 4
	clr MUX_INDEX			; index = 0
MUX_CONTINUE:
	out PORTA, MUX_INDEX	; A,B = MUX_INDEX (what number index to display)
	add YL, MUX_INDEX
	brcc NO_CARRY
	inc YH
NO_CARRY:
	ld TMP, Y
	add ZL, TMP				; Use current number to step in table
	brcc JUMP				; Increment Z-HIGH if carry
	inc ZH
JUMP:
	lpm TMP, Z
	out PORTB, TMP
	inc MUX_INDEX

	pop ZH
	pop ZL
	POP_ALL
	reti

NUMTAB: .db $3F, $06, $5B, $4F, $66, $6D, $7D, $07, $7F, $6F
	