.globl irq0

.macro IRQ NAME, VECTOR
    .globl irq\NAME
    irq\NAME:
        cli
        push $0
        push \VECTOR
        jmp irq_common
.endm

IRQ 0 $0
IRQ 1 $1
IRQ 2 $2
IRQ 3 $3
IRQ 4 $4
IRQ 5 $5
IRQ 6 $6
IRQ 7 $7
IRQ 8 $8
IRQ 9 $9
IRQ 10 $10
IRQ 11 $11
IRQ 12 $12
IRQ 13 $13
IRQ 14 $14
IRQ 15 $15



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
