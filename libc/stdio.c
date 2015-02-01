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
