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


extern "C" {
    void kernel_main (void) {
        /** initialize terminal interface */ 
        auto terminal = TextMode();

        /** Wrtite stuff to the screen to test the terminal**/ 
        terminal.println("Hello world!");
        terminal.println("We got newline support!");

        /** Test scrolling **/
        for(int i=0; i < 5; i++){
            delay(500);
            terminal.println("We have implemented terminal scrolling!");
        }
    
        /** Test objective cpp **/
        terminal.println("Testing C++ object support");
        auto testObject = Test();
        testObject.printMe();

        /** Setup the MMU **/
        //println("Starting MMU...");
        //auto mmu = MMU();
        //mmu.enable();
        //println("MMU enabled!");

        /** Lets start using the serial port for debugging .. **/
        // Hopefully once we go into realmode or do something that
        // cause the screen to go black.. this serial comms part will give
        // some situational awareness
        SerialHub serial =  SerialHub();

        serial.print("Hello world!");

    
    }   
}
