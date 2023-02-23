#!/bin/bash
PROC=$$

# Build the Corelib static library
(cd CoreLib
if ! make; then
    echo "Build failed!"
    kill -10 $PROC
fi)

# Build the kernel image
(cd kernel
make clean
make
if ! make; then
  echo "Build failed!"
  kill -10 $PROC
fi)

./scripts/update_harddrive.sh



./scripts/run_qemu.sh

