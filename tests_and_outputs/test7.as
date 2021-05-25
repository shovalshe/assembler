A: .data 7
.entry L
.extern G

D: .data -1
jmp %B
L: jmp G

B: bne %L
bne G

.data 3
K: .string ""
jsr %B
jsr G

red  r7
    red  A