//
// Created by nigel on 19/02/23.
//
#pragma once
#include "String.h"

class StringView : String {
public:
    StringView(String string, unsigned int start, unsigned  int end );
    char* str ();
private:
    unsigned int begin;
    unsigned int end;
};
