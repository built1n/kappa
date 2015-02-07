#include "io.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define BOCHS_PUTCHAR(ch) (outb(0xe9, ch))

int log_putchar(int ch)
{
    BOCHS_PUTCHAR(ch);
    return 0;
}

int log_puts(const char* str)
{
    while(*str)
        log_putchar(*str++);
    return 0;
}

static char hex_table[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

static void log_print_hex(unsigned int n)
{
    unsigned mask = 0xF0000000;
    unsigned shift = 28;
    while(mask)
    {
        log_putchar(hex_table[(n & mask) >> shift]);
        mask >>= 4;
        shift -= 4;
    }
}

int log(const char *fmt, ...)
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
                log_putchar(va_arg(ap, int));
                break;
            case 's':
                log_puts(va_arg(ap, const char*));
                break;
            case 'x':
                log_print_hex(va_arg(ap, unsigned));
                break;
            case 'd':
                log_puts(itoa(va_arg(ap, unsigned), 10));
                break;
            default:
                log_puts("klog: unknown format\n");
                break;
            }
            break;
        }
        default:
            log_putchar(ch);
            break;
        }
    }
    va_end(ap);
    return 0;
}
