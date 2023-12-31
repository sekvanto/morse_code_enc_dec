; Buttons
DOT_B   EQU P0.2
DASH_B  EQU P0.3
STOP_B  EQU P0.4

INDEX             EQU R1
CURSOR_POSITION   EQU R2

; Morse code decoding algorithm
DECODE:
  JB    B1,DECODE           ; Wait until B1 is unpressed 
  ACALL CLEAR_SCREEN
  MOV   CURSOR_POSITION,#0
  ; Binary tree is used for decoding 
DECODE_LOOP:
  MOV   INDEX,#0
  ACALL DECODE_SYMBOL
  ; Exit the mode
  JB    B2,DECODE_END
  JMP   DECODE_LOOP
DECODE_END:
  JB    B2,DECODE_END   ; Wait until B2 is unpressed
  RET

; Decode one symbol and display it
DECODE_SYMBOL:
  ; Exit the mode
  JB    B2,DECODE_END_SYM
  ; Erase
  JB    B1,ERASE_CH
  ; Dot not pressed
  JNB   DOT_B,DNEXT
  ACALL DOT
  JMP   DECODE_SYMBOL
DNEXT:
  ; Dash not pressed
  JNB   DASH_B,DNEXT1
  ACALL DASH
  JMP   DECODE_SYMBOL
DNEXT1:
  ; End character
  JB    STOP_B,STOP
  JMP   DECODE_SYMBOL
ERASE_CH:
  ; If index is already 0, dont do anything
  CJNE  CURSOR_POSITION,#0,ERASE_CH1
  JMP   ERASE_DEBOUNCE
ERASE_CH1:
  ; If the index is 16, jump to the end of the first line
  CJNE  CURSOR_POSITION,#16,ERASE_CH2
  ACALL MOVE_END_LINE1
  DEC   CURSOR_POSITION
  JMP   ERASE_DEBOUNCE
ERASE_CH2:
  ACALL ERASE
  DEC   CURSOR_POSITION
ERASE_DEBOUNCE:
  JB    B1,ERASE_DEBOUNCE    ; Wait until erase is unpressed
  RET
DECODE_END_SYM:
  RET
STOP:
  ; Display character
  ACALL DISPLAY_CHARACTER
  INC   CURSOR_POSITION      ; Increment cursor position
  ; If cursor position = 16, jump to the second line
  CJNE  CURSOR_POSITION,#16,CURSOR_NEXT
  ACALL MOVE_TO_LINE2
CURSOR_NEXT:
  ; If cursor position = 32, erase the screen
  CJNE  CURSOR_POSITION,#32,STOP_DEBOUNCE
  ACALL CLEAR_SCREEN
  MOV   CURSOR_POSITION,#1
  ACALL DISPLAY_CHARACTER
STOP_DEBOUNCE:
  JB    STOP_B,STOP_DEBOUNCE ; Wait until stop is unpressed
  RET

DISPLAY_CHARACTER:
  MOV   A,INDEX
  MOV   DPTR,#MORSE_TREE
  MOVC  A,@A+DPTR
  CJNE  A,' ',DISPLAY_CHARACTER1
  JMP   DISPLAY_CHARACTER_END
DISPLAY_CHARACTER1:
  CALL  SDATA
DISPLAY_CHARACTER_END:
  RET

; Dot added to the sequence
DOT:
   ; index = 2 * index + 1
   MOV  A,INDEX
   RL   A       ; Rotate left => multiply by 2
   INC  A
   MOV  INDEX,A
DOT_DEBOUNCE:   ; Wait until the dot isnt unpressed
  JB    DOT_B,DOT_DEBOUNCE
  RET

; Dash added to the sequence
DASH:
  ; index = 2 * index + 2
  MOV   A,INDEX
  RL    A
  INC   A
  INC   A
  MOV   INDEX,A
  DASH_DEBOUNCE:
  JB    DASH_B,DASH_DEBOUNCE
  RET

; Define the Morse code binary tree array
; Left children are at 2*index + 1, right children are at 2*index + 2.
; Left = dash, right = dot
MORSE_TREE: DB ' ', 'E', 'T', 'I', 'A', 'N', 'M', 'S' 
            DB 'U', 'R', 'W', 'D', 'K', 'G', 'O', 'H' 
            DB 'V', 'F', ' ', 'L', ' ', 'P', 'J', 'B' 
            DB 'X', 'C', 'Y', 'Z', 'Q', ' ', ' ', '5' 
            DB '4', ' ', '3', ' ', ' ', ' ', '2', ' ' 
            DB ' ', ' ', ' ', ' ', ' ', ' ', '1', '6' 
            DB ' ', ' ', ' ', ' ', ' ', ' ', ' ', '7' 
            DB ' ', ' ', ' ', '8', ' ', '9', '0'