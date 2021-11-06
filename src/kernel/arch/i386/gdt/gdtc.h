#include <stdint.h>
extern "C"{

typedef struct {
   uint16_t           lLimit;
   uint16_t           lBase;
   uint8_t            mBase;
   uint8_t            access;
   uint8_t            granularity;
   uint8_t            hBase;
} gdtEntry_t;

struct gdtSegmentPointer {
   uint16_t limit;
   uint32_t base;
};

extern gdtSegmentPointer gdtPointer;

extern void load_gdt();
void setupGdt();
}