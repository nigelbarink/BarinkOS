#pragma once

class File {

public:
    virtual const File* Open () const ; // TODO: figure out a proper return value 
    virtual const char* Read() const;
    virtual void Write();
};
