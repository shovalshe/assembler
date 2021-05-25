; second pass test 5

MAIN: add r3, LIST
.entry END
LOOP: prn #48
lea STR, r6
.entry LIST
.entry K
K: .entry MAIN
LOOP: .entry MAIN
S: .entry MAIN
MAIN: .entry END
inc r6
inc S
mov r3, K1
bne END
cmp vall, #-6
bne %END

.entry K
.entry END