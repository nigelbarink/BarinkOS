#include "pit.h"
#include "../../tty/kterm.h"
uint32_t pit_tick = 0;


void pit_initialise()
{
    asm volatile("CLI");

#ifdef __VERBOSE__
    printf("Init PIT!\n");
#endif
    // clear mask for IRQ 0 
    uint8_t value = inb(0x21) & ~(1<< 0);
    outb(0x21, value);

    io_wait();

    const int freq = 500;

    uint32_t divisor = 1193180 / freq;

    outb(PIT_COMMAND, 0x36);

    uint8_t l = (uint8_t) (divisor & 0xFF);
    uint8_t h = (uint8_t) ( (divisor>>8) & 0xff);

    outb(PIT_DATA_0, l);
    outb(PIT_DATA_0,h);


    asm volatile("STI");
}


void get_pit_count()
{
    asm volatile ("CLI");

    outb(PIT_COMMAND, 0);
    uint16_t count = inb(PIT_DATA_0);
    count |= inb(PIT_DATA_0) << 8;

    printf("PIT count: 0x%x\n", count); 

    asm volatile("STI");   

}

void set_pit_count()
{

}

