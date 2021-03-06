
EMULATOR = qemu-system-i386
AS = ${HOME}/opt/cross/bin/i686-elf-as
CC = ${HOME}/opt/cross/bin/i686-elf-gcc
CPP = ${HOME}/opt/cross/bin/i686-elf-g++ 
CFLAGS =  -ffreestanding -O2 -Wall -Wextra

OFILES =	$(BUILD_DIR)/boot.o $(BUILD_DIR)/kterm.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/io.o $(BUILD_DIR)/PageDirectory.o $(BUILD_DIR)/idt.o $(BUILD_DIR)/pic.o $(BUILD_DIR)/string.o

SRC_DIR = src
BUILD_DIR = build

CRTBEGIN_OBJ = $(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ = $(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)

CRTI_OBJ = $(BUILD_DIR)/crti.o
CRTN_OBJ = $(BUILD_DIR)/crtn.o
OBJ_LINK_LIST = $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OFILES) $(CRTEND_OBJ) $(CRTN_OBJ)  
INTERNAL_OBJS = $(CRTI_OBJ) $(OFILES) $(CRTN_OBJ)


all: clean build clean_up

build: build_kernel 



clean_iso: 
	if [[ -a isodir/* ]] ; then rm isodir/* -d ; fi
	if [ -f barinkOS.iso ] ; then rm barinkOS.iso ; fi
	
iso: clean_iso build
	mkdir -p isodir/boot/grub
	cp build/myos.bin isodir/boot/myos.bin
	cp src/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o barinkOS.iso isodir
	
clean_up:
	rm build/*.o

test:
	$(EMULATOR)  -kernel $(BUILD_DIR)/myos.bin -serial file:CON -vga std -monitor stdio -display gtk -m 2G -cpu core2duo 

build_kernel: $(OBJ_LINK_LIST)
	
	$(CC) -T $(SRC_DIR)/kernel/arch/i386/linker.ld -o $(BUILD_DIR)/myos.bin \
	 -ffreestanding -O2 -nostdlib $(OBJ_LINK_LIST) -lgcc

build_x86_64: 
	$(AS) $(SRC_DIR)/cgc/x86_64/crti.s -o $(BUILD_DIR)/crti_64.o
	$(AS) $(SRC_DIR)/cgc/x86_64/crtn.s -o $(BUILD_DIR)/crtn.o

clean: 
	rm -f $(BUILD_DIR)/myos.bin $(INTERNAL_OBJS)

$(BUILD_DIR)/kernel.o:
	$(CPP) -c $(SRC_DIR)/kernel/kernel.cpp -o $(BUILD_DIR)/kernel.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/kterm.o:
	$(CC) -c $(SRC_DIR)/kernel/arch/i386/tty/kterm.c  -o $(BUILD_DIR)/kterm.o $(CFLAGS) -std=gnu99

$(BUILD_DIR)/boot.o:
	$(AS) $(SRC_DIR)/kernel/arch/i386/boot.s -o $(BUILD_DIR)/boot.o

$(BUILD_DIR)/crti.o:
	$(AS) $(SRC_DIR)/kernel/arch/i386/crti.s -o $(BUILD_DIR)/crti.o

$(BUILD_DIR)/crtn.o:
	$(AS) $(SRC_DIR)/kernel/arch/i386/crtn.s -o $(BUILD_DIR)/crtn.o

$(BUILD_DIR)/io.o:
		$(CPP) -c $(SRC_DIR)/kernel/io.cpp  -o $(BUILD_DIR)/io.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/PageDirectory.o:
	$(CPP) -c $(SRC_DIR)/kernel/memory/PageDirectory.cpp -o $(BUILD_DIR)/PageDirectory.o $(CFLAGS) -fno-exceptions -fno-rtti 

$(BUILD_DIR)/idt.o:
	$(CPP) -c $(SRC_DIR)/kernel/arch/i386/idt/idt.cpp -o $(BUILD_DIR)/idt.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/pic.o:
	$(CPP) -c $(SRC_DIR)/kernel/arch/i386/pic/pic.cpp -o $(BUILD_DIR)/pic.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/string.o:
	$(CC) -c $(SRC_DIR)/libc/include/string.c  -o $(BUILD_DIR)/string.o $(CFLAGS) -std=gnu99
