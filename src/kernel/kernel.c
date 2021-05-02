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

void kernel_main (void) {
    /** initialize terminal interface */ 
    kterm_init();

    /** Wrtite stuff to the screen to test the terminal**/ 
    kterm_writestring("Hello world!\n");
    kterm_writestring("We got newline support!\n");

    for(;;){
        delay(500);
        kterm_writestring("We have implemented terminal scrolling!\n");
    }
   
   
}   

