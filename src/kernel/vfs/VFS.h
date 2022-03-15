#pragma once

class VirtualFileSystem{
public: 
    void Initialize( FS* root);
    void Open (const char* path);
    void Read();
    void Write();

    void Mount(const char* path,FS* FileSystem);
    void UnMount(FS* FileSystem);

private:
    FS* root;


};

struct FS
{
    const char* name ;
    int DeviceID;
    int ManufacturerID;
    FS* next;
    char**(Read)();
    void*(Write)();
    void*(Open)();
};

