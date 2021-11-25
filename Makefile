
EMULATOR = qemu-system-i386
AS = ${HOME}/opt/cross/bin/i686-elf-as
CC = ${HOME}/opt/cross/bin/i686-elf-gcc
CPP = ${HOME}/opt/cross/bin/i686-elf-g++ 
CFLAGS =  -ffreestanding -O2 -Wall -Wextra

OFILES =  $(BUILD_DIR)/boot.o $(BUILD_DIR)/kterm.o $(BUILD_DIR)/kernel.o  $(BUILD_DIR)/PhysicalMemoryManager.o $(BUILD_DIR)/io.o $(BUILD_DIR)/PageDirectory.o $(BUILD_DIR)/gdtc.o $(BUILD_DIR)/idt.o $(BUILD_DIR)/pci.o $(BUILD_DIR)/pic.o $(BUILD_DIR)/string.o

SRC_DIR = src
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
	cp src/grub.cfg root/boot/grub/grub.cfg
	grub-mkrescue -o build/barinkOS.iso root
	
test:
	$(EMULATOR)  -kernel $(BUILD_DIR)/myos.bin -serial stdio -vga std -display gtk -m 2G -cpu core2duo 

build_kernel: $(OBJ_LINK_LIST)
	$(CC) -T $(SRC_DIR)/kernel//linker.ld -o $(BUILD_DIR)/myos.bin \
	 -ffreestanding -O2 -nostdlib $(OBJ_LINK_LIST) -lgcc

build_x86_64: 
	$(AS) $(SRC_DIR)/cgc/x86_64/crti.s -o $(BUILD_DIR)/crti_64.o
	$(AS) $(SRC_DIR)/cgc/x86_64/crtn.s -o $(BUILD_DIR)/crtn.o

clean: 
	rm -f $(BUILD_DIR)/myos.bin $(INTERNAL_OBJS)

$(BUILD_DIR)/kernel.o:
	$(CPP) -c $(SRC_DIR)/kernel/kernel.cpp -o $(BUILD_DIR)/kernel.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/kterm.o:
	$(CPP) -c $(SRC_DIR)/kernel/tty/kterm.cpp  -o $(BUILD_DIR)/kterm.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/boot.o:
	$(AS) $(SRC_DIR)/kernel//boot.S -o $(BUILD_DIR)/boot.o

$(BUILD_DIR)/crti.o:
	$(AS) $(SRC_DIR)/kernel/crti.s -o $(BUILD_DIR)/crti.o

$(BUILD_DIR)/crtn.o:
	$(AS) $(SRC_DIR)/kernel/crtn.s -o $(BUILD_DIR)/crtn.o

$(BUILD_DIR)/io.o:
		$(CPP) -c $(SRC_DIR)/kernel/io.cpp  -o $(BUILD_DIR)/io.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/PageDirectory.o:
	$(CPP) -c $(SRC_DIR)/kernel/memory/PageDirectory.cpp -o $(BUILD_DIR)/PageDirectory.o $(CFLAGS) -fno-exceptions -fno-rtti 

$(BUILD_DIR)/idt.o:
	$(CPP) -c $(SRC_DIR)/kernel/idt/idt.cpp -o $(BUILD_DIR)/idt.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/gdtc.o:
	$(CPP) -c $(SRC_DIR)/kernel/gdt/gdtc.cpp -o $(BUILD_DIR)/gdtc.o $(CFLAGS) -fno-exceptions -fno-rtti


$(BUILD_DIR)/pic.o:
	$(CPP) -c $(SRC_DIR)/kernel/pic/pic.cpp -o $(BUILD_DIR)/pic.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/string.o:
	$(CC) -c $(SRC_DIR)/libc/include/string.c  -o $(BUILD_DIR)/string.o $(CFLAGS) -std=gnu99

$(BUILD_DIR)/PhysicalMemoryManager.o:
	$(CPP) -c $(SRC_DIR)/kernel/memory/PhysicalMemoryManager.cpp  -o $(BUILD_DIR)/PhysicalMemoryManager.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/pci.o:
	$(CPP) -c $(SRC_DIR)/kernel/pci.cpp  -o $(BUILD_DIR)/pci.o $(CFLAGS) -fno-exceptions -fno-rtti
