#pragma once

class Serial {

    public: 
        static Serial init();
        
        void print();

    private: 
        const int COM1 = 0x3F8;
        const int COM2 = 0x2F8;
        const int COM3 = 0x3E8;
        const int COM4 = 0x2E8;


        Serial();
        ~Serial();
};