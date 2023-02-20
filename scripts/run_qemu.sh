#!/bin/bash

# Run from harddisk
qemu-system-i386 -boot d -drive format=raw,file=disk.img -serial stdio -vga std -display gtk -m 2G -cpu core2duo

# Run disk version
# qemu-system-i386 -cdrom barinkOS.iso -serial stdio -vga std -display gtk -m 2G -cpu core2duo -s -d int -no-shutdown -no-reboot

# Run the raw kernel image
# qemu-system-i386  -kernel build/kernel/myos.bin -serial stdio -vga std -display gtk -m 2G -cpu core2duo -d int -no-shutdown -no-reboot


