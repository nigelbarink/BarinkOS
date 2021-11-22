#pragma once

#include "tty/kterm.h"
#include "io.h"
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


void printf_serial ( const char *format, ...) {
   
    char **arg = (char **)&format;
    int c;
    char buf[20];

    arg++;
    
    while ((c = *format++) != 0){
        if( c != '%')
            write_serial(c);
        else{
            char *p, *p2;
            int pad0 = 0, pad = 0;

            c = *format++;
            if(c =='0'){
                pad0 = 1;
                c = *format++;
            }

            if ( c >= '0' && c <= '9'){
                pad = c - '0';
                c = *format++;
            }

            switch (c)
            {
            case 'd':
             
            case 'u':
            case 'x':
                itoa(buf, c, *((int *) arg++));

                p = buf;
                goto string;
                break;
            
            case 's':
                p = *arg++;
                if(!p)
                    p = "(null)";
                
                string: 
                    for (p2 = p; *p2; p2++);
                    for (; p2 < p + pad; p2++)
                        write_serial(pad0 ? '0': ' ');
                    while (*p)
                        write_serial(*p++);
                    break;


            default:
                write_serial(*((int *)arg++));
                break;
            }
        }
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
