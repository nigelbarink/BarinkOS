
EMULATOR = qemu-system-i386
AS = ${HOME}/opt/cross/bin/i686-elf-as
CC = ${HOME}/opt/cross/bin/i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra

OFILES =	$(BUILD_DIR)/boot.o $(BUILD_DIR)/kterm.o $(BUILD_DIR)/kernel.o 

SRC_DIR = src
BUILD_DIR = build

CRTBEGIN_OBJ = $(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ = $(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)

CRTI_OBJ = $(BUILD_DIR)/crti.o
CRTN_OBJ = $(BUILD_DIR)/crtn.o
OBJ_LINK_LIST = $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OFILES) $(CRTEND_OBJ) $(CRTN_OBJ)  
INTERNAL_OBJS = $(CRTI_OBJ) $(OFILES) $(CRTN_OBJ)


all: clean build

build: build_kernel run 

run:
	$(EMULATOR) -kernel $(BUILD_DIR)/myos.bin 
build_kernel: $(OBJ_LINK_LIST)
	
	$(CC) -T $(SRC_DIR)/kernel/arch/i386/linker.ld -o $(BUILD_DIR)/myos.bin \
	 -ffreestanding -O2 -nostdlib $(OBJ_LINK_LIST) -lgcc

build_x86_64: 
	$(AS) $(SRC_DIR)/cgc/x86_64/crti.s -o $(BUILD_DIR)/crti_64.o
	$(AS) $(SRC_DIR)/cgc/x86_64/crtn.s -o $(BUILD_DIR)/crtn.o

clean: 
	rm -f $(BUILD_DIR)/myos.bin $(INTERNAL_OBJS)

$(BUILD_DIR)/kernel.o:
	$(CC) -c $(SRC_DIR)/kernel/kernel.c  -o $(BUILD_DIR)/kernel.o $(CFLAGS)

$(BUILD_DIR)/kterm.o:
	$(CC) -c $(SRC_DIR)/kernel/arch/i386/tty/kterm.c  -o $(BUILD_DIR)/kterm.o $(CFLAGS)

$(BUILD_DIR)/boot.o:
	$(AS) $(SRC_DIR)/kernel/arch/i386/boot.s -o $(BUILD_DIR)/boot.o

$(BUILD_DIR)/crti.o:
	$(AS) $(SRC_DIR)/kernel/arch/i386/crti.s -o $(BUILD_DIR)/crti.o

$(BUILD_DIR)/crtn.o:
	$(AS) $(SRC_DIR)/kernel/arch/i386/crtn.s -o $(BUILD_DIR)/crtn.o
