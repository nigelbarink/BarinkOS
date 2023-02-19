#include "timer.h"

uint32_t tick = 0;


static void timer_callback (registers_t regs ){
    tick ++ ;
    kterm_writestring ("tick passed!");
}

void init_timer (uint32_t frequency){
    // register timer callback

    uint32_t divisor = 1193180 / frequency;

    // Send the commmand byte
    outb(0x43, 0x36);

    // Divisor has to be sent byt-wise , so will send lower then upper bytes
    uint8_t low = (uint8_t) (divisor & 0xFF);
    uint8_t high = (uint8_t) ((divisor >> 8) & 0xFF);

    outb(0x40, low);
    outb(0x40, high);


}