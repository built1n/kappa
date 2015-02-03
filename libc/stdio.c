#include "stdio.h"
#include "tty.h"
#include <stdarg.h>

int putchar(int ch)
{
    tty_putchar((char)ch);
    return 0;
}

int puts(const char* str)
{
    tty_puts(str);
    return 0;
}

static char hex_table[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

static void print_hex(unsigned int n)
{
    unsigned mask = 0xF0000000;
    unsigned shift = 28;
    while(mask)
    {
        putchar(hex_table[(n & mask) >> shift]);
        mask >>= 4;
        shift -= 4;
    }
}

int printf(const char *fmt, ...)
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
                putchar(va_arg(ap, int));
                break;
            case 's':
                puts(va_arg(ap, const char*));
                break;
            case 'x':
                print_hex(va_arg(ap, unsigned));
                break;
            default:
                puts("printf: unknown format\n");
                break;
            }
            break;
        }
        default:
            putchar(ch);
            break;
        }
    }
    va_end(ap);
    return 0;
}
