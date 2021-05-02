#!/bin/bash

# Environment setup
export PATH="$HOME/opt/cross/bin:$PATH"


## Setup directory variables
SRC_DIR=src
BUILD_DIR=build


# clean old build 
rm build/*

# Execute build
i686-elf-as $SRC_DIR/boot.s -o $BUILD_DIR/boot.o
i686-elf-gcc -c $SRC_DIR/kernel.c -o $BUILD_DIR/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -T $SRC_DIR/linker.ld -o $BUILD_DIR/myos.bin -ffreestanding -O2 -nostdlib $BUILD_DIR/boot.o $BUILD_DIR/kernel.o -lgcc


## How to run build
## Use the command: qemu-system-i386 -kernel build/myos.bin