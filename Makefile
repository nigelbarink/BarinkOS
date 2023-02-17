
EMULATOR = qemu-system-i386
AS = ${HOME}/opt/cross/bin/i686-elf-as
CC = ${HOME}/opt/cross/bin/i686-elf-gcc
CPP = ${HOME}/opt/cross/bin/i686-elf-g++ 
CFLAGS =  -ffreestanding -Og -ggdb  -Wall -Wextra

OFILES =$(BUILD_DIR)/boot.o $(BUILD_DIR)/kterm.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/memory.o  $(BUILD_DIR)/paging.o	$(BUILD_DIR)/VFS.o $(BUILD_DIR)/pit.o 	$(BUILD_DIR)/time.o	$(BUILD_DIR)/keyboard.o	 $(BUILD_DIR)/io.o 	$(BUILD_DIR)/processor.o $(BUILD_DIR)/gdtc.o $(BUILD_DIR)/idt.o $(BUILD_DIR)/pic.o $(BUILD_DIR)/sv-terminal.o $(BUILD_DIR)/string.o  $(BUILD_DIR)/prekernel.o $(BUILD_DIR)/cpu.o $(BUILD_DIR)/KHeap.o $(BUILD_DIR)/pci.o $(BUILD_DIR)/pcidevice.o $(BUILD_DIR)/atapiDevice.o $(BUILD_DIR)/ataDevice.o $(BUILD_DIR)/rsdp.o $(BUILD_DIR)/acpi.o

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
	virtualboxvm --startvm "BarinkOS_test"
debug: all
	objcopy --only-keep-debug build/myos.bin kernel.sym
	$(EMULATOR) -cdrom build/barinkOS.iso -serial stdio -vga std -display gtk -m 2G -cpu core2duo -s -d int -no-shutdown -no-reboot
test:
	$(EMULATOR)  -kernel $(BUILD_DIR)/myos.bin -serial stdio -vga std -display gtk -m 2G -cpu core2duo -d int -no-shutdown -no-reboot

test_iso: 
	$(EMULATOR)  -boot d -cdrom $(BUILD_DIR)/barinkOS.iso -serial stdio -vga std -display gtk -m 2G -cpu core2duo -d int -no-reboot -no-shutdown
test_disk: all
	sudo losetup /dev/loop9 build/disk.img
	sudo mount /dev/loop9 /mnt
	sudo cp build/myos.bin /mnt/boot/myos.bin
	sudo umount /mnt
	sudo losetup -d /dev/loop9

	$(EMULATOR) -boot d -drive format=raw,file=build/disk.img -serial stdio -vga std -display gtk -m 2G -cpu core2duo 


build_kernel: $(OBJ_LINK_LIST)
	$(CC) -T $(SRC_DIR)/kernel//linker.ld -o $(BUILD_DIR)/myos.bin \
	 -ffreestanding -ggdb -Og -nostdlib $(OBJ_LINK_LIST) -lgcc

build_x86_64: 
	$(AS) $(SRC_DIR)/cgc/x86_64/crti.s -o $(BUILD_DIR)/crti_64.o
	$(AS) $(SRC_DIR)/cgc/x86_64/crtn.s -o $(BUILD_DIR)/crtn.o

clean: 
	rm -f $(BUILD_DIR)/myos.bin $(INTERNAL_OBJS)

# C++ definition -> Object files
$(BUILD_DIR)/kernel.o:
	$(CPP) -c $(SRC_DIR)/kernel/kernel.cpp -o $(BUILD_DIR)/kernel.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/kterm.o:
	$(CPP) -c $(SRC_DIR)/kernel/terminal/kterm.cpp  -o $(BUILD_DIR)/kterm.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/io.o:
		$(CPP) -c $(SRC_DIR)/kernel/drivers/io/io.cpp  -o $(BUILD_DIR)/io.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/idt.o:
	$(CPP) -c $(SRC_DIR)/kernel/interrupts/idt.cpp -o $(BUILD_DIR)/idt.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/gdtc.o:
	$(CPP) -c $(SRC_DIR)/kernel/memory/gdt/gdtc.cpp -o $(BUILD_DIR)/gdtc.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/pic.o:
	$(CPP) -c $(SRC_DIR)/kernel/drivers/pic/pic.cpp -o $(BUILD_DIR)/pic.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/string.o:
	$(CC) -c $(SRC_DIR)/lib/include/string.c  -o $(BUILD_DIR)/string.o $(CFLAGS) -std=gnu99

