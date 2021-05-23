#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../../../libc/include/string.h"
#include "../tty/kterm.h"
#include "../../../io.h"

class SerialHub {

    public: 
        
        SerialHub();

        ~SerialHub();


        void print(const char* message);
        void println(const char* message);

        inline void setPort (const int* port ){PORT = *port;}

    private: 
        const int COM1 = 0x3F8;
        const int COM2 = 0x2F8;
        const int COM3 = 0x3E8;
        const int COM4 = 0x2E8;
        
        int PORT;

        int init(int);
        int is_transmit_empty();
        void write_character(char a);
        int received();
        char read();
        void test();


};