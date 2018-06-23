;
; laxdemo3.asm
;
; Created: 2017-12-14 16:22:33
; Author : Alexander
;

;0002 9753
.def NUM = r17
.def LEFTSIDE = r18

ldi r16, HIGH(RAMEND)
out SPH, r16
ldi r16, LOW(RAMEND)
out SPL, r16

call HW_INIT

ldi LEFTSIDE, $01
FOREVER:
	sbic PINB, 4	; Strobe is not activated
	jmp READNUM
	jmp FOREVER
CONTINUE:
	cpi NUM, $0F	; F was pressed
	breq SWITCH
BACK:
	cpi LEFTSIDE, $01
	breq LEFTSIDE_OUT
	lsl NUM
	lsl NUM
	lsl NUM
	lsl NUM
LEFTSIDE_OUT:
	out PORTA, NUM
	jmp FOREVER

SWITCH:
	cpi LEFTSIDE, $01
	breq TORIGHT
	ldi LEFTSIDE, $01
	jmp BACK
TORIGHT:
	ldi LEFTSIDE, $00
	jmp BACK

READNUM:
	in NUM, PINB
	jmp CONTINUE

HW_INIT:
	ldi r16, $FF	; Use all out to display
	out DDRA, r16
	ldi r16, $00	; Use all for reading in
	out DDRB, r16
	ret