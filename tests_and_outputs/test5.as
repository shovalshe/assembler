;

.data 0 , -1
mov #-43 ,   L
mov #-43 ,   r0
mov #43 ,   A
mov #-43 ,   G
mov F, F
T: mov F, L
mov F, A
mov F, r4
mov     r5, T

L: .data 1
L: .extern A
A: .extern G
add #-43 ,   L
add #-43 ,   r0
add #43 ,   A
add #-43 ,   G

add F, F
add F, L
add F, A
add F, r4

add     r5, T

.entry F
sub #-43 ,   L
sub #-43 ,   r0
sub #43 ,   A
sub #-43 ,   G

F: sub F, F
sub F, L
sub F, A
sub F, r4

sub     r5, T


cmp #-43 ,   L
cmp #-43 ,   r0
cmp #43 ,   A
cmp #-43 ,   G
cmp #-43 ,   #1
cmp F, F
cmp F, L
cmp F, A
cmp F, r4
cmp F , #-34
cmp     r5, T
cmp r4, #1


lea F, F
lea A,r5
lea F, r7



clr   r0        
clr G  
