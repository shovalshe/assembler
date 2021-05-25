A: .data 7
.entry L
.extern G

not r0
D: .data -1
not r1
L: not G

inc r0
inc G
.entry L
.extern G
.data 3
K: .string ""
dec r0
dec G
