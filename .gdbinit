target remote localhost:1234

file root/boot/myos.bin
symbol-file kernel.sym

break prekernel/prekernel.cpp:18
continue