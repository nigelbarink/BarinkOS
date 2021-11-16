/* Tell processor to use our gdt*/
gdt: 
    .word (gdt_end - gdt_start -1) # Size of the GDT in bytes minus 1 for math reasons
    .int gdt_start # linear address of our GDT

	
.att_syntax

.global load_gdt
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
