#include <stdint.h>
#include <stddef.h>
#include "string.h"

int strlen(const char *str)
{
    int len = 0;
    while(*str++)
        len++;
    return len;
}

void* memset(void *buf, int val, size_t sz)
{
    for(size_t i = 0; i < sz; ++i)
    {
        ((uint8_t*)buf)[i] = (uint8_t) val;
    }
    return buf;
}

void* memcpy(void *dest, void *src, size_t sz)
{
    while(sz--)
        *(char*)dest++ = *(char*)src++;
    return dest;
}
