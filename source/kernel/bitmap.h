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

inline uint32_t bitmap_first_unset( uint32_t* map , int map_size)
{
    for ( int i = 0 ; i < map_size ; i ++ )
    {
        // a bit or more is set within this byte!
        if( (map[i] & 0xFFFFFFFF) > 0 ){

            // which bit is set? 
            for(int j = 0 ; j < 32 ; j++){
                if ( (map[i] & (0x00000001 << j)) > 0)
                {
                    return (i*32)+j;
                }
            }

        
        }


    }

    return -1;
    
}
