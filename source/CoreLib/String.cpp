#include "String.h"
#include <stdint.h>
#include <stddef.h>


String::String(char* characters)
: chars(characters)
        {

        }

char* String::str(){
    return chars;
}

unsigned int String::length ()
{
    int i = 0;

    while (  chars[i] != '\0'){
        i++;
    }

    return i;
}

// Returns a null character if size exceeds limits
char String::operator[] (size_t idx)
{
    if( idx  > this->length())
        return '\0';

    return chars[idx];
}

const char String::operator[](size_t idx) const {
return (const char) chars[idx];
}

