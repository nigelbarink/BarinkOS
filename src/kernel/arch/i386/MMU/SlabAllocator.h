#pragma once
/**
 *  We'll need something to this effect to allocate memory in the kernel
 *  this will hopefully someday implement a full slab allocator 
 **/
enum SlabState {
    empty,
    partial,
    full 
};

class CacheSlab {
    const int SlabSize = 4000;
    void* start = 0x0;
};


class Allocator {

    public:
        Allocator();
        ~Allocator();

        void* kmalloc( int size );
        void kfree (void* address);

    private:
        CacheSlab** _cache;




};