# NOTE: I wish this wasn't AT&T Syntax its horrible
# REMINDER: INSTRUCTION FROM_REGISTER, TO_REGISTER
.globl enablePaging
enablePaging:
	# Create a new call frame 
	push %ebp
	mov %esp, %ebp

	# Set the PG bit of CR0
	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0

	# Restore to the previous call frame 
	mov %ebp, %esp
	pop %ebp
	ret

.globl loadPageDirectory
loadPageDirectory:
	push %ebp
	mov %esp, %ebp

	/* NOTE: We should probably check if paging is already enabled.
		Changing the CR3 register whilst paging is enabled might 
		result in unwanted behaviour (in the worst case) or cause a 
		fault (in the best case).
	*/

	mov 8(%esp), %eax # Move the first argument in the eax register

	mov %eax, %cr3  # Move the value of eax into the CR3 register

	/*
		Moving the value of the argument  passed to this function 
		into the CR3 register will allow the MMU to access the paging 
		structure we setup in memory once we enable paging
	*/

	mov %ebp, %esp
	pop %ebp
	ret


