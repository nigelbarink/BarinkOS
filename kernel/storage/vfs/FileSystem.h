//
// Created by nigel on 21/02/23.
//
#pragma once
#include "StorageTypes.h"

class FileSystem {
public:
    static void WriteFile(PFILE file, unsigned char* beffer, unsigned int length);
    static void ReadFile(PFILE file, unsigned char* buffer, unsigned int length);
    static FILE OpenFile(const char* fname);
    static void CloseFile(PFILE file);


};



