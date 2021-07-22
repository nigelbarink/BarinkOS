#include "kernel.h"
/**
 *      simple delay function 
 **/
void delay(int t){
    volatile int i,j;
    for(i=0;i<t;i++)
        for(j=0;j<25000;j++)
            asm("NOP");
}

class Test {
    public:
         Test();
        void printMe();
         ~Test();
};

Test::Test(){
    kterm_writestring("Create a test object\n");
};

void Test::printMe(){
    kterm_writestring("testObject.printMe()\n");
}

Test::~Test(){
    kterm_writestring("Destroy testObject! Bye bye\n");
}

#define PORT 0x3f8 
static int init_serial() {
   outb(PORT + 1, 0x00);    // Disable all interrupts
   outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(PORT + 1, 0x00);    //                  (hi byte)
   outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   outb(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
   outb(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
   if(inb(PORT + 0) != 0xAE) {
      return 1;
   }
 
   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   outb(PORT + 4, 0x0F);
   return 0;
}

int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}
 
void write_serial(char a) {
   while (is_transmit_empty() == 0);
 
   outb(PORT,a);
}

int serial_received() {
   return inb(PORT + 5) & 1;
}
 
char read_serial() {
   while (serial_received() == 0);
 
   return inb(PORT);
}

void print_serial(const char* string ){
    for(size_t i = 0; i < strlen(string); i ++){
        write_serial(string[i]);
    }
}

void test_serial(){
        /** Serial test **/
        kterm_writestring("Writing to COM1 serial port:");
        init_serial();
        write_serial('A');
        write_serial('B');
        write_serial('C');
        write_serial('D');
        write_serial('E');

        char Character_received = read_serial();
        kterm_writestring("\n");
        kterm_writestring("received from COM 1: \n");
        kterm_put(Character_received);

        kterm_writestring("\n");
}

extern "C" {

    void early_main(unsigned long magic, unsigned long addr){

        /** initialize terminal interface */ 
        kterm_init();
      
        multiboot_info_t *mbi;

        if (magic != MULTIBOOT_BOOTLOADER_MAGIC){
            printf("Invalid magic number: 0x%x\n", (unsigned) magic);
            return;
        }

        /* Set MBI to the addresss of the multiboot information structure*/
        mbi = (multiboot_info_t *) addr;

        /* Print out the flags */
        printf("flags = 0x%x\n", (unsigned) mbi->flags);

        /* Are mem_* valid? */
        if ( CHECK_FLAG(mbi->flags,0)){
            printf("mem_lower = %uKB, mem_upper = %uKB\n");
        }

        /* is boot device valid ? */
        if (CHECK_FLAG (mbi->flags, 1)){
            printf("boot_device = 0x0%x\n", (unsigned) mbi->boot_device);
        }

        /* is the command line passed? */
        if (CHECK_FLAG ( mbi->flags,2)){
            printf("cmdline = %s\n", (char *) mbi->cmdline);
        }

        /* Are mods_* valid? */
        if(CHECK_FLAG ( mbi->flags, 3)){
            multiboot_module_t *mod;
            int i;

            printf("mods count = %d, mods_addr = 0x%x\n", (int) mbi->mods_count, (int) mbi->mods_addr);

            for(i = 0, mod = (multiboot_module_t *) mbi->mods_addr; i < mbi->mods_count; i++ , mod++){
                printf(" mod start = 0x%x, mod_end = 0x%x, cmdline = %s\n", (unsigned) mod->mod_start, (unsigned) mod->mod_end, (char*) mod->cmdline);
            }
        }

        /* Bits 4 and 5 are mutually exclusive! */
        if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG(mbi->flags, 5)){
            printf("Both bits 4 and 5 are set.\n");
            return;
        }

        /* Is the symbol table of a.out valid? */
        if (CHECK_FLAG(mbi->flags, 4)){
            multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);

            printf( "multiboot_aout_symbol_table: tabsize = 0x%0x, strsize = 0x%x, addr = 0x%x\n", 
            (unsigned) multiboot_aout_sym->tabsize,
            (unsigned) multiboot_aout_sym->strsize,
            (unsigned) multiboot_aout_sym->addr);

        }

        /* Is the section header table of ELF valid? */
        if (CHECK_FLAG(mbi->flags, 5)){
            multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);

            printf("multiboot_elf_sec: num = %u, size = 0x%x, addr = 0x%x, shnd = 0x%x\n",
            (unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
            (unsigned) multiboot_elf_sec->addr, (unsigned)  multiboot_elf_sec->shndx);
            
        }


        // AAAAAH memory map, Yes please!
        /* Are mmap_* valid? */
        if (CHECK_FLAG(mbi->flags, 6)){
            multiboot_memory_map_t *mmap;
            
            printf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
            (unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
            
            for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr; 
                (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length; 
                mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size))){
                    
                    printf(
                        "size = 0x%x, base_addr = 0x%x%08x, length = 0x%x%08x, type = 0x%x\n",
                        (unsigned) mmap->size,
                        (unsigned) (mmap->addr >> 32),
                        (unsigned) (mmap->addr & 0xffffffff),
                        (unsigned) (mmap->len >> 32),
                        (unsigned) (mmap->len & 0xffffffff),
                        (unsigned) mmap->type);
                
                }

        }

        /* Draw diagonal blue line */
        if (CHECK_FLAG (mbi->flags, 12)){
            printf("Can draw!");
        } 
    }

    void kernel_main (void) {


        init_serial();


        /** Setup the MMU **/
        //kterm_writestring("Starting MMU...\n");
        //auto mmu = MMU();
        //mmu.enable();
        //kterm_writestring("MMU enabled!\n");

        

       
       
    
 

        /** test interrupt handlers **/
        //asm volatile ("int $0x03");

        //asm volatile ("int $0x04");

        while (true){
            //Read time indefinetely 
            read_rtc();
            printf( "UTC time: %02d-%02d-%02d %02d:%02d:%02d  [ Formatted as YY-MM-DD h:mm:ss]\r" ,year, month, day, hour, minute, second);
            delay(1000);
        }

        
        /** Lets start using the serial port for debugging .. **/
        // Hopefully once we go into realmode or do something that
        // cause the screen to go black.. this serial comms part will give
        // some situational awareness
        //Serial serialbus = Serial::init();


    
    }   
}
