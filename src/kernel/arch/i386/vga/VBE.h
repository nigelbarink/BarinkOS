
#define VBE_DISPI_IOPORT_INDEX 0x01CE
#define VBE_DISPI_IOPORT_DATA 0x01CF

/* VBE index values*/
#define VBE_DISPI_INDEX_ID              0x0
#define VBE_DISPI_INDEX_XRES            0x1
#define VBE_DISPI_INDEX_YRES            0x2
#define VBE_DISPI_INDEX_BPP             0x3
#define VBE_DISPI_INDEX_ENABLE          0x4
#define VBE_DISPI_INDEX_BANK            0x5
#define VBE_DISPI_INDEX_VIRT_WIDTH      0x6
#define VBE_DISPI_INDEX_VIRT_HEIGHT     0x7
#define VBE_DISPI_INDEX_X_OFFSET        0x8
#define VBE_DISPI_INDEX_Y_OFFSET        0x9

/* BGA Version */
#define VBE_DISPI_ID5   0xB0C5
#define VBE_DISPI_ID4   0xB0C3 
#define VBE_DISPI_ID3   0xB0C2
#define VBE_DISPI_ID2   0xB0C1
#define VBE_DISPI_ID1   0xB0C0


/* BGA BIT DEPTH */
#define VBE_DISPI_BPP_4 0x04
#define VBE_DISPI_BPP_8 0x08
#define VBE_DISPI_BPP_15 0x0F
#define VBE_DISPI_BPP_16 0x10
#define VBE_DISPI_BPP_24 0x18
#define VBE_DISPI_BPP_32 0x20


    /*unsigned short BGAReadRegister(unsigned short IndexValue){
        // outpw(VBE_DISPI_IOPORT_INDEX, IndexValue);
       // return inpw (VBE_DISPI_IOPORT_DATA);
    }

    int BGAIsAvailable (){
        return (BGAReadRegister(VBE_DISPI_INDEX_ID) == VBE_DISPI_ID5);
    }*/
