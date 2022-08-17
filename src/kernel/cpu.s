# Basic cpu functions 
.globl GetCR0
GetCR0:
	push %ebp # save the base pointer on the stack
	mov %esp, %ebp  # Set the base pointer to the current stack pointer 

	xor %eax, %eax # Clear EAX to make sure no weird stuff is going on
	mov %cr0, %eax # Copy the value of the CR0 register into EAX

	mov %ebp, %esp # restore the base pointer 
	pop %ebp
	ret

.globl GetCR4
GetCR4:
	push %ebp
	mov %esp, %ebp

	xor %eax, %eax
	mov %cr4, %eax 

	mov %ebp, %esp 
	pop %ebp
	ret



.globl GetEFLAGS
GetEFLAGS:
	push %ebp 
	mov %esp, %ebp

	xor %eax, %eax 
	pushfl # Push the EFLAGS register content onto the stack, should be pushfd but GAS apparently doesn't agree
	mov 4(%esp) , %eax

	mov %ebp, %esp
	pop %ebp
	ret