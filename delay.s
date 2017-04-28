@ delay.s

/*kills some time*/
.global delay
delay:
	mov r1, #0 
	mov r2, r0 
	.top:
		cmp r2, r1 
		beq .done 
		add r1, r1, #1
		add r0, r0, r0
		b .top
	.done:
		mov pc, lr
