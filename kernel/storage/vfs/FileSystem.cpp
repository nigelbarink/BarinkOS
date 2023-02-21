//
// Created by nigel on 21/02/23.
//
#include "FileSystem.h"

void FileSystem::WriteFile(int file, unsigned char* buffer, unsigned int length) {

}

void FileSystem::ReadFile(int file, unsigned char* buffer, unsigned int length) {

}

FILE FileSystem::OpenFile(const char* fname){
    if(fname){
        unsigned char device = 'a';
        char* filename = (char*) fname;

        if(fname[1]== ':'){
            device = fname[0];
            filename += 2; // strip the volume component from the path
        }

        if(_filesystems[device - 'a']){
            FILE file = _filesystems[device-'a']->Open(filename);
            file.device = device;
            return file;
        }
    }
    FILE file;
    file.flags = FS_INVALID;
    return file;
}

void FileSystem::CloseFile(PFILE file) {
    if(file->device < DEVICE_MAX){
        // _filesystems[file->device]->Close(file);
    }
}


