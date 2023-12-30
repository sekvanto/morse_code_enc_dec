; Erase one character
ERASE:
  ; Shift position to the left by 1
  MOV   A,#00010000B
  CALL  COMMAND
  ; Output space
  MOV   A,#00100000B
  CALL  SDATA
  ; Shift position to the left by 1
  MOV   A,#00010000B
  CALL  COMMAND
  RET

; Display init message (2-lines)
DISPLAY_INIT_MSG:
  MOV	  DPTR,#INIT_MSG
  ACALL DISPLAY_LINE1
  MOV   DPTR,#INIT_MSG_2
  ACALL DISPLAY_LINE2
  RET

; Display menu
DISPLAY_MENU:
  MOV   DPTR,#MENU_LINE1
  ACALL DISPLAY_LINE1
  MOV   DPTR,#MENU_LINE2
  ACALL DISPLAY_LINE2
  RET

; The driver for displaying line 1
DISPLAY_LINE1:
  ACALL CLEAR_SCREEN
DL1: 			
	CLR	  A
	MOVC  A,@A+DPTR  ; next character
	JZ    DL1END    ; second line of the message
	CALL  SDATA			
	INC	  DPTR					
	JMP	  DL1
DL1END:
  ACALL DELAY
  RET

; The driver for displaying line 2
DISPLAY_LINE2:
  MOV   A,#11000000B    ; move cursor to the head of the second line
  CALL  COMMAND
DL2:
  CLR   A
  MOVC  A,@A+DPTR
  JZ    DL2END
  CALL  SDATA
  INC   DPTR
  JMP   DL2
DL2END:
  ACALL DELAY
  RET

; Clear the screen
CLEAR_SCREEN:
  ; Clear the screen	
	MOV	  A,#00000001B		
	CALL  COMMAND
  ; Return the cursor to the home position
  MOV   A,#00000010B
  CALL  COMMAND

; Initialize LCD when the device boots up
LCD_INIT:
  ;5ms
	;waiting for the LCD to turn on
	MOV	R5,#0FAH
	CALL DELAY
	
	;Settings:
	;8-bit data access
	;2 columns
	;5x7 characters
	MOV	A,#00111011B							
	CALL COMMAND

	;Turn on the display
	;no cursor blinking				
	MOV	A,#00001110B	
	CALL COMMAND
	
	;Clear the screen	
	MOV	A,#00000001B		
	CALL COMMAND
	
	;2ms delay
	MOV	R5,#00FFH		
	CALL DELAY
	
	;DD RAM address = 0
	MOV	A,#10000000B		
	CALL COMMAND
  RET

; Send a command to LCD
COMMAND:	      
	MOV	P2,A ; Command data			
	MOV	P1,#00000100B ;E=1,RW=0,RS=0
	CALL DELAY			
	MOV	P1,#00000000B ;E=0,RW=0,RS=0
	MOV	R5,#2H ; Short delay				
	CALL DELAY
	RET

; Send data to display
SDATA:		
	MOV	P2,A
	MOV	P1,#00000101B ;E=1,RW=0,RS=1
	CALL DELAY
	MOV	P1,#00000001B ;E=0,RW=0,RS=1
	MOV	P1,#00000000B ;E=0,RW=0,RS=0
	MOV	R5,#0FAH ;5ms
	MOV	R3,#019H
WAIT:
	CALL DELAY
	DJNZ R3,WAIT
	RET

INIT_MSG:
	DB	"Welcome to the",0
INIT_MSG_2:
  DB "morse code tool!",0         

MENU_LINE1:
  DB  "B1: Decode",0
MENU_LINE2:
  DB  "B2: Encode",0

DELAY:
	MOV	R6,#02H
DELAY1:
	MOV	R7,#05H
DELAY2:
	DJNZ	R7,DELAY2
	DJNZ	R6,DELAY1
	DJNZ	R5,DELAY
	RET