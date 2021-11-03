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
