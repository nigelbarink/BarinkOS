#pragma once 
#include "rsdp.h"
class ACPI {
    public:
        static void initialize();
        
        // In the future ACPI might start
        // doing more systems initialization

    static RSDPDescriptor* rsd_ptr;
    static RSCPDescriptor20* rsd2_ptr;
    static RSDT* rsd_table;
    private:
};