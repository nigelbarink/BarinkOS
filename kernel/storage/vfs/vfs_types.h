//
// Created by nigel on 21/02/23.
//
#pragma once
// grasslab.github.io/osdi/en/labs/lab7.html
#include <stddef.h>
#include <stdint.h>
#include "../../../CoreLib/List.h"

struct inode_operations;
struct vfsmount;
struct FS_SUPER;
struct inode;
struct dentry_operations;
struct DirectoryNode;
struct filesystem;
struct FS_SUPER;
struct FILE;


// Describes a mount
struct vfsmount {
    vfsmount* mnt_parent; // fs we are mounted on
    DirectoryNode* mountpoint; // dentry of mount point
    DirectoryNode* root; // root of the mounted tree
    FS_SUPER* sb; // pointer to the superblock
    unsigned int  mnt_count; // keep track of users of this structure
    int mnt_flags;
    char* mnt_devname; // name of device eg /dev/dsk/hda1

};
struct FS_SUPER;
// Represents a filesystem object (i.e. a file or directory or device)
struct inode {
   unsigned long mode; // access permissions
   unsigned long uid; // user id owner
   unsigned long gid; // group id owner
   unsigned int flags;
 // operations possible on inode
    int (*create)(inode*, DirectoryNode*, const char* );
    DirectoryNode* (*lookup)(inode* , DirectoryNode*);
    int (*link)(DirectoryNode*, inode* , DirectoryNode*);
    int (*unlink)(inode*, DirectoryNode*);
    int (*symlink)(inode*, DirectoryNode*);
    int (*mkdir)(inode*, DirectoryNode*, const char*);
    int (*rmdir)(inode*, DirectoryNode*);
    int (*rename)(inode*, DirectoryNode*, inode*, DirectoryNode*);
    void (*truncate)(inode*);
    int (*permission)(inode*, int);
    int (*setattr)(DirectoryNode, unsigned long*);
    int (*getattr)(vfsmount* mnt, DirectoryNode*, unsigned long*);

    FS_SUPER* sb;
    unsigned  long ino; // unique number of this inode
    unsigned int links; // number of hard links
    void* device;
    unsigned long size; // size of inode contents in bytes
    unsigned long atime; //  Access time
    unsigned long mtime; // Modify time
    unsigned long ctime; // Creation time
    unsigned short bytes; // bytes consumed
    // File operations possible on Inode;
    int(*write) (FILE* file, const void* buf, unsigned int len);
    int(*read) (FILE* file, void* buf, unsigned int len);
    void* internal; // point  to underlying representation of this virtual node (e.g. FAT entry or Directory Entry)
};

// Represents the name of files in the filesystem
// it identifies the file that an inode represents
struct DirectoryNode {
    char* name; // name of the file on the disk
    DirectoryNode* parent; // parent of the file
    inode* node; // node belongs to...
    int (*compare)(DirectoryNode*, char*, char* );
    int (*o_delete)(DirectoryNode*);
    void (*release)(DirectoryNode*);
    void (*iput)(DirectoryNode*, inode*);
    List* children;
};
// Represents a filesystem type
struct filesystem {
    const char* name;
    FS_SUPER* (*mount)(filesystem* self, const char* name, vfsmount* mnt);
};

// Represents a mounted filesystem
struct FS_SUPER {

    void* device; // device associated with the filesystem
    unsigned long blocksize;
    unsigned long maxbytes;
    filesystem* type;
    unsigned long magic; // IDK
    DirectoryNode* root; // Root dentry
    int count; // IDK
    void* fs_info; // pointer to raw filesystem info
    dentry_operations* d_op;
    inode* inodes[];
};

// Represents an opened file
struct FILE {
    inode* root;
    unsigned int f_pos; // The next read/write position of this file descriptor;
    int(*write) (FILE* file, const void* buf, unsigned int len);
    int(*read) (FILE* file, void* buf, unsigned int len);
    int flags;
};


