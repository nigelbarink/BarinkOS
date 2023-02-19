#pragma once
#include <stddef.h>

class  String {
public:
    String(char* characters);
    String(String&) = default;
    unsigned int length();

    char* str ();
    char operator[](size_t index) ;
    const char operator[](size_t idx) const;

protected:
    char* chars;


};