//
// Created by nigel on 17/02/23.
//

#include "processor.h"


uint32_t processor::cap_page;
uint32_t processor::cap_page1;
uint32_t processor::cap_page7 ;

void processor::initialize()
{

    asm volatile ("movl $0x80000001, %%eax;"
                  "CPUID;"
                  "movl %%edx, %0"
                  :: "m"(cap_page));

    asm volatile ("movl $0x01, %%eax; "
                  "CPUID;"
                  "movl %%edx, %0"
                  :: "m"(cap_page1));

    asm volatile ("movl $0x07, %%eax;"
                  "movl $0x0, %%ecx;"
                  "CPUID;"
                  "movl %%edx, %0"
                  :: "m"(cap_page7));

}

bool processor::hasAMXExtension()
{
    return (cap_page7 & AMX_TYPE::AMX_BF16) || (cap_page7 & AMX_TYPE::AMX_TILE) || (cap_page7 & AMX_TYPE::AMX_INT8);
}


/*
 * PSE: page-size extensions for 32-bit paging.
 * If CPUID.01H:EDX.PSE [bit 3] = 1, CR4.PSE may be set to 1, enabling support for 4-MByte pages with 32-bit paging
 */
bool processor::has32bitPagingSupport() {
    // is the PSE bit set
    return cap_page1 & (0x1 << 3);
}

/*
 * PAE: physical-address extension.
 * If CPUID.01H:EDX.PAE [bit 6] = 1, CR4.PAE may be set to 1, enabling PAE paging (this setting is also required
 * for 4-level paging and 5-level paging).
 */
bool processor::hasPAEExtension(){
    return cap_page1 & (0x1 << 6);
}

/*
 * PGE: global-page support.
 * If CPUID.01H:EDX.PGE [bit 13] = 1, CR4.PGE may be set to 1, enabling the global-page feature (see Section
 * 4.10.2.4).
 */
bool processor::hasPageSupport(){
    return cap_page1 & (0x1 << 13);
}

/*
 * Page1GB: 1-GByte pages.
 * If CPUID.80000001H:EDX.Page1GB [bit 26] = 1, 1-GByte pages may be supported with 4-level paging and 5-
 * level paging (see Section 4.5).
 */
bool processor::gigabytePages() {
    return cap_page & (0x1 << 26);
}