#include "io.h"

unsigned char inb_p(unsigned short ){

}
unsigned short inw(unsigned short ){

}
unsigned short inw_p(unsigned short ){

}
unsigned int inl(unsigned short ){

}
unsigned int inl_p(unsigned short ){

}


void outb_p(unsigned char , unsigned short ){

}
void outw(unsigned short , unsigned short ){

}
void outw_p(unsigned short , unsigned short ){

}
void outl(unsigned int , unsigned short ){

}
void outl_p(unsigned int , unsigned short ){

}

void insb(unsigned short , void *,
        unsigned long ){

        }
void insw(unsigned short , void *,
        unsigned long ){

        }
void insl(unsigned short , void *,
        unsigned long ){

        }
void outsb(unsigned short , const void *,
        unsigned long ){

        }
void outsw(unsigned short , const void *,
        unsigned long ){

        }
void outsl(unsigned short , const void *,
        unsigned long ){

        }

void io_wait(void)
{
    /* TODO: This is probably fragile. */
    asm volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}