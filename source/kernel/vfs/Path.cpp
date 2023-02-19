//
// Created by nigel on 19/02/23.
//
#include "Path.h"
#include "../memory/KernelHeap.h"
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

StringView::StringView(String &string, unsigned int start, unsigned int end)
: String(string), begin(start), end(end)
{

}

char* StringView::str(){
    char* str = (char*) malloc((this->length() * sizeof(char))+1);

    int index = 0;
    for ( int i = begin; i < end ; i++){
        str[index] = chars[i];
        index++;
    }
    chars[index+1] ='\0';
    return str;
}


Path::Path(char *path)
: path(String(path))
{

}

Path::Path(String &path)
: path(path)
{

}

StringView Path::getbasename()
{
    unsigned int path_length = path.length();
    int i = path_length;
    while (path[i] != '/')
        i--;

    return StringView(path,i +1, path_length);
}

char* Path::str() {
    return path.str();
}