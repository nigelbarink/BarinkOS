.global LoadGlobalDescriptorTable

	LoadGlobalDescriptorTable:
		lgdt gdtDescriptor

		movw $16, %ax 
		movw %ax, %ds
		movw %ax, %es
		movw %ax, %fs
		movw %ax, %gs
		movw %ax, %ss
		
		jmp $8,$flush 

		flush: 
		ret
		
	

