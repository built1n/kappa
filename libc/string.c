#include <stdint.h>
#include <stddef.h>
#include "string.h"

size_t strlen(const char *str)
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

char* strdup(const char *str)
{
    int len = strlen(str);
    /* allocate room for the string and it's NULL terminator */
    char *ret = malloc(len + 1);
    memcpy(ret, str, len + 1);
    return ret;
}

char* strncat(char *dest, const char *src, size_t n)
{
    /* save this for the return */
    char *d = dest;
    while(*dest++);
    while(n && (*dest++ = *src++)) --n;
    if(*dest)
        *dest = '\0';
    return d;
}
