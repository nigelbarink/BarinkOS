# Writing an Operating system 
## As a learning experience!
 Inspired by people like Linus Torvalds and Andreas Kling

![Logo](images/BarinkOS.png)

________________________
### Screenshot(s)
![Scrolling the terminal](screenshots/Screenshot1.png)   \
The first scrolling boot screen. 😲


![Interrupt handeling](screenshots/WIP_interruptHandling.png)   \
W.I.P - Working on interrupt handling


![Multiboot integration](screenshots/multiboot.png) \
Multiboot information can be read by the kernel.

![PCI enumeration](screenshots/PCIBusEnumeration.png) \
Enumerating the PCI bus 

![ATAPI CD-ROM Identification](screenshots/CD-ROM_Identify.png) \
Correctly identified our ATAPI device 🎉

![Reading Files from FAT-16](screenshots/ReadingFilesFromFAT16.png) \
Reading a file from a FAT-16 Formatted drive 

________________________

### The goal
Writing a hobby operating system to better understand the basic building blocks of any operating system.Initially I'd like for my 
operating system to be able to run bash.

________________________
### Operating System Technical specs/details
The operating system can print strings to the 
screen. The terminal/screen has scrolling so the latest messages are visible on the screen.

________________________
### Planning
[See TODO](todo.md) \
[See Features](features.md)
________________________
### Docs
[Intro](docs/Intro.md) \
[Manuals](docs/Manuals.md) \
[Project structure](docs/ProjectStructure.md) 
________________________
### Resources:

#### General kernel stuff
[wiki.osdev.org/Main_Page](wiki.osdev.org/Main_Page)

[Modern Operating Systems [book]](https://www.amazon.com/Modern-Operating-Systems-Tanenbaum-Andrew/dp/1292061421/ref=sr_1_1?__mk_nl_NL=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=Modern+Operating+systems&qid=1619967779&sr=8-1)

[whiteheadsoftware.dev](https://whiteheadsoftware.dev/operating-systems-development-for-dummies/)

#### More specific stuff
[VFS explained: science.unitn.it](https://www.science.unitn.it/~fiorella/guidelinux/tlk/node102.html)


