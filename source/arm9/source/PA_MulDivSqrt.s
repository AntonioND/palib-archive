.arch armv5te

.arm
.text

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Q12 fixed point multiplication function                     @
@ Mainly used to support 64-bit multiplications in THUMB code @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ int PA_mulf32(int a, int b);

.align 2
.global PA_mulf32
PA_mulf32:               @ int PA_mulf32(int a, int b){
	smull r2, r3, r0, r1 @     long long result = a * b;
	mov r0, r2, lsr #12  @     result = result >> 12;
	orr r0, r3, lsl #20  @
	bx lr                @     return (int) result;
	                     @ }

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Q12 fixed point division function @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ int PA_divf32(int a, int b);

.align 2
.global PA_divf32

PA_divf32:
	@ Check for division by zero
	movs r1, r1
	beq .Ldivision_by_zero
	
	@ Address base
	mov r3, #0x04000000
	orr r3, r3, #0x280
	
	@ Write denominator
	str r1, [r3, #0x18]
	
	@ Write numerator
	mov r1, r0, asr #20
	mov r0, r0, lsl #12
	strd r0, [r3, #0x10]
	
	@ Signal the hardware
	mov r0, #1
	str r0, [r3]
	
	@ Wait for the division to be completed
.Ldivloop:
	ldr r0, [r3]
	tst r0, #0x8000
	bne .Ldivloop
	
	@ Load the result
	ldr r0, [r3, #0x20]
	
	@ Return
	bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Q12 fixed point modulo function @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ int PA_modf32(int a, int b);

.align 2
.global PA_modf32

PA_modf32:
	@ Check for division by zero
	movs r1, r1
	beq .Ldivision_by_zero
	
	@ Address base
	mov r3, #0x04000000
	orr r3, r3, #0x280

	@ Write numerator
	str r0, [r3, #0x10]	

	@ Write denominator
	str r1, [r3, #0x18]
	
	@ Signal the hardware
	mov r0, #0
	str r0, [r3]
	
	@ Wait for the modulo to be completed
.Lmodloop:
	ldr r0, [r3]
	tst r0, #0x8000
	bne .Lmodloop
	
	@ Load the result
	ldr r0, [r3, #0x28]
	
	@ Return
	bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Q12 square root function @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ int PA_sqrtf32(int a);

.align 2
.global PA_sqrtf32

PA_sqrtf32:
	@ Fix negative values
	movs r0, r0
	rsbmi r0, r0, #0
	
	@ Address base
	mov r3, #0x04000000
	orr r3, r3, #0x2B0
	
	@ Write the parameter
	mov r1, r0, lsr #20
	mov r0, r0, lsl #12
	strd r0, [r3, #0x08]
	
	@ Signal the hardware
	mov r0, #1
	str r0, [r3]
	
	@ Wait for the square root to be completed
.Lsqrtloop:
	ldr r0, [r3]
	tst r0, #0x8000
	bne .Lsqrtloop
	
	@ Load the result
	ldr r0, [r3, #0x04]
	
	@ Return
	bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Crappy division by zero handler @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align 2
.Ldivision_by_zero:
	ldr r0, =.Lcondition
	ldr r1, =.Lmessage
	ldr r2, =.Lfile
	mov r3, #35
	b _PA_Assert

.align 2
.Lcondition:
	.ascii "b != 0\000"

.align 2
.Lmessage:
	.ascii "Division by zero\000"

.align 2
.Lfile:
	.asciz __FILE__
