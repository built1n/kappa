#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

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

static unsigned long rand_s1 = 18, rand_s2 = 5, rand_s3 = 43;

#define M 1103515245UL
#define A 12345UL

/* this is a very fast tausworthe RNG */

unsigned int rand(void)
{
    rand_s1 = (((rand_s1&4294967294)<<12)^(((rand_s1<<13)^rand_s1)>>19));
    rand_s2 = (((rand_s2&4294967288)<< 4)^(((rand_s2<< 2)^rand_s2)>>25));
    rand_s3 = (((rand_s3&4294967280)<<17)^(((rand_s3<< 3)^rand_s3)>>11));
    return (rand_s1 ^ rand_s2 ^ rand_s3);
}

void srand(unsigned int seed)
{
    if(!seed)
        seed = 42;
    rand_s1 = seed++;
    rand_s2 = seed++;
    rand_s3 = seed++;
    /* "warm it up" */
    for(int i=0;i<10;++i)
        rand();
}

int abs(int val)
{
    return (val<0?-val:val);
}

void *malloc(size_t sz)
{
    static uint8_t mallocbuf[1024*1024*16];
    static uint8_t *next_ptr = mallocbuf;
    static int bytes_left = sizeof(mallocbuf);
    bytes_left -= sz;
    if(bytes_left < 0)
        return NULL;
    else
    {
        void *ret = next_ptr;
        next_ptr += sz;
        return ret;
    }
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
