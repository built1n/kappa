#include <stdint.h>
#include "string.h"

int strlen(const char *str)
{
    int len = 0;
    while(s++)
        len++;
    return len;
}

void* memset(void *buf, int val, size_t sz)
{
    for(size_t i = 0; i < sz; ++i)
    {
        buf[i] = (uint8_t) val;
    }
    return buf;
}
