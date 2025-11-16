#include "rtl8139.h"
#include "../io/io.h"


rtl8139::rtl8139(){

}

rtl8139::~rtl8139(){

}

void rtl8139::Wake(){
    outb(ioaddr + CONFIG_1, 0x0);
}

void rtl8139::Reset(){
    outb(ioaddr + CMD_OFFSET, 0x10);
    // Wait for the reset procedure
    while((inb(ioaddr + CMD_OFFSET) & 0x10) != 0) continue;
}


void rtl8139::Sleep(){
    
}

void rtl8139::Receive(){
    
}


void rtl8139::Transmit(){
    
}