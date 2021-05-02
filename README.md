# Writing an Operating system 
## As a learning experience... Inspired by people like.. Linus Torvalds and Andreas Kling


### The goal
Writing a hobby operating system to better understand the basic building blocks of any operating system.



### Operating System Technical specs/details
Currently the operating system is in the planning fase. 
I hope to soon have the basic output and booting sequence with multiboot done.


### Planning

[ ] Muliboot to kernel \
[ ] Printing strings and integer numbers (both decimal and hex) on the screen is certainly a must. This is one of most basic ways of debugging, and virtually all of us have gone through a kprint() or kout in version 0.01. \
[ ] Outputting to a serial port will save you a lot of debugging time. You don't have to fear losing information due to scrolling. You will be able to test your OS from a console, filter interesting debug messages, and automatize some tests. \
[ ] Having a working and reliable interrupt/exception handling system that can dump the contents of the registers (and perhaps the address of the fault) will be very useful. \
[ ] Plan your memory map (virtual, and physical) : decide where you want the data to be. \
[ ] The heap: allocating memory at runtime (malloc and free) is almost impossible to go without. It should be implemented as soon as possible.


### Other features I am thinking of:
[ ] USTAR Filesystem ( For its simplicity this is very likely the first filesystem the OS is going to support) \
[ ] Memory Management \
[ ] Scheduling (Unknown what the scheduling algorithm will be, as with everything suspect simplicity) \
[ ] RPC - for interprocess communication \
[ ] Sync primitives  - Semaphores, Mutexes, spinlocks et al. \
[ ] ACPI support ( Or some other basic way to support shutdown, reboot and possibly hibernation ) \
[ ] ATA support \
[ ] Keyboard support ( must have ) \
[ ] Basic hardware recognition ( CPU codename, memory, ATA harddisk, RAW diskSpace, CPU speed et al. ) \
[ ] Basic Terminal \
[ ] Simplistic draw ( maybe ?!?) \
### Far in the future: \
[ ] Basic Window server/client 
#### Support for more filesystems if I like the challenge in writing these ...
[ ] FAT Filesystem
[ ] EXT2 Filesystem



### Resources:

[wiki.osdev.org/Main_Page](wiki.osdev.org/Main_Page)

[Modern Operating Systems [book]](https://www.amazon.com/Modern-Operating-Systems-Tanenbaum-Andrew/dp/1292061421/ref=sr_1_1?__mk_nl_NL=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=Modern+Operating+systems&qid=1619967779&sr=8-1)
