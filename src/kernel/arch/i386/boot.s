/*
 * Multiboot 
 */
.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
 

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
 
.section .text

/*
* Interupt handlers
*/
# NOTE: I have no clue how I should use these macros.
# Have tried to use them in a myriad of ways, none would actually work
.macro irq_NoErrCode code:req
	.globl irq\code
	irq\code:
		cli 
		pushb $0
		pushb \code
		jmp irq_common
.endm

.macro irq_ErrCode code
	.globl irq\code
	irq\code:
		cli
		pushb \code
		jmp irq_common
.endm

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

.globl irq16
irq16:
	cli 
	push $0
	push $16
	jmp irq_common

.globl irq17
irq17:
	cli 
	push $0
	push $17
	jmp irq_common

.globl irq18
irq18:
	cli 
	push $0
	push $18
	jmp irq_common

.globl irq19
irq19:
	cli 
	push $0
	push $19
	jmp irq_common

.globl irq20
irq20:
	cli 
	push $0
	push $20
	jmp irq_common

.globl irq21
irq21:
	cli 
	push $0
	push $21
	jmp irq_common

.globl irq22
irq22:
	cli 
	push $0
	push $22
	jmp irq_common

.globl irq23
irq23:
	cli 
	push $0
	push $23
	jmp irq_common

.globl irq24
irq24:
	cli 
	push $0
	push $24
	jmp irq_common

.globl irq25
irq25:
	cli 
	push $0
	push $25
	jmp irq_common

.globl irq26
irq26:
	cli 
	push $0
	push $26
	jmp irq_common

.globl irq27
irq27:
	cli 
	push $0
	push $27
	jmp irq_common

.globl irq28
irq28:
	cli 
	push $0
	push $28
	jmp irq_common

.globl irq29
irq29:
	cli 
	push $0
	push $29
	jmp irq_common

.globl irq30
irq30:
	cli 
	push $0
	push $30
	jmp irq_common

.globl irq31
irq31:
	cli 
	push $0
	push $31
	jmp irq_common





irq_common:
	pusha 				# Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax


	mov %ds, %ax
	push %eax

	/* load the kernel data segment descriptor*/

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
	add  $8, %esp  # cleans push error and irq code
	sti
	iret # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

.globl idt_flush
idt_flush:
	mov 4(%esp), %eax
	lidt (%eax)
	ret

.globl enablePaging
enablePaging:
	push %ebp
	mov %esp, %ebp
	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0
	mov %ebp, %esp
	pop %ebp
	ret

.globl loadPageDirectory
loadPageDirectory:
	push %ebp
	mov %esp, %ebp
	mov 8(%esp), %eax
	mov %eax, %cr3
	mov %ebp, %esp
	pop %ebp
	ret

.global _start
.type _start, @function
_start:
	/*Setup the stack pointer to point to the beginning of our stack */
	/* I believe its a hight address growing down to lower adress for the stack on x86*/
	mov $stack_top, %esp
	call early_main
	cli
	load_gdt:
		lgdt gdt

		# set the segment selecters

		movw $0x10, %ax 
		movw %ax, %ds
		movw %ax, %es
		movw %ax, %fs
		movw %ax, %gs
		movw %ax, %ss
		ljmp $0x08, $flush 
		
		flush:
        
		
		#load idt
		call init_idt
        sti
        
        # Try enable A20
		# mov $0x2401, %ax
		# int $0x15


		# enable protected mode
		mov %cr0, %eax 
		or $1, %eax
		mov %eax, %cr0

		
		call kernel_main


		cli
	1:	hlt
		jmp 1b
	


	
	/* Tell processor to use our gdt*/
	gdt: 
		.word (gdt_end - gdt_start -1) # Size of the GDT in bytes minus 1 for math reasons
		.int gdt_start # linear address of our GDT

	

	
.att_syntax


.size _start, . - _start


/*
* Create the GDT
*/
.section .data
gdt_start:
gdt_null:
.long 0x0
.long 0x0

gdt_kcode:
.word 0xFFFF             # limit
.word 0x0                # base
.byte 0x0                # base
.byte 0b10011010         # 1st flags | type flags
.byte 0b11001111         # 2nd flags | limit
.byte 0x0                # base

gdt_kdata:
.word 0xFFFF             # limit
.word 0x0                # base
.byte 0x0                # base
.byte 0b10010010         # 1st flags | type flags
.byte 0b11001111         # 2nd flags | limit
.byte 0x0                # base
gdt_end: