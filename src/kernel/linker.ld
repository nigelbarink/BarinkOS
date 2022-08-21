ENTRY(_start)
 
/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	. = 0x00100000;  /* place code at 1MB mark*/
 
	_kernel_start	= .;
	kernel_begin	= .; /*  For legacy reasons */


	.multiboot.data : {
		*(.multiboot.data)
	}

	.multiboot.text : {
		*(multiboot.text)
		*launcher.o(.text)
	}

	. += 0xC0000000; /* Addresses in the following code need to be above the 3Gb mark */


	.text ALIGN (4K) : AT (ADDR (.text) - 0xC0000000)
	{
		*(.text)
	}
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xC0000000)
	{
		*(.rodata)
	}
	.data ALIGN (4K) : AT (ADDR (.data) - 0xC0000000)
	{
		*(.data)
	}
	.bss ALIGN (4K) : AT (ADDR (.bss) - 0xC0000000)
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}
	_kernel_end = .;
	kernel_end = .; /* For legacy reasons */
}

