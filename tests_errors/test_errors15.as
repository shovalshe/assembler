; second pass test 4

.extern W
MAIN: add r3, LIST
LOOP: prn #48
lea W, r6
inc r6
mov r3, K
sub r1, r4
bne END
cmp K, #-6
bne %K
dec W
.entry MAIN
jmp %W
add L3, L3
END: stop
STR: .string "abcd"
 .data 6, -9
.data -100
K: .data 31
.entry STT
.entry MAIN
.entry M
.entry LIST
