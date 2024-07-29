#include <stdlib.h>
#include <string.h>
#include "OJPAL.h"




void* pal_memcpy(void *dest, const void * src, unsigned int destSize, unsigned int count)
{
    unsigned int r_cnt = (count <= destSize) ? (count) : (destSize);
    return memcpy(dest, src, r_cnt);
}