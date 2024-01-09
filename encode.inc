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
  CJNE  TIMER_INTERVALS,#0,DECREASE
  JMP   TIMER_END
DECREASE:
  DEC   TIMER_INTERVALS   ; 4 ms have passed 
TIMER_END:
  CLR   TF0
  MOV 	TL0,#0	
  MOV 	TH0,#131
  RETI

; Morse code encoding algorithm using buzzer
; 600 Hz, 30 WPM
ENCODE:
  JB    B2,ENCODE     ; Wait until B2 is unpressed
  MOV   CURSOR_POSITION,#0
  ACALL CLEAR_SCREEN
ENCODE_LOOP:          ; Encode loop, fetch characters from the buffer and turn them into sound
  ; Exit the mode
  JB    B2,ENCODE_END
  ; Check whether a new element is available
  CJNE  READY,#1,ENCODE_LOOP
ENCODE_NEXT:
  CALL  ENCODE_CHARACTER
  MOV   READY,#0
  JMP   ENCODE_LOOP
ENCODE_END:
  MOV   READY,#0
  MOV   CHAR,#0
  RET

; Encode and generate buzzer sound for a single character
ENCODE_CHARACTER:
  MOV   A,CHAR
  ; Loop for finding the character in the lookup table
  MOV   B,A
  MOV   DPTR,#MORSE_TABLE
LOOKUP_LOOP:
  MOV   A,#0
  MOVC  A,@A+DPTR
  
  ; If ACC = 4, the end of the table is reached
  CJNE  A,#4,LOOKUP_1
  RET

LOOKUP_1:
  CJNE  A,B,LOOKUP_NEXT
  ; Found the character index
  ; Display the character
  CALL  SDATA
  INC   CURSOR_POSITION
  ; If cursor position = 16, jump to the second line
  CJNE  CURSOR_POSITION,#16,CURSOR_NEXT_ENC
  ACALL MOVE_TO_LINE2
  JMP   GEN_SOUND
CURSOR_NEXT_ENC:
  ; If cursor position = 32, erase the screen
  CJNE  CURSOR_POSITION,#32,GEN_SOUND
  ACALL CLEAR_SCREEN
  MOV   CURSOR_POSITION,#1
  MOV   A,B
  CALL  SDATA
GEN_SOUND:
  ; Generate sound
  MOV   A,B
  JMP   GENERATE_SOUND
LOOKUP_NEXT:
  INC   DPTR
  JMP   LOOKUP_LOOP
GENERATE_SOUND:
  INC   DPTR
SOUND_LOOP:
  MOV   A,#0
  MOVC  A,@A+DPTR
  CJNE  A,#0,SOUND_NEXT
  ; The sound ends, exit
  JMP   SOUND_EXIT
SOUND_NEXT:
  CJNE  A,#1,DOT_SOUND
DASH_SOUND:
  ; 120 ms = 30 * 4 ms
  ; 600 ms = 150 * 4 ms
  MOV   TIMER_INTERVALS,#150
  JMP   PLAY_SOUND
DOT_SOUND:
  ; 40 ms  : 10
  ; 200 ms : 50
  MOV   TIMER_INTERVALS,#50
PLAY_SOUND:           ; Play the sound until the time intervals are over (from 40 or 10 to 0)
  CJNE  TIMER_INTERVALS,#0,PLAY_SOUND_NEXT
  JMP   PLAY_SOUND_END
PLAY_SOUND_NEXT:
  ; Stay in this loop to invert the square wave each 833uS until the time intervals are over
  ; 833uS delay
  ACALL DELAY833US
  ; Invert the square wave for sound
  CPL   SQUARE_WAVE
  JMP   PLAY_SOUND
PLAY_SOUND_END:
  INC   DPTR
  ACALL DELAY40MS     ; 40 ms delay after each element of a letter

  ACALL DELAY40MS
  ACALL DELAY40MS
  ACALL DELAY40MS
  ACALL DELAY40MS

  JMP   SOUND_LOOP
SOUND_EXIT:
  RET

DELAY833US:			;@11.0592MHz
	NOP
	NOP
	NOP
	PUSH 30H
	PUSH 31H
	MOV 30H,#8
	MOV 31H,#37
NEXT833US:
	DJNZ 31H,NEXT833US
	DJNZ 30H,NEXT833US
	POP 31H
	POP 30H
	RET

DELAY40MS:			;@11.0592MHz
	NOP
	PUSH 30H
	PUSH 31H
	PUSH 32H
	MOV 30H,#2
	MOV 31H,#89
	MOV 32H,#56
NEXT40MS:
	DJNZ 32H,NEXT40MS
	DJNZ 31H,NEXT40MS
	DJNZ 30H,NEXT40MS
	POP 32H
	POP 31H
	POP 30H
	RET