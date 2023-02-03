#include "string.h"

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
