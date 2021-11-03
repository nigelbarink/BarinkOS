/*
* Interupt handlers
*/

.globl irs0
irs0:
	cli 
	push $0
	push $0
	jmp irs_common

.globl irs1
irs1:
	cli 
	push $0
	push $1
	jmp irs_common

.globl irs2
irs2:
	cli 
	push $0
	push $2
	jmp irs_common

.globl irs3
irs3:
	cli 
	push $0
	push $3
	jmp irs_common

.globl irs4
irs4:
	cli 
	push $0
	push $4
	jmp irs_common

.globl irs5
irs5:
	cli 
	push $0
	push $5
	jmp irs_common

.globl irs6
irs6:
	cli 
	push $0
	push $6
	jmp irs_common

.globl irs7
irs7:
	cli 
	push $0
	push $7
	jmp irs_common

.globl irs8
irs8:
	cli 
	push $0
	push $8
	jmp irs_common

.globl irs9
irs9:
	cli 
	push $0
	push $9
	jmp irs_common

.globl irs10
irs10:
	cli 
	push $0
	push $10
	jmp irs_common

.globl irs11
irs11:
	cli 
	push $0
	push $11
	jmp irs_common

.globl irs12
irs12:
	cli 
	push $0
	push $12
	jmp irs_common

.globl irs13
irs13:
	cli 
	push $13
	jmp irs_common

.globl irs14
irs14:
	cli 
	push $0
	push $14
	jmp irs_common

.globl irs15
irs15:
	cli 
	push $0
	push $15
	jmp irs_common

.globl irs16
irs16:
	cli 
	push $0
	push $16
	jmp irs_common

.globl irs17
irs17:
	cli 
	push $0
	push $17
	jmp irs_common

.globl irs18
irs18:
	cli 
	push $0
	push $18
	jmp irs_common

.globl irs19
irs19:
	cli 
	push $0
	push $19
	jmp irs_common

.globl irs20
irs20:
	cli 
	push $0
	push $20
	jmp irs_common

.globl irs21
irs21:
	cli 
	push $0
	push $21
	jmp irs_common

.globl irs22
irs22:
	cli 
	push $0
	push $22
	jmp irs_common

.globl irs23
irs23:
	cli 
	push $0
	push $23
	jmp irs_common

.globl irs24
irs24:
	cli 
	push $0
	push $24
	jmp irs_common

.globl irs25
irs25:
	cli 
	push $0
	push $25
	jmp irs_common

.globl irs26
irs26:
	cli 
	push $0
	push $26
	jmp irs_common

.globl irs27
irs27:
	cli 
	push $0
	push $27
	jmp irs_common

.globl irs28
irs28:
	cli 
	push $0
	push $28
	jmp irs_common

.globl irs29
irs29:
	cli 
	push $0
	push $29
	jmp irs_common

.globl irs30
irs30:
	cli 
	push $0
	push $30
	jmp irs_common

.globl irs31
irs31:
	cli 
	push $0
	push $31
	jmp irs_common


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

	call irs_handler

	pop %eax

	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	popa
	add  $8, %esp  # cleans push error and irs code
	sti
	iret # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
