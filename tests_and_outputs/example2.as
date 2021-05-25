;;;;;;;
;comment;
STR1: .string "abc123abc"
mov #-3, r2
add #-234, r2
clr r2
clr R2
jmp %LBL1
    .entry MAIN
MAIN: clr r3
bne  HERE
;CODE
add     #15, L1
inc R2
stop
sub  #-7 ,L2
; this is an error for checking -> jmp r1
HERE: add r7 , r1
dec L2
rts
R2: .data -1
jmp LBL1
bne HERE
STR: .string "abcd"
mov #8,  L1
    .entry   LIST
not r2
jsr HERE
LIST: .data   6,     -9
    .data -100
    not R2
    bne %JMP
red r3
lea JMP, L2
prn r3
mov r3, L1
dec r5
L2: .data 1,2
R1: .data 3
add L1, r4
        ;  another comment
; this is an error       sub 125, r3
JMP: sub R1, r1
sub r1, R1
inc r1
LBL1: add r7, L1
cmp L1, r3
    ; this is an error -> cmp 3,
    lea HERE, r3
cmp r4, L2
mov L1, r3
red L1
prn L1
rts
    .extern EX1
;LIST: .data 6, -9, 2
    .data -100, 4
    .data -3
; this is an error as R1 is not defined -> sub R1,R1
sub r1, r5
; DATA
L1: .string "A"
      ;  this is an error -> L1: .data "A"
; this is an error -> L8: .data 'A'
; this is an error -> L8: .string 'A'
mov r2,r7
    .entry MAIN
    .extern EX2
stop
;END
