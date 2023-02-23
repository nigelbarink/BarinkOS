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

     static struct file* open(const char* pathname, int flags);
     static int close(struct file* file);
     static int write(struct file* file, const void* buf, size_t len);
     static int read(struct file* file, void* buf, size_t len);

 private:
    static vfsmount* rootfs;


    static int mount_number;
    static int superblock_number;
    static int filesystem_number;

    static filesystem* filesystems[];
    static superblock* superblocks[];
    static vfsmount* mounts[];

 };

