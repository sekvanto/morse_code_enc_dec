CHAR    EQU R6
READY   EQU R7

INIT_UART_BUFFER:
  MOV   CHAR,#0
  MOV   READY,#0

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
  
  ; Check whether the current character is read already
  CJNE  READY,#0,RX_END

  MOV   A, SBUF         ; Read character from UART buffer
  MOV   CHAR,A
  MOV   READY,#1
RX_END:
  CLR   RI
  POP   B
  POP   ACC
  RETI