#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "heap.h"
#include "panic.h"

/* adapted from <http://www.strudel.org.uk/itoa/> */
char* itoa(int val, int base)
{
    static char buf[32] = {0};

    int neg = 0;

    if(val < 0)
    {
        val = -val;
        neg = 1;
    }

    int i = 30;

    do
    {
        buf[i] = "0123456789abcdef"[val % base];
        --i;
        val /= base;
    }
    while(val && i);

    if(neg)
    {
        buf[i] = '-';
    }

    return &buf[i+(neg?0:1)];
}

#define RAND_A1 12
#define RAND_A2 25
#define RAND_A3 27

#define RAND_C1 4101842887655102017LL
#define RAND_C2 2685821657736338717LL

static uint64_t rand_state = RAND_C1;

uint64_t rand64(void)
{
    /* Marsaglia's Xorshift generator combined with a LCRNG */
    rand_state ^= rand_state >> RAND_A1;
    rand_state ^= rand_state << RAND_A2;
    rand_state ^= rand_state >> RAND_A3;
    return rand_state * RAND_C2;
}

static uint64_t rand_temp;
static int bytes_left = 0;

unsigned int rand(void)
{
    if(bytes_left < 4)
    {
        rand_temp = rand64();
        bytes_left = 8;
    }
    unsigned int ret = rand_temp & 0xFFFFFFFF;
    rand_temp >>= 32;
    bytes_left -= 4;
    return ret;
}

void srand(uint64_t seed)
{
    if(seed == RAND_C1)
        seed = RAND_C1 + 1;
    rand_state = RAND_C1 ^ seed;
}

int abs(int val)
{
    return (val<0?-val:val);
}

void *malloc(size_t sz)
{
    return kmalloc(sz);
}

static inline int snputs(char *buf, int idx, int sz, const char *str)
{
    while(idx < sz && *str)
    {
        buf[idx++] = *str++;
    }
    return idx;
}

static char hex_table[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

static inline int snprint_hex(char *buf, int idx, int sz, unsigned int n)
{
    unsigned mask = 0xF0000000;
    unsigned shift = 28;
    while(mask && idx < sz)
    {
        buf[idx++] = hex_table[(n & mask) >> shift];
        mask >>= 4;
        shift -= 4;
    }
    return idx;
}

int snprintf(char *buf, int sz, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int i = 0;
    /* leave room for the null! */
    sz--;
    while(*fmt && i < sz)
    {
        char ch = *fmt++;
        switch(ch)
        {
        case '%':
        {
            switch(*fmt++)
            {
            case 'c':
                buf[i++] = va_arg(ap, int);
                break;
            case 's':
                i = snputs(buf, i, sz, va_arg(ap, const char*));
                break;
            case 'x':
                i =  snprint_hex(buf, i, sz, va_arg(ap, unsigned));
                break;
            case 'd':
                i = snputs(buf, i, sz, itoa(va_arg(ap, unsigned), 10));
                break;
            default:
                i = snputs(buf, i, sz, "snprintf: unknown format\n");
                break;
            }
            break;
        }
        default:
            buf[i++] = ch;
            break;
        }
    }
    buf[i] = '\0';
    va_end(ap);
    return 0;
}

void assert_fail(const char *func, const char *file, int line)
{
    printf("\nAssertion failed in function %s in file %s, line %d\n", func, file, line);
    panic("assertion failed!\n");
}

int toupper(int ch)
{
    if('a' <= ch && ch <= 'z')
        return ch ^ (1<<5);
    return ch;
}

int tolower(int ch)
{
    if('A' <= ch && ch <= 'Z')
        return ch ^ (1<<5);
    return ch;
}
