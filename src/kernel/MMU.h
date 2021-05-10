#pragma once 
#include <stdint.h>

extern "C" void loadPageDirectory (long unsigned int* addr );
extern "C" void enablePaging();

class MMU {
    public:
    void enable ();

    private:
    uint32_t page_directory[1024] __attribute__((aligned(4096)));
    uint32_t first_page_table[1024] __attribute__((aligned(4096)));
};