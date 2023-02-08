#pragma once 
#include "rsdp.h"
class ACPI {
    public:
        static void initialize();
        
        // In the future ACPI might start
        // doing more systems initialization
        
    private:
       static RSDPTR* rsd_ptr;
       static RSDT* rsd_table;
};