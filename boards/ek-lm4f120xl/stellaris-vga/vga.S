  .file "vga.S"

  .data
  .align 2
  .globl LineCount

LineCount: .word 0

  .text
  .align 2
  .globl Timer0IntHandler
  .type Timer0IntHandler, %function

Timer0IntHandler:
  @ horizontal timing
  @ hfp 40 (1-40)
  @ hsp 128 (41-168)
  @ hbp 88 (169-256)
  @ hpx 800 (257-1056)
  @ total = 1056
  
  @ vertical timing
  @ vsp 4 (1-4)
  @ vbp 23 (5-27)
  @ vln 600 (28-627)
  @ vfp 1 (628)
  @ total = 628
  
  @ *********
  @ ** hfp ** [40 cycles]
  @ *********

  @ prologue [3 cycles]
  push {r3, lr}
 
  @ save regs [7 cycles]
  push {r1, r2, r3, r4, r5, r6}
  
  @ clear interrupt timer [5 cycles]
  mov r1, #1             @ TIMER_TIMA_TIMEOUT
  ldr r0, =0x40030000    @ TIMER0_BASE
  str r1, [r0, #0x24]    @ *(TIMER0_BASE + TIMER_O_ICR) = TIMER_TIMA_TIMEOUT

  @ ptr to GPIO_PORTB_BASE in r0 [3 cycles]
  @ (this will be persisted)
  ldr r0, =0x40005000    @ GPIO_PORTB_BASE
  nop

  @ ptr to GPIO_PORTB_BASE in r1 [3 cycles]
  @ (this will be persisted)
  ldr r1, =0x40024000    @ GPIO_PORTE_BASE
  nop

  @ ptr to line counter  [3 cycles]
  @ (this will be persisted)
  ldr r2, =LineCount
  nop

  @ waste cycles [16 cycles]
  mov r3, #0
loop_0: @ {3*4-1=11 cycles}
  add r3, #1
  cmp r3, #3
  bne loop_0 @2

  @ hsync setup [4 cycles]
  ldrb r3,[r0]
  mov r4, #0xfd
  and r3, r4
  
  @ *********
  @ ** hsp ** [128 cycles]
  @ *********

  @ set hsync low [2 cycles]
  str r3, [r0]  @ GPIO_PIN_1 clear
  
  @ reset line counter [8 cycles]
  ldr r4, =629
  cmp r3, r4
  beq .Lline_reset
  nop
  nop
  b .Lno_reset
.Lline_reset:
  mov r3, #0
  str r3, [r2]
.Lno_reset:
  
  @ inc line counter  [5 cycles]
  ldr r3, [r2]
  add r3, #1
  str r3, [r2]

  @ vsync on at line 1 [9 cycles]
  cmp r3, #1    @ now at start of vsp
  beq .Lvsync_on
  nop
  nop
  nop
  nop
  nop
  b .Lcont_0
.Lvsync_on:     @ {6 cycles}
  ldrb r3,[r0]
  mov r4, #0xfe
  and r3, r4
  str r3, [r0]  @ GPIO_PIN_0 clear
.Lcont_0:
 
  @ vsync off at line 5 [11 cycles]
  ldr r3, [r2]
  cmp r3, #5
  beq .Lvsync_off
  nop
  nop
  nop
  nop
  nop
  b .Lcont_1
.Lvsync_off:    @ {6 cycles}
  ldrb r3,[r0]
  mov r4, #1
  orr r3, r4
  str r3, [r0]  @ GPIO_PIN_0 set
.Lcont_1:
  
  @ activate pixels on line 28 [6 cycles]
  ldr r3, [r2]
  cmp r3, #28
  beq .Lactivate_px
  b .Lcont_2
.Lactivate_px:  @ {1 cycle}
  mov r6, #1
.Lcont_2:
  
  @ deactivate pixels on line 628 [7 cycles]
  ldr r3, [r2]
  ldr r4, =628
  cmp r3, r4
  beq .Ldeactivate_px
  b .Lcont_3
.Ldeactivate_px: @ {1 cycle}
  mov r6, #0
.Lcont_3:

  @ waste cycles [74 cycles] 
  mov r0, #0
.Lwasteloop_1:  @ {18*4-1=71 cycles}
  add r0, #1
  cmp r0, #14
  bne .Lwasteloop_1 @2
  nop
  nop

  @ hsync off [6 cycles]
  ldrb r3,[r0]
  mov r4, #2
  orr r3, r4            @ GPIO_PIN_1 set
  str r3, [r0]
  
  @ *********
  @ ** hbp ** [88 cycles]
  @ *********
  @ exit on vblank [3 cycles]
  cmp r6, #0
  beq end
  nop
  
  @ waste cycles [83 cycles]
  mov r3, #0
.Lwasteloop_2:  @ {20*4-1=79 cycles}
  add r3, #1
  cmp r3, #19
  bne .Lwasteloop_2
  nop
  nop
  nop
  
  @ set up color counter [1 cycle]
  mov r4, #0

  @ set initial rgb [1 cycle]
  mov r3, #0

  @ *********
  @ ** hpx ** [800 cycles]
  @ *********

  @ output pattern [800 cycles]
.Lcolor_loop:   @ {100*8-1=799}
  str r3, [r1]
  add r3, #1
  add r4, #1
  cmp r4, #100
  nop
  bne .Lcolor_loop
  nop
  
  @ *********

  @ horizontal blanking
  mov r3, #0
  str r3, [r1]

end:
  @ restore regs [7 cycles]
  pop {r1, r2, r3, r4, r5, r6}
  
  @ return from interrupt [4 cycles]
  pop {r3, pc}
  .word 0x40030000 @addmi r0, r3, r0

