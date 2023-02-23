//
// Created by nigel on 21/02/23.
//
#pragma once
// grasslab.github.io/osdi/en/labs/lab7.html
#include <stddef.h>
#include <stdint.h>

struct inode_operations;
struct vfsmount;
struct superblock;
struct inode;
struct dentry_operations;
struct directoryEntry;
struct filesystem;
struct superblock;
struct file;

struct file_operations{
    int(*write) (file* file, const void* buf, size_t len);
    int(*read) (file* file, void* buf, size_t len);
};
struct inode_operations {
    int (*create)(inode*, directoryEntry*, const char* );
    directoryEntry* (*lookup)(inode* , directoryEntry*);
    int (*link)(directoryEntry*, inode* , directoryEntry*);
    int (*unlink)(inode*, directoryEntry*);
    int (*symlink)(inode*, directoryEntry*);
    int (*mkdir)(inode*, directoryEntry*, const char*);
    int (*rmdir)(inode*, directoryEntry*);
    int (*rename)(inode*, directoryEntry*, inode*, directoryEntry*);
    void (*truncate)(inode*);
    int (*permission)(inode*, int);
    int (*setattr)(directoryEntry, unsigned long*);
    int (*getattr)(vfsmount* mnt, directoryEntry*, unsigned long*);
};
// Describes a mount
struct vfsmount {
    vfsmount* mnt_parent; // fs we are mounted on
    directoryEntry* mountpoint; // dentry of mount point
    directoryEntry* root; // root of the mounted tree
    superblock* sb; // pointer to the superblock
    unsigned int  mnt_count; // keep track of users of this structure
    int mnt_flags;
    char* mnt_devname; // name of device eg /dev/dsk/hda1

};
struct superblock;
// Represents a filesystem object (i.e. a file or directory or device)
struct inode {
   unsigned long mode; // access permissions
   unsigned long uid; // user id owner
   unsigned long gid; // group id owner
   unsigned int flags;
   inode_operations* i_op; // operations possible on inode
   superblock* sb;
    unsigned  long ino; // unique number of this inode
    unsigned int links; // number of hard links
    void* device;
    unsigned long size; // size of inode contents in bytes
    unsigned long atime; //  Access time
    unsigned long mtime; // Modify time
    unsigned long ctime; // Creation time
    unsigned short bytes; // bytes consumed
    file_operations* fop;
    void* internal; // point  to underlying representation of this virtual node (e.g. FAT entry or Directory Entry)
};
// Represents the possible operations on a directory entry
struct dentry_operations
{
    int (*compare)(directoryEntry*, char*, char* );
    int (*o_delete)(directoryEntry*);
    void (*release)(directoryEntry*);
    void (*iput)(directoryEntry*, inode* );

};
// Represents the name of files in the filesystem
// it identifies the file that an inode represents
struct directoryEntry {
    char* name; // name of the file on the disk
    directoryEntry* parent; // parent of the file
    inode* node; // node belongs to...
    dentry_operations* op;
    directoryEntry* children[];

};
// Represents a filesystem type
struct filesystem {
    const char* name;
    superblock* (*mount)(filesystem* self, const char* name, vfsmount* mnt);
};

// Represents a mounted filesystem
struct superblock {

    void* device; // device associated with the filesystem
    unsigned long blocksize;
    unsigned long maxbytes;
    filesystem* type;
    unsigned long magic; // IDK
    directoryEntry* root; // Root dentry
    int count; // IDK
    void* fs_info; // pointer to raw filesystem info
    dentry_operations* d_op;
    inode* inodes[];
};

// Represents an opened file
struct file {
    inode* root;
    size_t f_pos; // The next read/write position of this file descriptor;
    file_operations* f_ops;
    int flags;
};


