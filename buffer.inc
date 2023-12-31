; Reserve 30 bytes for BUFFER (each char is 0-255 which takes 8 bits or 1 byte, same as ACC)
BUFFER_SIZE EQU 30             ; Define the buffer size
BUFFER:     DS  BUFFER_SIZE    ; Allocate buffer

BUF_HEAD    EQU R3      ; Head index for buffer (i.e. 0-29)

INIT_UART_BUFFER:
  MOV   BUF_HEAD, #0    ; Initialize BUF_HEAD (BUFFER head index) as 0
 
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
  JNB   RI,RX_END
  MOV A, BUFFER_SIZE
  MOV B, A              ; Store buffer size in B
  ; Check if BUF_HEAD has reached or exceeded the end of BUFFER
  MOV   A, BUF_HEAD
  CJNE  A, B, NOT_AT_END
  RETI                  ; At end, dont do anything
NOT_AT_END:
  //MOV A, BUF_HEAD
  MOV   DPTR,#BUFFER
  MOV   A, SBUF         ; Read character from UART buffer
  MOVX  @DPTR, A        ; Store character in the buffer
  INC   BUF_HEAD        ; Increment head pointer
  CLR   RI
RX_END:
  RETI