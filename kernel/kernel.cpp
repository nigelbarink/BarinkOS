/*
    Copyright © Nigel Barink 2023
*/
#include "acpi/acpi.h"
#include "drivers/pit/pit.h"
#include "drivers/vga/VBE.h"
#include "i386/processor.h"
#include "interrupts/idt.h"
#include "klog.h"
#include "memory/KernelHeap.h"
#include "memory/TaskStateSegment.h"
#include "memory/VirtualMemoryManager.h"
#include "memory/gdt/gdtc.h"
#include "memory/memory.h"
#include "pci/pci.h"
#include "storage/filesystems/FAT/FAT.h"
#include "storage/vfs/vfs.h"
#include "supervisorterminal/superVisorTerminal.h"
#include "terminal/kterm.h"

extern BootInfoBlock *BIB;
extern "C" void LoadGlobalDescriptorTable();
extern "C" void jump_usermode();

void initBootDrive() {
  printf("Boot device: 0x%x\n", BIB->bootDeviceID);
  unsigned int part3 = BIB->bootDeviceID & 0xFF;
  unsigned int part2 = (BIB->bootDeviceID & 0xFF00) >> 8;
  unsigned int part1 = (BIB->bootDeviceID & 0xFF0000) >> 16;
  unsigned int drive = (BIB->bootDeviceID & 0xFF000000) >> 24;
  if (drive == 0x80)
    printf("booted from disk!\n");
  if (drive == 0x00)
    printf("booted from floppy disk\n");

  printf("Part1: %d, Part2: %d, Part3: %d\n", part1, part2, part3);
  ATAPIO::Identify(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER);
}
extern uint32_t *boot_page_directory;
extern "C" void kernel() {

  while (true) {
    __asm__ __volatile__("NOP");
  };
  // kterm_init();

  uint32_t cr3;
  __asm__ __volatile__("mov %%cr3, %%eax\n\t"
                       "mov %%eax, %0\n\t"
                       : "=m"(cr3)
                       :
                       : "%eax");

  printf("value of cr3: 0x%x\n", cr3);
  // printf("page directory entry : 0x%x\n" , boot_page_directory[1023]);
  while (true)
    ;

  setup_tss();
  initGDT();
  initidt();
  LoadGlobalDescriptorTable();
  flush_tss();

  print_info("Memory setup complete!\n");
  // Enable interrupts
  asm volatile("STI");

  initHeap();
  pit_initialise();

  PCI::Scan();
  processor::initialize();
  processor::enable_protectedMode();
  initBootDrive();
  VirtualFileSystem::initialize();

  print_dbg("Hello debug!\n");
  print_info("Hello info!\n");
  print_err("Hello error!\n");

  ACPI::initialize();

#define VFS_EXAMPLE
#ifdef VFS_EXAMPLE
  auto fontFile = VirtualFileSystem::open("/FONT    PSF", 0);
  printf("Size of font file: %d bytes\n",
         fontFile->root->size); // COOL This Works like a charm

#endif

#ifdef USERMODE_RELEASE
  // Lets jump into user mode
  jump_usermode();
#else
  startSuperVisorTerminal();

#endif
}
