//
// Created by nigel on 26/02/23.
//
#pragma once


#include "terminal/kterm.h"
#include "storage/vfs/vfs_types.h"


void sys_version (){
    printf("KERNEL VERSION v0.4\n");
}

void sys_open(){

}

void sys_read(FILE* file, char* data){
    file->read(file, data, 512);
}

void sys_write(FILE* path, const char* data, size_t size){

}

// NOTE: this should become our standard!
void syscall_handler(int syscall_no ,  uint32_t* args... ){
    switch(syscall_no){
        case 0x0:
            printf("test!\n");
            break;
        case 0x5:
            // SYS_OPEN
            // sys_open();
            break;
        case 0x10:
            // SYS_READ
            // sys_read((FILE*)args[1], (char*) args[2] );
            break;
        case 0x20:
            // SYS_WRITE
            //sys_write((FILE*)args[1], (const char*) args[2], (size_t)args[3]);
            break;
        case 0x666:
            // SYS_VERSION
            sys_version();
            break;
    }
}


