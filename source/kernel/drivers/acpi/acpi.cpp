#include "acpi.h"
RSDPTR* ACPI::rsd_ptr;
RSDT* ACPI::rsd_table;



void ACPI::initialize(){

    // Find the Root System Description Pointer
    ACPI::rsd_ptr = FindRSD();
    printRSD(rsd_ptr);
    //  Get the Root System Description Table
    ACPI::rsd_table = getRSDT(rsd_ptr);
}