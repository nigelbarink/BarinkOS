#!/bin/bash
echo "creating symbols file"
echo $(pwd)
objcopy --only-keep-debug root/boot/myos.bin kernel.sym