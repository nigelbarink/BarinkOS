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

[[maybe_unused]] void memcpy (void* dest, const void* src, size_t count ){
    for( int i = 0; i < count; i++){
        ((char *)dest)[i] = ((const char*)src)[i];
    }
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

char* strchr(const char* s , int c){
    while(*s) {
        if(*s == c) return const_cast<char*>(s);
        s++;
    }
    return NULL;
}
char* strtok(char* str, const char* delim , char**saveptr){
    char *begin;
    if(str) {
        begin = str;
    }
    else if (*saveptr) {
        begin = *saveptr;
    }
    else {
        return NULL;
    }

    while(strchr(delim, begin[0])) {
        begin++;
    }


    char *next = NULL;
    for(int i = 0; i < strlen(delim); i++) {
        char *temp = strchr(begin, delim[i]);
        if(temp < next || next == NULL) {
            next = temp;
        }
    }
    if(!next) {
        *saveptr = NULL;
        return begin;
    }
    *next = 0;
    *saveptr=next+1;
    return begin;
}