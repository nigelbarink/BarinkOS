
void PhysicalMemoryAllocatorTest()
{
    #ifdef UNIT_TESTS
        // Small test!
        void* block = allocate_block();
        void* block2 = allocate_block();
        printf("Allocated addresss 1: 0x%x 2: 0x%x\n", (uint32_t)block ,(uint32_t)block2);
        free_block(block);
        free_block(block2);
        void* block3 = allocate_block();
        printf("Allocated addresss 3: 0x%x\n", (uint32_t)block3);
        free_block(block3);
    #endif
}