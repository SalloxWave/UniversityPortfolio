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
.org 0x00
clr r16
ldi r16, $8F ; set bits 0-3 and 7 to write in port b
out DDRB, r16 ; sends it to port b
FOREVER:
	clr digit
	clr num
	sbic PINA, 0
	rcall READ_NUM
    breq FOREVER ; Om vi inte läste in något tal, loopa vidare
    out PORTB, num ; skriv ut talet till port b
    clr num
    ldi r16, 3 ; Half delay
	rcall HALFD
    rjmp FOREVER

READ_NUM: 
	  clr digit
      rcall DELAY ; vänta hela startbiten
      ldi r16, 3 ; Half delay
	  rcall HALFD ; vänta halva så att vi läser mitten på nästa bit
      in digit, PINA ; läs pin a (bit 0) till digit
	  add num, digit ; lägg till digit till num
      rcall DELAY
	  in digit, PINA ; bit 1
	  lsl digit
	  add num, digit
	  rcall DELAY
	  in digit, PINA ; bit 2
	  lsl digit
	  lsl digit
	  add num, digit
	  rcall DELAY
	  in digit, PINA ; bit 3
	  lsl digit
	  lsl digit
	  lsl digit
	  add num, digit
	  ret

DELAY:
  ldi     r16,5   ; Decimal bas
HALFD:
  sbi     PORTB,7
delayYttreLoop:
  ldi     r17,$FF
delayInreLoop:
  dec     r17
  brne    delayInreLoop
  dec     r16
  brne    delayYttreLoop
  cbi     PORTB,7
  ret

