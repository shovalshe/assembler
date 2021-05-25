;actions tests 2
A: .data 7
.entry L
.extern G

cmp #-43 ,   L
cmp #-43 ,   r0
cmp #-2049 ,   r0
cmp #43 ,   A
cmp #-43 ,   G
cmp #-43 ,   %LIST
cmp #-43 ,   7A
cmp #-43 ,   #1
cmp F, F
cmp F, L
cmp F, A
cmp F, r4
cmp F , #-34
cmp F   , %T
cmp     r5, T
cmp r4, #1
cmp r0, %L
cmp  %T, T

lea #-43 ,   L
lea #-43 ,   r0
lea #-2049 ,   r0
lea #43 ,   A
lea #-43 ,   G
lea #-43 ,   %LIST
lea #-43 ,   7A
lea #-43 ,   #1
lea F, F
lea F, L
lea F, A
lea F, r4
lea r7, J65
lea F , #-34
lea F   , %T
lea     r5, T
lea r4, #1
lea r0, %L
lea  %T, T

clr #-43
clr r0
clr #-2049
clr #A
clr G
clr %LIST
clr 7A
clr 
clr F, F
clr F ,
clr 