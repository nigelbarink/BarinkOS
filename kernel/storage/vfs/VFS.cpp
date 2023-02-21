#include "VFS.h"
#include <stdint-gcc.h>
#include "../../memory/KernelHeap.h"
#include <CoreLib/Memory.h>

PFS VirtualFileSystem::_filesystems[VirtualFileSystem::DEVICE_MAX];

void VirtualFileSystem::initialize()
{

}

void VirtualFileSystem::ResolvePath(Path &path)
{
    // See reference material (1) https://man7.org/linux/man-pages/man7/path_resolution.7.html

    char* string_path  = path.str();
    void* cpy = string_path;

    bool isAbsolutePath = string_path[0] == '/';
    if(isAbsolutePath)
    {
        // strip the first slash
        string_path++;
    }

    char* tokstate = NULL;
    char* nextdir = strtok(string_path, "/", &tokstate);
    while (nextdir)
    {
        printf("First entry to look for: %s\n", nextdir);
        nextdir = strtok(NULL, "/", &tokstate);
    }

    free(cpy);
}

void VirtualFileSystem::Mount(PFS filesystemDescriptor, unsigned int DeviceID)
{
    if(DeviceID < DEVICE_MAX)
        if(filesystemDescriptor)
            _filesystems[DeviceID] = filesystemDescriptor;
}


void VirtualFileSystem::Unmount(unsigned int DeviceID) {
    if(DeviceID < DEVICE_MAX)
        _filesystems[DeviceID] = nullptr;
}





