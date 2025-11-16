#pragma once 
#define MAC05_OFFSET    0x00 // size 6
#define MAC07_OFFSET    0x08 // size 8
#define RBSTART_OFFSET  0x30 // size 4
#define CMD_OFFSET      0x37 // size 1
#define IMR_OFFSET      0x3C // size 2
#define ISR_OFFSET      0x3E // size 2
#define CONFIG_1 0x52

// Copyright  © Nigel Barink 2023
// Information source: https://wiki.osdev.org/RTL8139

// Get me some networking !!  XD
class NetworkDevice {
    protected:

};


class rtl8139 : public  NetworkDevice { 
    public:
        rtl8139();
        ~rtl8139();


        void Wake();
        void Reset();
        
        void Sleep();
        void Receive();
        void Transmit();
    private:
        int ioaddr ;

};