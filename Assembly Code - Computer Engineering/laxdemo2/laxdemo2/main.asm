;
; laxdemo2.asm
;
; Created: 2017-12-14 15:44:07
; Author : Alexander
;


; PORTA, out to left display
; PORTB, button all IN
.def NUM = r17

ldi r16, HIGH(RAMEND)
out SPH, r16
ldi r16, LOW(RAMEND)
out SPL, r16

call HW_INIT

clr NUM
;0000 00xx = 0000 00 leftbtn rightbtn
; Listen for left button
FOREVER:
	in r16, PINB
	cpi r16, $01	; Left button is pressed
	breq INCNUM
CONTINUE:
	cpi r16, $02	; Right button is pressed
	breq DISPLAY
	jmp FOREVER

INCNUM:
	cpi NUM, $0F
	brsh CONTINUE
	inc NUM
	jmp CONTINUE

DISPLAY:
	out PORTA, NUM
	jmp FOREVER

HW_INIT:
	ldi r16, $0F	; 0000 1111
	out PORTA, r16
	ldi r16, $00
	out PORTB, r16	; 0000 0000
	ret
