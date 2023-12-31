; Reserve 30 bytes for BUFFER (each char is 0-255 which takes 8 bits or 1 byte, same as ACC)
BUFFER_SIZE EQU 30             ; Define the buffer size
BUFFER:     DS  BUFFER_SIZE    ; Allocate buffer

; Buffer head increases whenever a symbol is added.
; Buffer tail is increases whenever a symbol is retrieved, to catch up with the tail
; When the tail exceeds the final index in the allocated array space (e.g. 30)
; it is set to 0 circularly.
; When the head catches up with the tail, this means the maximum size is reached
; And no new elements can be added until some space is freed by the reader
BUF_HEAD  EQU  R6       ; Head index for the buffer (i.e. 0-29)
BUF_TAIL  EQU  R7

INIT_UART_BUFFER:
  MOV   BUF_HEAD,#0      ; Initialize BUF_HEAD (BUFFER head index) as 0
  MOV   BUF_TAIL,#0
 
  SETB  ES              ; Serial Port Interrupt
  CLR   TI
  CLR   RI
  SETB  EA
  SETB  PS              ; Priority

  MOV   TMOD,#00100000B ; Timer 1, Mode 2
  MOV   TL1,#0E6H       ; Baud rate=2400
  MOV   TH1,#0E6H
  ORL   PCON,#80H       ; SMOD=1, functions as UART Baud rate generator
  SETB  TR1             ; Timer Run
  
  CLR   SM0          
  SETB  SM1             ; Serial Port Mode 1
  CLR   SM2
  SETB  REN             ; Receive Enable
  RET

; UART interrupt routine
UART_RECEIVE:
  PUSH  ACC
  PUSH  B
  ; If theres nothing to receive, exit
  JNB   RI,RX_END
  ; If the mode currently isnt Encoding, exit
  CJNE  CURRENTLY_ENCODING,#1,RX_END
//  MOV   B, BUF_TAIL     ; Store buffer size in B
//  ; Check if BUF_HEAD has reached or exceeded the end of BUFFER
//  MOV   A, BUF_HEAD
//  CJNE  A, B, NOT_AT_END
//  JMP   RX_END          ; At end, dont do anything
NOT_AT_END:
  ; Save original DPL
  PUSH DPL
  MOV   DPTR,#BUFFER
  MOV   A,BUF_HEAD
  ; Add buffer head index to DPTR (DPL - lower byte of DPTR)
  ADD   A,DPL
  MOV   DPL,A

  MOV   A, SBUF         ; Read character from UART buffer
  MOVX  @DPTR, A        ; Store character in the buffer
  INC   BUF_HEAD        ; Increment head pointer
  POP   DPL
  CLR   RI
RX_END:
  POP   B
  POP   ACC
  RETI