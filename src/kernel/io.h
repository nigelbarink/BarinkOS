#pragma once
#include <stdint.h>

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}
unsigned char inb_p(unsigned short port);
unsigned short inw(unsigned short port);
unsigned short inw_p(unsigned short port);
unsigned int inl(unsigned short port);
unsigned int inl_p(unsigned short port);

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}
void outb_p(unsigned char value, unsigned short port);
void outw(unsigned short value, unsigned short port);
void outw_p(unsigned short value, unsigned short port);
void outl(unsigned int value, unsigned short port);
void outl_p(unsigned int value, unsigned short port);

void insb(unsigned short port, void *addr,
        unsigned long count);
void insw(unsigned short port, void *addr,
        unsigned long count);
void insl(unsigned short port, void *addr,
        unsigned long count);
void outsb(unsigned short port, const void *addr,
        unsigned long count);
void outsw(unsigned short port, const void *addr,
        unsigned long count);
void outsl(unsigned short port, const void *addr,
        unsigned long count);