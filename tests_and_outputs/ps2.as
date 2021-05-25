; file ps.as


MAIN:    add r3,LIST
LOOP:    prn #48
         lea W,r6
STR:    .string "abcd"
LIST:   .data 6,-9
        .data -100

         inc r6
         mov r3,K
         sub r1,r4
         bne END
K:      .data 31
.entry LIST
.extern W
         cmp K,#-6
         bne %END
         dec W
.entry MAIN
         jmp %LOOP
         add L3,L3
END:     stop


.extern L3