inline void* memset (void* ptr, int value, size_t num){
    for( int i = 0; i < num; i++ )
    {
        unsigned char* data  = (unsigned char*)ptr+ i;
        *data = (unsigned char)value;
    }
    return ptr;
}
