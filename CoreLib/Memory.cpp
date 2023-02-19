//
// Created by nigel on 19/02/23.
//
#include "Memory.h"

void* memset (void* ptr, int value, size_t num)
{
    for( int i = 0; i < num; i++ )
    {
        unsigned char* data  = (unsigned char*)ptr+ i;
        *data = (unsigned char)value;
    }
    return ptr;
}


int memcmp( const void* ptr1, const void* ptr2, size_t num)
{
    const unsigned char * cs = (const unsigned char*) ptr1;
    const unsigned char * ct = (const unsigned char*) ptr2;


    for (int i = 0 ; i < num ; i++, cs++, ct++ ){
        if( *cs < *ct){
            return -1;
        } else if( *cs > *ct){
            return 1;
        }
    }

    return 0;

}

size_t strlen(const char* str) {
    size_t len = 0;
    while(str[len]){
        len++;
    }
    return len;
}

int strncmp ( const char* str1, const char* str2, size_t num ){
    for( int i = 0; i < num ; i++){

        if( str1[i] < str2[i]){
            return -1;
        }

        if( str1[i] > str2[i] ){
            return 1;
        }


    }

    return 0;
}