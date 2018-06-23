/*
 * lab2.asm
 *
 *  Created: 11/28/2017 2:09:03 PM
 *   Author: alejo720
 */ 

 ; Replace with your application code
.def CHAR = r16
.def TIMES = r20
.equ FREQ = 30
.equ INNER_FREQ = $10
.equ LENGTH = 45
.equ ZERO = $00

; setup stack
ldi r16, HIGH(RAMEND)
out SPH, r16
ldi r16, LOW(RAMEND)
out SPL, r16
ldi ZH, HIGH(MESSAGE*2) ; Let Z point to message
ldi ZL, LOW(MESSAGE*2)
ldi r18, $80
out DDRB, r18

rcall GET_CHAR ; Get first ASCII-value from string
MORSE:
  cpi CHAR, ZERO ; Check if r16 is 0
  breq MORSE ; Continue if loaded character in r16 is not 0 (end of string)
  rcall LOOKUP ; Convert ASCII-value to corresponding binary value
  rcall SEND ; Send the character
  ldi TIMES, 2
  rcall NOBEEP ; Wait three time units between character
  rcall GET_CHAR ; Get ASCII-value from string
  rjmp MORSE

GET_CHAR:
  lpm CHAR, Z+
  ret

LOOKUP:
  subi CHAR, $41  ;r16-=$41
  brmi SPACE ; ASCII is less than 41 means it's a space
  push ZH ; Push Z pointing to MESSAGE to be able to use table
  push ZL
  ldi ZH, HIGH(BTAB*2) ; Let Z point to table
  ldi ZL, LOW(BTAB*2)
  add ZL, r16 ; Go down in table according to character
  brcc HOPP ; Branch if carry cleared, skip ZH++ if carry cleared
  inc ZH ; ZH++
HOPP:
  lpm CHAR, Z ; Read ASCII-value from table
  cln
  pop ZL
  pop ZH
SPACE:
  ret

SPACE2:
  ldi TIMES, 2
  rcall NOBEEP
  rjmp SLUT

SEND:
  brmi SPACE2
  rcall GET_BIT
LOOP:
  breq SLUT
  ldi TIMES, 1
  brcc SKIP
  ldi TIMES, 3
SKIP:
  rcall BEEP
  ldi TIMES, 1
  rcall NOBEEP
  rcall GET_BIT
  rjmp LOOP
SLUT:
  ret

GET_BIT:
  lsl CHAR
  ret
  
BEEP:
  ldi r19, LENGTH
BLOOP:
  sbi PORTB, 7
  rcall DELAY
  cbi PORTB, 7
  dec r19
  brne BLOOP
  dec TIMES
  brne BEEP
  ret

NOBEEP:
  ldi r19, LENGTH
NLOOP:
  rcall DELAY
  dec r19
  brne NLOOP
  dec TIMES
  brne NOBEEP
  ret

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
  
MESSAGE: .db "DATORTEKNIK", $00
BTAB: .db $60, $88, $A8, $90, $40, $28, $D0, $08, $20, $78, $B0, $48, $E0, $A0, $F0, $68, $D8, $50, $10, $C0, $30, $18, $70, $98, $B8, $C8 


