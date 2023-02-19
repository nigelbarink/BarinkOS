//
// Created by nigel on 19/02/23.
//
#include "Path.h"

Path::Path(String path)
: path(path)
{

}

StringView Path::getbasename()
{
    unsigned int path_length = path.length();
    int i = path_length;
    while (path[i] != '/')
        i--;

    return {path,static_cast<unsigned int>(i +1), path_length};
}

char* Path::str() {
    return path.str();
}