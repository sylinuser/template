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


.equ Flag, 0x2322

.align 2

.global DisablePrizeMoney2
DisablePrizeMoney2:
    push {r1}
    ldr r0, =Flag
    ldr r2, =(0x806E6D0 +1)
    bl bx_r2
    pop {r1}
    cmp r0, #0x1
    beq skipDisablePrize2
    add r0, r4, #0x0
    ldr r2, =(0x809fdd8 +1)
    bl bx_r2

skipDisablePrize2:
    ldr r2, =(0x80a0058 +1)
    bl bx_r2
    ldr r2, =(0x8054BEA +1)
    bl bx_r2

.pool

.align 2

bx_r2:
	bx r2
