DELAY_DEBOUNCE:
DELAY5MS:			;@11.0592MHz
	NOP
	NOP
	PUSH 30H
	PUSH 31H
	MOV 30H,#44
	MOV 31H,#2
NEXT_DEBOUNCE:
	DJNZ 31H,NEXT_DEBOUNCE
	DJNZ 30H,NEXT_DEBOUNCE
	POP 31H
	POP 30H
	RET

; Morse code lookup table
; Representing dot as 1 and dash as 2, and 0 as the end of a sequence
MORSE_TABLE: 
    DB 'a', 2, 1, 0               ; A -> .- 
    DB 'b', 1, 2, 2, 2, 0         ; B -> -...
    DB 'c', 1, 2, 1, 2, 0         ; C -> -.-.
    DB 'd', 1, 2, 2, 0            ; D -> -..
    DB 'e', 2, 0                  ; E -> .
    DB 'f', 2, 2, 1, 2, 0         ; F -> ..-.
    DB 'g', 1, 1, 2, 0            ; G -> --.
    DB 'h', 2, 2, 2, 2, 0         ; H -> ....
    DB 'i', 2, 2, 0               ; I -> ..
    DB 'j', 2, 1, 1, 1, 0         ; J -> .---
    DB 'k', 1, 2, 1, 0            ; K -> -.-
    DB 'l', 2, 1, 2, 2, 0         ; L -> .-..
    DB 'm', 1, 1, 0               ; M -> --
    DB 'n', 1, 2, 0               ; N -> -.
    DB 'o', 1, 1, 1, 0            ; O -> ---
    DB 'p', 2, 1, 1, 2, 0         ; P -> .--.
    DB 'q', 1, 1, 2, 1, 0         ; Q -> --.-
    DB 'r', 2, 1, 2, 0            ; R -> .-.
    DB 's', 2, 2, 2, 0            ; S -> ...
    DB 't', 1, 0                  ; T -> -
    DB 'u', 2, 2, 1, 0            ; U -> ..-
    DB 'v', 2, 2, 2, 1, 0         ; V -> ...-
    DB 'w', 2, 1, 1, 0            ; W -> .--
    DB 'x', 1, 2, 2, 1, 0         ; X -> -..-
    DB 'y', 1, 2, 1, 1, 0         ; Y -> -.-- 
    DB 'z', 1, 1, 2, 2, 0         ; Z -> --..

    ; Numbers
    DB '0', 1, 1, 1, 1, 1, 0      ; 0 -> -----
    DB '1', 2, 1, 1, 1, 1, 0      ; 1 -> .----
    DB '2', 2, 2, 1, 1, 1, 0      ; 2 -> ..---
    DB '3', 2, 2, 2, 1, 1, 0      ; 3 -> ...--
    DB '4', 2, 2, 2, 2, 1, 0      ; 4 -> ....-
    DB '5', 2, 2, 2, 2, 2, 0      ; 5 -> .....
    DB '6', 1, 2, 2, 2, 2, 0      ; 6 -> -....
    DB '7', 1, 1, 2, 2, 2, 0      ; 7 -> --...
    DB '8', 1, 1, 1, 2, 2, 0      ; 8 -> ---..
    DB '9', 1, 1, 1, 1, 2, 0      ; 9 -> ----.

    DB 4 ; END