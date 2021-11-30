#pragma once 

struct IDEChannelRegisters{
    unsigned short base; // I/O Base.
    unsigned short ctrl; // Control Base
    unsigned short bmide; // Bus Master IDE
    unsigned char nIEN; // IEN (no interrupt)
};


struct IDE_DEVICE {
    unsigned char Reserved; // 0 (Empty) or 1 (This device exists).
    unsigned char Channel; // 0 (Primary Channel) or 1 (Secondary Channel).
    unsigned char Drive; // 0 (Master Drive) or 1 (Slave Drive).
    unsigned short Type; // 0 ATA, 1:ATAPI
    unsigned short Signature; // Drive Signature
    unsigned short Capabilities; // Features.
    unsigned int CommandSets; // Command Sets Supported.
    unsigned int Size; // Size in Sectors (NOTE: Seems unused nowadays as i've only seen the value be zero
    unsigned char Model[41]; // Model in string.
} ;



extern IDEChannelRegisters channels[2];
extern IDE_DEVICE ide_devices[4];
extern unsigned char ide_buf[2048];
extern unsigned  char ide_irq_invoked;
extern unsigned  char atapi_packet[12];
