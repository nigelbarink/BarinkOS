#pragma once 
#include <stddef.h>
#include <stdint.h>


inline void bitmap_set( uint32_t* map , int index )
{
    map[index/32] |= (1 << (index % 32));
}

inline void bitmap_unset(uint32_t* map , int index)
{
    map[index/32] &= ~(1 << (index % 32));
}

inline int bitmap_first_unset( uint32_t* map , int size)
{
    uint32_t rem_bits = size % 32;
    for(uint32_t i = 0; i < size/32; i++)
    {
        if(map[i] != 0xFFFFFFFF){
            for(int j = 0; j < 32; j++){
                if(!(map[i] & (1<< j))){
                    return (i*32) + j;
                }
            }
        }
    }

    if(rem_bits){
        for(uint32_t j = 0; j < rem_bits; j++){
            if(!(map[size/32] & (1 << j ))){
                return size + j; // Original author divided size by 32 and then multiplied it by 32 which is a net zero calculation ?!?
            }
        }
    }

    return -1;
    
}
