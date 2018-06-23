;
; lab1.asm
;
; Created: 2017-11-14 14:12:52
; Author : AlexanderJ
;


; Replace with your application code
/*start:
    inc r16
    rjmp start*/


.def num = r20
.def digit = r21
.equ DEL = 4
.equ DEL_2 = DEL/2
.equ INNER_LOOP_COUNT = $FF
.org 0x00
ldi r16, HIGH(RAMEND)
out SPH, r16
ldi r16, LOW(RAMEND)
out SPL, r16
ldi r16, $8F ; set bits 0-3 and 7 to write in port b
out DDRB, r16 ; sends it to port b
FOREVER:
	clr digit
	clr num
	sbic PINA, 0 ; Skip if bit is cleared (0)
	rcall READ_NUM
    breq FOREVER ; Start bit not found, try again
    out PORTB, num ; Send read number to port B
    ldi r16, DEL_2 ; Half delay to reset
	rcall HALFD
    rjmp FOREVER

READ_NUM:
	rcall DELAY ; Wait whole start bit
	ldi r16, DEL_2 ; Wait half delay to read middle of next bit
	rcall HALFD
	in digit, PINA ; Read lsb (bit 0) to digit
	add num, digit ; Add to num
	rcall DELAY
	in digit, PINA ; Read bit 1 and add to num
	lsl digit ; Left shift to get correct position
	add num, digit
	rcall DELAY
	in digit, PINA ; Bit 2
	lsl digit
	lsl digit
	add num, digit
	rcall DELAY
	in digit, PINA ; Bit 3
	lsl digit
	lsl digit
	lsl digit
	add num, digit
	clz
	ret

DELAY:
	ldi r16, DEL ; r16 = delay
HALFD:
	sbi PORTB, 7 ; Set bit 7 (to oscilloscope)
DELAY_OUTER_LOOP:
	ldi r17, INNER_LOOP_COUNT ;r17 = INNER_LOOP_COUNT
DELAY_INNER_LOOP:
	dec r17
	brne DELAY_INNER_LOOP
	dec r16
	brne DELAY_OUTER_LOOP
	cbi PORTB, 7 ; Clear bit 7 (to oscilloscope)
	ret

