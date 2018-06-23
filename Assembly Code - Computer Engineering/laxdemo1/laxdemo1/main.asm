;
; laxdemo1.asm
;
; Created: 2017-12-14 15:07:20
; Author : Alexander
;


; Replace with your application code
;PORTA, out to display
;PORTB, In from keyboard, ???2 9753

.def r17 = NUM
.def r18 = LEFTNUM
.def r19 = RIGHTNUM

ldi r16, HIGH(RAMEND)
out SPH, r16
ldi r16, LOW(RAMEND)
out SPL, r16

call HW_INIT
FOREVER:
	sbic PINB, 4	; Strobe is 0, no button pressed
	call READ_NUM
	jmp FOREVER	
CONTINUE:
	; DCBA DCBA = 0123 4567 -> LEFTNUM RIGHTNUM
	; 0000 xxxx = xxxx 0000
	lsl RIGHTNUM
	lsl RIGHTNUM
	lsl RIGHTNUM
	lsl RIGHTNUM
	add LEFTNUM, RIGHTNUM
	out PORTB, LEFTNUM

	clr NUM
	clr LEFTNUM
	clr RIGHTNUM
	jmp FOREVER

READ_NUM:
	in NUM, PINB
	cp NUM, $0A
	brsh TOO_BIG ;>=10
	mov LEFTNUM, NUM
	jmp CONTINUE
TOO_BIG:
	ldi LEFTNUM, $01 ; left num = 1
	subi NUM, $0A	 ; NUM - 10 = rightnum
	mov NUM, RIGHTNUM
	jmp CONTINUE

HW_INIT:
	ldi r16, $FF
	out DDRA, r16	; out = 1111 1111

	ldi r16, $00
	out DRRB, r16	; in from keyboard
	;ldi r16, $1F	; 0001 1111, use only those
	;out PORTB, r16
	ret
