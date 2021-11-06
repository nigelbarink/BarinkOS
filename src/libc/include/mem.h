inline void* memset (void* ptr, int value, size_t num){
    for( int i = 0; i < num; i++ )
    {
        int* data  = (int*)ptr+ i;
        *data = value;
    }
    return ptr;
}
