#include "VFS.h"
/*
 * TODO: Implement this!!
 * 
 */



void VirtualFileSystem::Initialize(FS* root)
{
    root = root;
}

void VirtualFileSystem::Open(const char* path)
{
    /*
        What does this mean?
        1. Parse the path string 
        2. Traverse the graph (Finding the correct Node)
        3. Create some kind of open file pointer thingy
    */
}

void VirtualFileSystem::Read()
{
    // NOTE: we need some way to know what file we wish to read from 
}

void VirtualFileSystem::Write()
{
   // NOTE: we need some way to know what file we wish to write to 
}

void VirtualFileSystem::Mount(const char* path, FS* FileSystem)
{
    /*
        What does this mean?
        1. Parse the path string
        2. Add a node to our internal graph 
    */
}

void VirtualFileSystem::UnMount(FS* FileSystem)
{
 /*
        What does this mean?
        1. Parse the path string
        2. Remve a node to our internal graph 
    */
}