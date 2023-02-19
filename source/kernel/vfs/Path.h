//
// Created by nigel on 19/02/23.
//
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

class StringView : String {
public:
    StringView(String& string, unsigned int start, unsigned  int end );
    char* str ();
private:
    unsigned int begin;
    unsigned int end;
};

class Path{
public:
    Path(String& path);
    Path(char* path);

    StringView getbasename();

    char* str();

private:
    String path;

};


