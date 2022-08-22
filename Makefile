
EMULATOR = qemu-system-i386
AS = ${HOME}/opt/cross/bin/i686-elf-as
CC = ${HOME}/opt/cross/bin/i686-elf-gcc
CPP = ${HOME}/opt/cross/bin/i686-elf-g++ 
CFLAGS =  -ffreestanding -Og -ggdb  -Wall -Wextra

OFILES =$(BUILD_DIR)/boot.o $(BUILD_DIR)/kterm.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/memory.o  $(BUILD_DIR)/paging.o	$(BUILD_DIR)/pit.o 	$(BUILD_DIR)/time.o	$(BUILD_DIR)/keyboard.o	 $(BUILD_DIR)/io.o 	$(BUILD_DIR)/gdtc.o $(BUILD_DIR)/idt.o $(BUILD_DIR)/pic.o $(BUILD_DIR)/sv-terminal.o $(BUILD_DIR)/string.o  $(BUILD_DIR)/prekernel.o

SRC_DIR = source
BUILD_DIR = build

CRTBEGIN_OBJ = $(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ = $(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)

CRTI_OBJ = $(BUILD_DIR)/crti.o
CRTN_OBJ = $(BUILD_DIR)/crtn.o
OBJ_LINK_LIST = $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OFILES) $(CRTEND_OBJ) $(CRTN_OBJ)  
INTERNAL_OBJS = $(CRTI_OBJ) $(OFILES) $(CRTN_OBJ)


all: clean build 

build: build_kernel iso

clean_iso: 
	if [[ -a isodir/boot ]] ; then rm root/boot -rd ; fi
	if [ -f build/barinkOS.iso ] ; then rm build/barinkOS.iso ; fi
	
iso: clean_iso clean build
	mkdir -p root/boot/grub
	cp build/myos.bin root/boot/myos.bin
	cp source/grub.cfg root/boot/grub/grub.cfg
	grub-mkrescue -o build/barinkOS.iso root
	
run: all
	$(EMULATOR) -cdrom build/barinkOS.iso -serial stdio -vga std -display gtk -m 2G -cpu core2duo

debug: all
	objcopy --only-keep-debug build/myos.bin kernel.sym
	$(EMULATOR) -cdrom build/barinkOS.iso -serial stdio -vga std -display gtk -m 2G -cpu core2duo -s -d int

build_kernel: $(OBJ_LINK_LIST)
	$(CC) -T $(SRC_DIR)/kernel//linker.ld -o $(BUILD_DIR)/myos.bin \
	 -ffreestanding -ggdb -Og -nostdlib $(OBJ_LINK_LIST) -lgcc

build_x86_64: 
	$(AS) $(SRC_DIR)/cgc/x86_64/crti.s -o $(BUILD_DIR)/crti_64.o
	$(AS) $(SRC_DIR)/cgc/x86_64/crtn.s -o $(BUILD_DIR)/crtn.o

clean: 
	rm -f $(BUILD_DIR)/myos.bin $(INTERNAL_OBJS)

$(BUILD_DIR)/kernel.o:
	$(CPP) -c $(SRC_DIR)/kernel/kernel.cpp -o $(BUILD_DIR)/kernel.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/kterm.o:
	$(CPP) -c $(SRC_DIR)/kernel/Terminal/kterm.cpp  -o $(BUILD_DIR)/kterm.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/boot.o:
	$(AS) $(SRC_DIR)/kernel/Boot/boot.s -o $(BUILD_DIR)/boot.o

$(BUILD_DIR)/crti.o:
	$(AS) $(SRC_DIR)/kernel/crti.s -o $(BUILD_DIR)/crti.o

$(BUILD_DIR)/crtn.o:
	$(AS) $(SRC_DIR)/kernel/crtn.s -o $(BUILD_DIR)/crtn.o

$(BUILD_DIR)/io.o:
		$(CPP) -c $(SRC_DIR)/kernel/io.cpp  -o $(BUILD_DIR)/io.o $(CFLAGS) -fno-exceptions -fno-rtti


$(BUILD_DIR)/idt.o:
	$(CPP) -c $(SRC_DIR)/kernel/Interrupts/idt/idt.cpp -o $(BUILD_DIR)/idt.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/gdtc.o:
	$(CPP) -c $(SRC_DIR)/kernel/Memory/GDT/gdtc.cpp -o $(BUILD_DIR)/gdtc.o $(CFLAGS) -fno-exceptions -fno-rtti


$(BUILD_DIR)/pic.o:
	$(CPP) -c $(SRC_DIR)/kernel/Drivers/PIC/pic.cpp -o $(BUILD_DIR)/pic.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/string.o:
	$(CC) -c $(SRC_DIR)/kernel/Lib/string.c  -o $(BUILD_DIR)/string.o $(CFLAGS) -std=gnu99


$(BUILD_DIR)/pit.o:
	$(CPP) -c $(SRC_DIR)/kernel/Drivers/PIT/pit.cpp  -o $(BUILD_DIR)/pit.o $(CFLAGS) -fno-exceptions -fno-rtti


$(BUILD_DIR)/keyboard.o:
	$(CPP) -c $(SRC_DIR)/kernel/Drivers/PS-2/keyboard.cpp  -o $(BUILD_DIR)/keyboard.o $(CFLAGS) -fno-exceptions -fno-rtti


$(BUILD_DIR)/time.o:
	$(CPP) -c $(SRC_DIR)/kernel/time.cpp  -o $(BUILD_DIR)/time.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/sv-terminal.o:
	$(CPP) -c $(SRC_DIR)/kernel/SuperVisorTerminal/superVisorTerminal.cpp  -o $(BUILD_DIR)/sv-terminal.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/memory.o:
	$(CPP) -c $(SRC_DIR)/kernel/Memory/PhysicalMemoryManager.cpp  -o $(BUILD_DIR)/memory.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/paging.o:
	$(CPP) -c $(SRC_DIR)/kernel/Memory/VirtualMemoryManager.cpp -o $(BUILD_DIR)/paging.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/prekernel.o:
	$(CPP) -c $(SRC_DIR)/kernel/PreKernel/prekernel.cpp -o $(BUILD_DIR)/prekernel.o $(CFLAGS) -fno-exceptions -fno-rtti