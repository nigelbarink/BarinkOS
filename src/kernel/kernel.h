#pragma once 
extern "C" {
    #include "arch/i386/tty/kterm.h"
}

#include "../libc/include/string.h"
#include "./bootloader/multiboot.h"
#include "bootcheck.h"
#include "arch/i386/idt/idt.h"

#include "io.h"
#include "time.h"
#include "cpu.h"
#include "arch/i386/vga/VBE.h"
#define CHECK_FLAG(flags, bit) ((flags) & (1 <<(bit)))



/* This needs to be moved! */
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
