#!/bin/bash


cd CoreLib
make
cd ../kernel
make clean
make
cd ..

./scripts/update_harddrive.sh
./scripts/run_qemu.sh
