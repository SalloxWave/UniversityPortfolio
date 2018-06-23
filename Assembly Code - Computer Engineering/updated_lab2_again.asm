/*
 * lab2_new.asm
 *
 *  Created: 11/30/2017 5:12:38 PM
 *   Author: alejo720
 */ 


.def CHAR = r16
.def N = r20
.equ FREQ = 20
.equ INNER_FREQ = $10
.equ T = 45

; Setup stack
	ldi r16, HIGH(RAMEND)
	out SPH, r16
	ldi r16, LOW(RAMEND)
	out SPL, r16
	ldi ZH, HIGH(MESSAGE*2) ; Let Z point to message
	ldi ZL, LOW(MESSAGE*2)
	ldi r18, $80		; 1000000 = 2^7
	out DDRB, r18
	rcall GET_CHAR		; Get first ASCII-value from string

MORSE:
	cpi CHAR, $00	; CHAR == 0?
	breq MORSE		; Continue if loaded character in r16 is not 0 (end of string)
	rcall LOOKUP	; Convert ASCII-value to corresponding binary value
	rcall SEND		; Send the character
	ldi N, 2		; NOBEEP(2)
	rcall NOBEEP
	rcall GET_CHAR	; Get ASCII-value from string
	rjmp MORSE
GET_CHAR:
	lpm CHAR, Z+
	ret

LOOKUP:
	subi CHAR, $41	;r16-=$41
	brmi SPACE		; ASCII is less than 41 means it's a space
	push ZH			; Push Z pointing to MESSAGE to be able to use table
	push ZL
	ldi ZH, HIGH(BTAB*2)	; Let Z point to table
	ldi ZL, LOW(BTAB*2)
	add ZL, r16		; Go down in table according to character
	brcc HOPP	; If carry, increment Z-high
	inc ZH		; ZH++
HOPP:
	lpm CHAR, Z		; Read ASCII-value from table
	cln				; Clear negative flag
	pop ZL			; Let Z point back to message
	pop ZH
SPACE:
  ret

SPACE2:
	ldi N, 2		; NOBEEP(2), makes a total of 7 which creates a space
	rcall NOBEEP
	rjmp SLUT		; End of SEND sub routine

SEND:
	brmi SPACE2
	rcall GET_BIT
LOOP:
	breq SLUT	; Morse binary is finished
	ldi N, 1	; 1N NOBEEP between character morse parts
	brcc SKIP	; Carry cleared means you should not use a dash
	ldi N, 3	; Beep 3N
SKIP:
	rcall BEEP		; Send out a bit N times
	ldi N, 1		; NOBEEP(1)
	rcall NOBEEP
	rcall GET_BIT	; Get next bit in morse binary
	rjmp LOOP
SLUT:
	ret

GET_BIT:
	lsl CHAR		; Get next bit by left shifting
	ret
  
BEEP:
	ldi r19, T
BLOOP:
	sbi PORTB, 7	; Send out on B7
	rcall DELAY		; Frequency delay
	cbi PORTB, 7	; Stop sending out on B7
	rcall DELAY
	dec r19
	brne BLOOP		; Alternate between 1 and 0 T amount of times...
	dec N			; ...N amount of times
	brne BEEP
	ret

; Same as BEEP but without creating a sound
NOBEEP:
	ldi r19, T
NLOOP:
	rcall DELAY
	dec r19
	brne NLOOP
	dec N
	brne NOBEEP
	ret

; Frequency delay
DELAY:
	ldi r18, FREQ
DELAY_OUTER_LOOP:
	ldi r17, INNER_FREQ
DELAY_INNER_LOOP:
	dec r17
	brne DELAY_INNER_LOOP
	dec r18
	brne DELAY_OUTER_LOOP
	ret
  
MESSAGE: .db "SS SS SS", $00
BTAB: .db $60, $88, $A8, $90, $40, $28, $D0, $08, $20, $78, $B0, $48, $E0, $A0, $F0, $68, $D8, $50, $10, $C0, $30, $18, $70, $98, $B8, $C8 