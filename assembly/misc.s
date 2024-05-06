.text
.align 2
.thumb

.global RewriteTrainerStarCount
RewriteTrainerStarCount:
mov r0, r5
bl GetTrainerStarCount // shouldn't need to deal with the stack because r0-r3 are all garbo anyway
strb r0, [r5, #1] // store stars back into trainerCard->rse.stars or whatever
ldr r0, =0x0808998A | 1
bx r0

.pool


.global RewriteMaxCountInDexScreen
RewriteMaxCountInDexScreen:
// r0 is NationalDexEnabled
mov r1, #255
add r1, #1 // 256 total for regional
str r1, [sp]
cmp r0, #0
beq _returnTo810353A
add r1, #(386-256)
str r1, [sp]

_returnTo810353A:
ldr r0, =0x810353A | 1;
bx r0

.pool
