#include "io.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define BOCHS_PUTCHAR(ch) (outb(0xe9, ch))

static int eputchar(int ch)
{
    BOCHS_PUTCHAR(ch);
    return 0;
}

static int eputs(const char* str)
{
    while(*str)
        putchar(*str++);
    return 0;
}

static char ehex_table[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

static void eprint_hex(unsigned int n)
{
    unsigned mask = 0xF0000000;
    unsigned shift = 28;
    while(mask)
    {
        eputchar(ehex_table[(n & mask) >> shift]);
        mask >>= 4;
        shift -= 4;
    }
}

int klog(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    while(*fmt)
    {
        char ch = *fmt++;
        switch(ch)
        {
        case '%':
        {
            switch(*fmt++)
            {
            case 'c':
                eputchar(va_arg(ap, int));
                break;
            case 's':
                eputs(va_arg(ap, const char*));
                break;
            case 'x':
                eprint_hex(va_arg(ap, unsigned));
                break;
            case 'd':
                eputs(itoa(va_arg(ap, unsigned), 10));
                break;
            default:
                eputs("klog: unknown format\n");
                break;
            }
            break;
        }
        default:
            eputchar(ch);
            break;
        }
    }
    va_end(ap);
    return 0;
}
