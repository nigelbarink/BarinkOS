//
// Created by nigel on 19/02/23.
//
#pragma once
#include <stddef.h>
#include "StringView.h"


class Path{
public:
    explicit Path(String path);

    StringView getbasename();

    char* str();

private:
    String path;

};


