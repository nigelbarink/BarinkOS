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
.include "./src/kernel/gdt/gdt.s"
.include "./src/kernel/irs_table.s"
.include "./src/kernel/irq_table.s"
.include "./src/kernel/idt/idt.s"
.include "./src/kernel/memory/paging.s"


.global _start
.type _start, @function
_start:
	/*Setup the stack pointer to point to the beginning of our stack */
	/* I believe its a high address growing down to lower adress for the stack on x86*/
	mov $stack_top, %esp

	/*Reset EFLAGS*/
	pushl $0 
	popf 

	/* push the pointer to the Multiboot information structure*/
	pushl %ebx

	/* push the magic value */
	pushl %eax
	
	call early_main


	mov %cr0, %eax 
	or $1, %eax
	mov %eax, %cr0

	
	call kernel_main


	cli
1:	hlt
	jmp 1b


.size _start, . - _start


