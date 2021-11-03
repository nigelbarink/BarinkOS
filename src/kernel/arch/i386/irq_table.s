.globl irq0
irq0:
	cli 
	push $0
	push $0
	jmp irq_common

.globl irq1
irq1:
	cli 
	push $0
	push $1
	jmp irq_common

.globl irq2
irq2:
	cli 
	push $0
	push $2
	jmp irq_common

.globl irq3
irq3:
	cli 
	push $0
	push $3
	jmp irq_common

.globl irq4
irq4:
	cli 
	push $0
	push $4
	jmp irq_common

.globl irq5
irq5:
	cli 
	push $0
	push $5
	jmp irq_common

.globl irq6
irq6:
	cli 
	push $0
	push $6
	jmp irq_common

.globl irq7
irq7:
	cli 
	push $0
	push $7
	jmp irq_common

.globl irq8
irq8:
	cli 
	push $0
	push $8
	jmp irq_common

.globl irq9
irq9:
	cli 
	push $0
	push $9
	jmp irq_common

.globl irq10
irq10:
	cli 
	push $0
	push $10
	jmp irq_common

.globl irq11
irq11:
	cli 
	push $0
	push $11
	jmp irq_common


.globl irq12
irq12:
	cli 
	push $0
	push $12
	jmp irq_common

.globl irq13
irq13:
	cli 
	push $0
	push $13
	jmp irq_common

.globl irq14
irq14:
	cli 
	push $0
	push $14
	jmp irq_common

.globl irq15
irq15:
	cli 
	push $0
	push $15
	jmp irq_common

irq_common:
	pusha

	mov %ds, %ax
	push %eax

	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	call irq_handler

	pop %eax

	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	popa
	add  $8, %esp  # cleans push error and irs code
	sti
	iret # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
