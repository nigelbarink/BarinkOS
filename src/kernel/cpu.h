#include <cpuid.h> // NOTE: Only available in GCC 
// NOT currently usefull!
/*    static int get_model(){
        int ebx, unused;
        __cpuid(0, unused, ebx, unused, unused);
        return ebx;
    }

    enum {
        CPUID_FEAT_EDX_APIC = 1 << 9
    };
    static int check_apic (){
        unsigned int eax, unused, edx;
        __get_cpuid(1, &eax, &unused, &unused, &edx);
        return  edx & CPUID_FEAT_EDX_APIC;
    }
*/