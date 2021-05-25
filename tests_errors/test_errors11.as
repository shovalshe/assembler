;actions tests 4
A: .data 7
.entry L
.extern G

prn #-43
prn r0
prn #-2049
prn #A
prn G
prn %LIST
prn 7A
prn ,
prn F, F
prn F ,
prn 

rts #-43
rts r0
rts #-2049
rts #A
rts G
rts %LIST
rts 7A
rts 
rts F, F
rts F ,
rts
rts          

stop #-43
stop r0
stop #-2049
stop #A
stop G
stop %LIST
stop 7A
stop 
stop F, F
stop F ,
stop
stop     


cmp #, r1
sub r1, # 
jmp %7
bne %, 0
cmp # , r1
cmp #r, r1
cmp #-0, r1
cmp #+9, r1
cmp #+U, r1
cmp #4, aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
cmp #4, aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

