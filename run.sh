#!/bin/bash
PROC=$$

# Build the Corelib static library
(cd CoreLib
if ! make 2> warnings.log 1> /dev/null ; then
    echo "Build Corelib failed!"
    kill -10 $PROC
fi)

# Build the kernel image
(cd kernel
# make clean
if ! make 2> warnings.log 1> /dev/null ; then
  echo "Build kernel failed!"
  kill -10 $PROC
fi)

./scripts/update_harddrive.sh
./scripts/create_symbol_lookup.sh

args="";
if [[ $1 == "-d" ]]
then
  args="debug"
fi
./scripts/run_qemu.sh $args
