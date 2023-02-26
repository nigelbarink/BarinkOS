.code32
/*
* Interupt handlers
*/

.macro ISR_NOERRORCODE NAME, VECTOR
	.globl irs\NAME
	irs\NAME:
		cli 
		push $0
		push \VECTOR
		jmp irs_common
.endm

.macro ISR_ERROCODE NAME, VECTOR
	.globl irs\NAME
	irs\NAME:
		cli
		push \VECTOR
		jmp irs_common
.endm


ISR_NOERRORCODE 0 $0
ISR_NOERRORCODE 1 $1
ISR_NOERRORCODE 2 $2
ISR_NOERRORCODE 3 $3
ISR_NOERRORCODE 4 $4
ISR_NOERRORCODE 5 $5
ISR_NOERRORCODE 6 $6
ISR_NOERRORCODE 7 $7
ISR_NOERRORCODE 8 $8
ISR_NOERRORCODE 9 $9
ISR_NOERRORCODE 10 $10
ISR_NOERRORCODE 11 $11
ISR_NOERRORCODE 12 $12
ISR_NOERRORCODE 13 $13
ISR_NOERRORCODE 14 $14
ISR_NOERRORCODE 15 $15
ISR_NOERRORCODE 16 $16
ISR_NOERRORCODE 17 $17
ISR_NOERRORCODE 18 $18
ISR_NOERRORCODE 19 $19
ISR_NOERRORCODE 20 $20
ISR_NOERRORCODE 21 $21
ISR_NOERRORCODE 22 $22
ISR_NOERRORCODE 23 $23
ISR_NOERRORCODE 24 $24
ISR_NOERRORCODE 25 $25
ISR_NOERRORCODE 26 $26
ISR_NOERRORCODE 27 $27
ISR_NOERRORCODE 28 $28
ISR_NOERRORCODE 29 $29
ISR_NOERRORCODE 30 $30
ISR_NOERRORCODE 31 $31
ISR_NOERRORCODE 50 $50

irs_common:
	pusha 				# Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

	mov %ds, %ax
	push %eax

	/* load the kernel data segment descriptor*/

	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	
	mov %esp, %eax
	push %eax 

	call irs_handler
	
	pop %eax        // pop stack pointer
	pop %ebx		// reload ther orignal data segment descriptor
	mov %bx, %ds
	mov %bx, %es
	mov %bx, %fs
	mov %bx, %gs

	popa
	add  $8, %esp  # cleans push error and irs code

	iret # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
