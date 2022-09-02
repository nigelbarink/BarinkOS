.include "./source/kernel/boot/multiboot.s"
/*
*	Allocate initial stack
*/
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

/*
* Preallocate a couple pages to get us bootstrapped 
* Being carefull to not use any address the bootloader might 
* be using for its multiboot structures
*/
.section .bss, "aw", @nobits
	.align 4096
.globl boot_page_directory
boot_page_directory:
	.skip 4096
.globl boot_page_table
boot_page_table:
	.skip 4096
.globl multiboot_page_table
multiboot_page_table:
	.skip 4096
# More page tables may be required

# Entry point
.section .multiboot.text, "a"
.globl _start
.type _start, @function 
_start:

	# Get physical address of the boot_page_table
	movl $(boot_page_table - 0xC0000000), %edi
	# Map address 0
	movl $0, %esi
1:	
	cmpl $(_kernel_end - 0xC0000000), %esi
	jge 3f

	# Map physical address as "present and writable"
	movl %esi, %edx
	orl $0x003, %edx
	movl %edx, (%edi)

2: # Size of page is 4096 bytes
	addl $4096, %esi
	# Size of entries in boot_page_table is 4 bytes
	addl $4, %edi 
	# Loop to the next entry if we haven't finished.
	loop 1b

3:	# Map VGA video memory to 0xC03FF00 as "present, writable"
	movl $(0x000B8000 | 0x003), boot_page_table - 0xC0000000 + 1023 * 4

	# Map the page table to both virtual addresss 0x00000000 and 0xC0000000
	movl $(boot_page_table - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
	movl $(boot_page_table - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4

	# Set cr3 to the address of the boot_page_directory
	movl $(boot_page_directory - 0xC0000000), %ecx
	movl %ecx, %cr3

	# Enable paging and the write-protect bit
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0

	# Jump to higher half with an absolute jump
	lea 4f, %ecx
	jmp *%ecx

.section .text
4:
	# At this point, paging is fully set up and enabled
isPaging:
	/* push the pointer to the Multiboot information structure*/
	pushl %ebx

	/* push the magic value */
	pushl %eax
	call prekernelSetup

	# Unmap the identity mapping as it is now unnecessary  
	movl $0, boot_page_directory + 0
	
	# Reload cr3 to force tlb flush 
	movl %cr3, %ecx
	movl %ecx, %cr3

	/*Setup the stack pointer to point to the beginning of our stack */
	/* I believe its a high address growing down to lower adress for the stack on x86*/
	mov $stack_top, %esp

	/*Reset EFLAGS*/
	pushl $0 
	popf 


	call early_main

	cli
1:	hlt
	jmp 1b


.include "./source/kernel/memory/gdt/gdt.s"
.include "./source/kernel/irs_table.s"
.include "./source/kernel/irq_table.s"
.include "./source/kernel/interrupts/idt/idt.s"


