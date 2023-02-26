#pragma once
#include <stdint-gcc.h>
#include "../../../CoreLib/Path.h"
#include "vfs_types.h"
#include "vfs_types.h"
 class VirtualFileSystem
 {
 public:
     static void initialize();
     static void Mount(filesystem* fs, const char* name);

     static int register_filesystem(struct filesystem* fs);

     static FILE* open(const char* pathname, int flags);
     static int close(struct FILE* file);
     static int write(struct FILE* file, const void* buf, unsigned int len);
     static int read(struct FILE* file, void* buf, unsigned int len);

 private:
    static vfsmount* rootfs;


    static int mount_number;
    static int superblock_number;
    static int filesystem_number;

    static filesystem* filesystems[];
    static FS_SUPER* superblocks[];
    static vfsmount* mounts[];

 };

