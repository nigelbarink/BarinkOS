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
uint32_t inl( int port  );
unsigned int inl_p(unsigned short port);

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}
void outb_p(unsigned char value, unsigned short port);
void outw(unsigned short value, unsigned short port);
void outw_p(unsigned short value, unsigned short port);
void outl( int port , uint32_t data );
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
void io_wait();