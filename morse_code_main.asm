ORG	0000H
JMP	START
ORG	0050H
ORG 0BH
JMP TIMER0INT      ; 4 ms timer interrupt
ORG 23H
JMP UART_RECEIVE   ; UART for receiving input text

B1  EQU P0.0
B2  EQU P0.1

TIMER_INTERVALS     EQU R3    ; Timer intervals left until the sound turns off (e.g. a 40ms sound => 10 intervals)
CURRENTLY_ENCODING  EQU R4    ; Set to 1 when the Encoding mode entered

START:
  ; Initialize ports with 0
  CLR   B1
  CLR   B2
  CLR   DOT_B
  CLR   DASH_B
  CLR   STOP_B
  CLR   SQUARE_WAVE
  MOV   INDEX,#0
  MOV   CURSOR_POSITION,#0
  MOV   CURRENTLY_ENCODING,#0
  MOV   TIMER_INTERVALS,#0
  ; Initialize timers and interrupts
  ACALL INIT_UART_BUFFER
  ACALL BUZZER_TIMER_INIT
MAIN:
  ACALL LCD_INIT
  ACALL DISPLAY_INIT_MSG
ENDMES:
  JB  B1,MENU   ; Press (B1) lower button to enter the menu
  JMP ENDMES

; Display menu
MENU:
  ACALL DISPLAY_MENU
MENU_DEBOUNCE:
  JB  B1,MENU_DEBOUNCE   ; Wait until B1 isnt pressed
ENDMENU:
  ; B1: jump to decode morse code input into text
  ; B2: jump to encode text (buzzer + LED output)
  JB  B1,DECODE_CALL
  JB  B2,ENCODE_CALL
  JMP ENDMENU
DECODE_CALL:
  LCALL DECODE
  ACALL DISPLAY_MENU
  JMP   ENDMENU
ENCODE_CALL:
  MOV   CURRENTLY_ENCODING,#1
  LCALL ENCODE
  ACALL DISPLAY_MENU
  MOV   CURRENTLY_ENCODING,#0
  JMP   ENDMENU

#include "decode.inc"
#include "buffer.inc"
#include "encode.inc"
#include "lcd_display.inc"
#include "utilities.inc"

END