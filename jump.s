@ jump.s

.global jump
jump:
	cmp r0, #0  @if davies0>falling is zero
	beq .fall
	.fall:
		mov r1, #0
		sub r1, r1, #900
		mov r0, #1
	b .done
	.done:
		mov pc, lr
