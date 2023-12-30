ORG	0000H
JMP	START
ORG	0050H

B1  EQU P0.0
B2  EQU P0.1

START:
  ; Initialize ports with 0
  CLR   B1
  CLR   B2
MAIN:
  ACALL LCD_INIT
  ACALL DISPLAY_INIT_MSG
ENDMES:
  JB  B1,MENU   ; Press (B1) lower button to enter the menu
	JMP ENDMES

; Display menu
MENU:
  ACALL DISPLAY_MENU
ENDMENU:
  ; B1: jump to decode morse code input into text
  ; B2: jump to encode text (buzzer + LED output)
  JB  B1,DECODE
  //JB  B2,ENCODE
  JMP ENDMENU

#include "lcd_display.inc"
#include "utilities.inc"

END