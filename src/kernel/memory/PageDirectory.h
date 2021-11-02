#pragma once 
#include <stdint.h>

extern "C" void loadPageDirectory (uint32_t* addr );
extern "C" void enablePaging();
typedef uintptr_t address_t;


#define KB 1024

static const int MAX_PAGES =  1024 * KB; // 4GB , 4kB/page
static volatile address_t pmem_stack[MAX_PAGES];
static volatile address_t pmem_stack_top = MAX_PAGES; // top down allocation


struct page_directory_entry {};
struct page_table_entry{};



class PageDirectory {
    public:
    void enable ();
    
    private:
    uint32_t page_directory[1024] __attribute__((aligned(4096)));
    uint32_t first_page_table[1024] __attribute__((aligned(4096)));
};