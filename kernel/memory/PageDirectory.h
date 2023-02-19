#pragma once 
#include <stdint.h>
#include "./memory.h"
#include "./../terminal/kterm.h"
#define KB 1024


typedef uintptr_t address_t;

static const int MAX_PAGES =  1024 * KB; // 4GB , 4kB/page
static volatile address_t pmem_stack[MAX_PAGES];
static volatile address_t pmem_stack_top = MAX_PAGES; // top down allocation

extern "C" void loadPageDirectory (uint32_t* addr );
extern "C" void enablePaging();

struct page_directory_entry {};
struct page_table_entry{};



class PageDirectory {
    public:
    void enable ();
    void MapPhysicalToVirtualAddress ( address_t PAddress , address_t VAddress, uint32_t size  );

    private:
    uint32_t page_directory[1024] __attribute__((aligned(4096))); // align on 4 kiloByte pages
    uint32_t first_page_table[1024] __attribute__((aligned(4096))); // align on 4 kiloByte pages

};