;
; laxdemo2.asm
;
; Created: 2017-12-14 15:44:07
; Author : Alexander
;

; PORTA, out to left display
; PORTB, button all IN
.def NUM = r17

.org $00 jmp MAIN
.org INT0_addr jmp INCNUM
.org INT1_addr jmp DISPLAY

MAIN:
	ldi r16, HIGH(RAMEND)
	out SPH, r16
	ldi r16, LOW(RAMEND)
	out SPL, r16

	clr NUM
	call HW_INIT
LOOP: jmp LOOP

INCNUM:
	cpi NUM, $0F
	brsh TOO_HIGH
	inc NUM
TOO_HIGH:
	reti

DISPLAY:
	out PORTA, NUM
	reti

HW_INIT:
	ldi r16, $0F	; 0000 1111 display
	out DDRA, r16
	ldi r16, $00
	out DDRD, r16	; 0000 0000
	ldi r16, (1 << PD2 | 1 << PD3)	; INT0 and INT1
	out PORTD, r16
	ldi r16, (1 << INT01 | 1 << INT00 | 1 << INT11 | 1 << INT10)
	out MCUCR, r16
	ldi r16, (1 << INT0 | 1 << INT1)
	out GICR, r16
	ret