$(BUILD_DIR)/PhysicalMemoryManager.o:
	$(CPP) -c $(SRC_DIR)/kernel/memory/PhysicalMemoryManager.cpp  -o $(BUILD_DIR)/PhysicalMemoryManager.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/pci.o:
	$(CPP) -c $(SRC_DIR)/kernel/drivers/pci/pci.cpp  -o $(BUILD_DIR)/pci.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/pcidevice.o:
	$(CPP) -c $(SRC_DIR)/kernel/drivers/pci/pciDevice.cpp  -o $(BUILD_DIR)/pcidevice.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/atapiDevice.o:
	$(CPP) -c $(SRC_DIR)/kernel/drivers/atapi/atapiDevice.cpp  -o $(BUILD_DIR)/atapiDevice.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/ataDevice.o:
	$(CPP) -c $(SRC_DIR)/kernel/drivers/ata/ataDevice.cpp  -o $(BUILD_DIR)/ataDevice.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/rsdp.o:
	$(CPP) -c $(SRC_DIR)/kernel/drivers/acpi/rsdp.cpp  -o $(BUILD_DIR)/rsdp.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/acpi.o:
	$(CPP) -c $(SRC_DIR)/kernel/drivers/acpi/acpi.cpp -o $(BUILD_DIR)/acpi.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/pit.o:
	$(CPP) -c $(SRC_DIR)/kernel/drivers/pit/pit.cpp  -o $(BUILD_DIR)/pit.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/VFS.o:
	$(CPP) -c $(SRC_DIR)/kernel/vfs/VFS.cpp  -o $(BUILD_DIR)/VFS.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/keyboard.o:
	$(CPP) -c $(SRC_DIR)/kernel/drivers/ps-2/keyboard.cpp  -o $(BUILD_DIR)/keyboard.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/time.o:
	$(CPP) -c $(SRC_DIR)/kernel/time.cpp  -o $(BUILD_DIR)/time.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/sv-terminal.o:
	$(CPP) -c $(SRC_DIR)/kernel/supervisorterminal/superVisorTerminal.cpp  -o $(BUILD_DIR)/sv-terminal.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/memory.o:
	$(CPP) -c $(SRC_DIR)/kernel/memory/PhysicalMemoryManager.cpp  -o $(BUILD_DIR)/memory.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/paging.o:
	$(CPP) -c $(SRC_DIR)/kernel/memory/VirtualMemoryManager.cpp -o $(BUILD_DIR)/paging.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/KHeap.o:
	$(CPP) -c $(SRC_DIR)/kernel/memory/KernelHeap.cpp -o $(BUILD_DIR)/KHeap.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/prekernel.o:
	$(CPP) -c $(SRC_DIR)/kernel/prekernel/prekernel.cpp -o $(BUILD_DIR)/prekernel.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/cpu.o:
	$(CPP) -c $(SRC_DIR)/kernel/cpu.cpp -o $(BUILD_DIR)/cpu.o $(CFLAGS) -fno-exceptions -fno-rtti

$(BUILD_DIR)/processor.o:
	$(CPP) -c $(SRC_DIR)/kernel/i386/processor.cpp -o $(BUILD_DIR)/processor.o $(CFLAGS) -fno-exceptions -fno-rtti


# Assembly -> Object files
$(BUILD_DIR)/boot.o:
	$(AS) $(SRC_DIR)/kernel/boot/boot.s -o $(BUILD_DIR)/boot.o

$(BUILD_DIR)/crti.o:
	$(AS) $(SRC_DIR)/kernel/crti.s -o $(BUILD_DIR)/crti.o

$(BUILD_DIR)/crtn.o:
	$(AS) $(SRC_DIR)/kernel/crtn.s -o $(BUILD_DIR)/crtn.o
