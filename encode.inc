SQUARE_WAVE EQU P0.5

; Initialize 4 ms timer
BUZZER_TIMER_INIT:
  ; 4 ms timer
  MOV 	TL0,#0	
  MOV 	TH0,#131
  SETB	TR0
  ; Interrupt enable register
  SETB  ET0   ; Timer Interrupt
  CLR   TF0
  RET

TIMER0INT:
  RETI

; Morse code encoding algorithm using buzzer
; 600 Hz, 30 WPM
ENCODE:
  JB    B2,ENCODE     ; Wait until B2 is unpressed
  ACALL CLEAR_SCREEN
ENCODE_LOOP:          ; Encode loop, fetch characters from the buffer and turn them into sound
  ;
  RET