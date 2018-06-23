;
; laxdemo5.asm
;
; Created: 2017-12-14 16:57:35
; Author : Alexander
;


; PORTA, out to display
; PORTB, in from keyboard
.def NUM = r17
.def OLDNUM = r20
.def INVERT = r18

ldi r16, HIGH(RAMEND)
out SPH, r16
ldi r16, LOW(RAMEND)
out SPL, r16

call HW_INIT
ldi INVERT, $00
FOREVER:
	sbic PINB, 4	; button is not pressed
	jmp READNUM
	jmp FOREVER
CONTINUE:
	mov r19, OLDNUM	; TMP
	cpi NUM, $0F
	brsh NOSHIFT	; Already left shifted
	lsl NUM
	lsl NUM
	lsl NUM
	lsl NUM
NOSHIFT:
	cpi INVERT, $00
	breq NO_INVERT
	com r19
NO_INVERT:
	andi r19, $0F
	andi NUM, $F0
	add NUM, r19 ;r19 = right side, NUM = left side
	out PORTA, NUM
	jmp FOREVER

READNUM:
	in r16, PINB
	andi r16, $0F
	cpi r16, $00	
	breq CHANGE_INVERT
	mov NUM, r16	; Only read number again if not 0	
	mov OLDNUM, NUM
	jmp CONTINUE

CHANGE_INVERT:
	cpi INVERT, $00
	breq TO_INVERT
	ldi INVERT, $00
	jmp CONTINUE
TO_INVERT:
	ldi INVERT, $01
	jmp CONTINUE

HW_INIT:
	; 0002 9753
	ldi r16, $FF
	out DDRA, r16
	ldi r16, $00
	out DDRB, r16
	ret
