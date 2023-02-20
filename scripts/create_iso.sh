#!/bin/bash
mkdir -p root/boot/grub
cp build/kernel/myos.bin root/boot/myos.bin
cp kernel/grub.cfg root/boot/grub/grub.cfg
grub-mkrescue -o barinkOS.iso root