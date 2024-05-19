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

.global AddShuckleFunctionality_hook
AddShuckleFunctionality_hook:

bl AddShuckleFunctionality

ldr r2, =0x02023D74
ldr r3, [r2]
add r3, #1
str r3, [r2]
ldr r3, =0x08747BF8 | 1
bx r3

.pool


/* Put 00 4A 10 47 XX XX XX 08 at 0x7FF90 (hook via r2) */

/*
----------------------------------------------------------
This routine changes the address to load Trainer Data from
to be the Battle Tower Trainer RAM (0x0203b0d8) if the
index number of the current trainer is 0x0.

Further edited to restore Jambo's pre-battle mugshot code.
----------------------------------------------------------
*/
.equ TrainerData,       0x0203FEC0
.equ TrainerTable,      0x0823EAC8

Start7FF90:
    push {r2}
    ldrh r2, [r4]
    cmp r2, #0x0
    beq LoadBattleTower7FF90
    ldr r1, =TrainerTable
    lsl r0, r2, #0x2
    add r0, r0, r2
    lsl r0, r0, #0x3
    add r0, r0, r1
    b Return7FF90

LoadBattleTower7FF90:
    ldr r1, =TrainerData
    mov r0, r1

Return7FF90:
    pop {r2}
    ldr r5, .Return7FF90
    bx r5

.align 2
//61 72 C2 09
// offset of mugshothackone from https://www.pokecommunity.com/threads/pre-battle-mugshots-in-firered.240183/
.Return7FF90:           .word 0x09C27231
.pool
