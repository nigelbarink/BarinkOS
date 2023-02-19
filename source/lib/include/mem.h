#pragma once
// NOTE: These should not be inline 
inline void* memset (void* ptr, int value, size_t num)
{
    for( int i = 0; i < num; i++ )
    {
        unsigned char* data  = (unsigned char*)ptr+ i;
        *data = (unsigned char)value;
    }
    return ptr;
}



inline int memcmp( const void* ptr1, const void* ptr2, size_t num)
{
    const unsigned char * cs = (const unsigned char*) ptr1;
    const unsigned char * ct = (const unsigned char*) ptr2;


    for (int i = 0 ; i < num ; i++, cs++, ct++ ){
       if( *cs < *ct){
           return -1;
       } else if( *cs > *ct){
           return 1;
       }
    }

    return 0;

}