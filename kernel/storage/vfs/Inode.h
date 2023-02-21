//
// Created by nigel on 21/02/23.
//
#pragma once
enum struct NODE_TYPE {
    FILESYSTEM,
    FILE,
    DIRECTORY
};

enum struct PERMISSIONS {
    READ,
    WRITE,
    EXECUTE
};

struct Inode {
    NODE_TYPE type;
    PERMISSIONS permissions;
    Inode* Parent;
    Inode* sibling;
};


