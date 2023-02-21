AS = ${HOME}/opt/cross/bin/i686-elf-as
CC = ${HOME}/opt/cross/bin/i686-elf-gcc
CPP = ${HOME}/opt/cross/bin/i686-elf-g++ 
CFLAGS =  -ffreestanding -Og -ggdb  -Wall -Wextra  -I ../build/CoreLib/include
BUILD_DIR = ../build/kernel
OBJ_DIR = ../bin/kernel

CRTBEGIN_OBJ = $(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ = $(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)

CRTI_OBJ = $(OBJ_DIR)/crti.o
CRTN_OBJ = $(OBJ_DIR)/crtn.o

OFILES = $(OBJ_DIR)/boot.o	\
 		 $(OBJ_DIR)/kterm.o \
 		 $(OBJ_DIR)/kernel.o \
 		 $(OBJ_DIR)/memory.o \
 		 $(OBJ_DIR)/paging.o \
 		 $(OBJ_DIR)/VFS.o \
 		 $(OBJ_DIR)/pit.o \
 		 $(OBJ_DIR)/time.o \
 		 $(OBJ_DIR)/keyboard.o \
 		 $(OBJ_DIR)/io.o \
 		 $(OBJ_DIR)/processor.o \
		 $(OBJ_DIR)/gdtc.o \
		 $(OBJ_DIR)/idt.o \
		 $(OBJ_DIR)/pic.o \
		 $(OBJ_DIR)/sv-terminal.o \
		 $(OBJ_DIR)/prekernel.o \
		 $(OBJ_DIR)/KHeap.o \
		 $(OBJ_DIR)/pci.o \
		 $(OBJ_DIR)/pcidevice.o \
		 $(OBJ_DIR)/atapiDevice.o \
		 $(OBJ_DIR)/ataDevice.o \
		 $(OBJ_DIR)/rsdp.o \
		 $(OBJ_DIR)/acpi.o \
		 $(OBJ_DIR)/fat.o

OBJ_LINK_LIST = $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OFILES) $(CRTEND_OBJ) $(CRTN_OBJ)  
INTERNAL_OBJS = $(CRTI_OBJ) $(OFILES) $(CRTN_OBJ)

all:  build

clean:
	rm $(OBJ_DIR)/* -r


build: $(OBJ_LINK_LIST)
	$(CPP) -T linker.ld -o $(BUILD_DIR)/myos.bin -ffreestanding -ggdb -Og -nostdlib $(OBJ_LINK_LIST) -lgcc -L ../build/CoreLib -lCoreLib

# C++ definition -> Object files
$(OBJ_DIR)/kernel.o:
	$(CPP) -c kernel.cpp -o $(OBJ_DIR)/kernel.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/kterm.o:
	$(CPP) -c terminal/kterm.cpp  -o $(OBJ_DIR)/kterm.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/io.o:
	$(CPP) -c io/io.cpp  -o $(OBJ_DIR)/io.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/idt.o:
	$(CPP) -c interrupts/idt.cpp -o $(OBJ_DIR)/idt.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/gdtc.o:
	$(CPP) -c memory/gdt/gdtc.cpp -o $(OBJ_DIR)/gdtc.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/pic.o:
	$(CPP) -c drivers/pic/pic.cpp -o $(OBJ_DIR)/pic.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/PhysicalMemoryManager.o:
	$(CPP) -c memory/PhysicalMemoryManager.cpp  -o $(OBJ_DIR)/PhysicalMemoryManager.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/pci.o:
	$(CPP) -c pci/pci.cpp  -o $(OBJ_DIR)/pci.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/pcidevice.o:
	$(CPP) -c pci/pciDevice.cpp  -o $(OBJ_DIR)/pcidevice.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/atapiDevice.o:
	$(CPP) -c storage/atapi/atapiDevice.cpp  -o $(OBJ_DIR)/atapiDevice.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/ataDevice.o:
	$(CPP) -c "storage/ata pio/ATAPIO.cpp"  -o $(OBJ_DIR)/ataDevice.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/rsdp.o:
	$(CPP) -c acpi/rsdp.cpp  -o $(OBJ_DIR)/rsdp.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/acpi.o:
	$(CPP) -c acpi/acpi.cpp -o $(OBJ_DIR)/acpi.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/pit.o:
	$(CPP) -c drivers/pit/pit.cpp  -o $(OBJ_DIR)/pit.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/VFS.o:
	$(CPP) -c storage/vfs/vfs.cpp  -o $(OBJ_DIR)/VFS.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/keyboard.o:
	$(CPP) -c drivers/ps-2/keyboard.cpp  -o $(OBJ_DIR)/keyboard.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/time.o:
	$(CPP) -c time.cpp  -o $(OBJ_DIR)/time.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/sv-terminal.o:
	$(CPP) -c supervisorterminal/superVisorTerminal.cpp  -o $(OBJ_DIR)/sv-terminal.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/memory.o:
	$(CPP) -c memory/PhysicalMemoryManager.cpp  -o $(OBJ_DIR)/memory.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/paging.o:
	$(CPP) -c memory/VirtualMemoryManager.cpp -o $(OBJ_DIR)/paging.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/KHeap.o:
	$(CPP) -c memory/KernelHeap.cpp -o $(OBJ_DIR)/KHeap.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/prekernel.o:
	$(CPP) -c prekernel/prekernel.cpp -o $(OBJ_DIR)/prekernel.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/processor.o:
	$(CPP) -c i386/processor.cpp -o $(OBJ_DIR)/processor.o $(CFLAGS) -fno-exceptions -fno-rtti

$(OBJ_DIR)/fat.o:
	$(CPP) -c storage/filesystems/FAT/FAT.cpp -o $(OBJ_DIR)/fat.o $(CFLAGS) -fno-exceptions -fno-rtti

# Assembly -> Object files
$(OBJ_DIR)/boot.o:
	$(AS) boot/boot.s -o $(OBJ_DIR)/boot.o

$(OBJ_DIR)/crti.o:
	$(AS) crti.s -o $(OBJ_DIR)/crti.o

$(OBJ_DIR)/crtn.o:
	$(AS) crtn.s -o $(OBJ_DIR)/crtn.o
