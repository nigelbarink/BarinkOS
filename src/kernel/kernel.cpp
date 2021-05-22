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

SerialHub* serial;
extern "C" {
    void kernel_main (void) {
        /** initialize terminal interface */ 
        kterm_init();

        /** Wrtite stuff to the screen to test the terminal**/ 
        kterm_writestring("Hello world!\n");
        kterm_writestring("We got newline support!\n");

        /** Test scrolling **/
        for(int i=0; i < 5; i++){
            delay(500);
            kterm_writestring("We have implemented terminal scrolling!\n");
        }
    
        /** Test objective cpp **/
        kterm_writestring("Testing C++ object support\n");
        auto testObject = Test();
        testObject.printMe();

        /** Setup the MMU **/
        kterm_writestring("Starting MMU...\n");
        auto mmu = MMU();
        mmu.enable();
        kterm_writestring("MMU enabled!\n");

        /** Lets start using the serial port for debugging .. **/
        // Hopefully once we go into realmode or do something that
        // cause the screen to go black.. this serial comms part will give
        // some situational awareness
        serial = new SerialHub();

        serial->print("Hello world!");

    
    }   
}
