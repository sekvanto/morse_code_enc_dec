; Buttons
DOT_B   EQU P0.2
DASH_B  EQU P0.3
STOP_B  EQU P0.4

INDEX   EQU R0

; Morse code decoding algorithm
DECODE:
  ACALL CLEAR_SCREEN
  ; Binary tree is used for decoding 
DECODE_LOOP:
  ACALL DECODE_SYMBOL
  JMP   DECODE_LOOP
//ERASE_CH:
//  ACALL ERASE
//  JMP   DECODE_LOOP
DECODE_END:
  JMP   DECODE_END

; Decode one symbol and display it
DECODE_SYMBOL:
  ; Exit the mode
  ;JB    B2,DECODE_END
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
  ACALL ERASE
ERASE_DEBOUNCE:
  JB    B1,ERASE_DEBOUNCE    ; Wait until erase is unpressed
  RET
STOP:
  ; Display character
  ACALL DISPLAY_CHARACTER
  MOV   INDEX,#0
STOP_DEBOUNCE:
  JB    STOP_B,STOP_DEBOUNCE ; Wait until stop is unpressed
  RET

; Index <= 65
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
            DB '=', '/', ' ', ' ', ' ', ' ', ' ', ' ' 
            DB ' ', ' ', '7', ' ', ' ', ' ', '8', ' ' 
            DB '9', '0'