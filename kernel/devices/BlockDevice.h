//
// Created by nigel on 21/02/23.
//
#pragma once

class BlockDevice {
    virtual char* Read()=0;
    virtual void Write() =0;
};


