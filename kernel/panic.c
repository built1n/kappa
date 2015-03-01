#include "panic.h"
#include <stdarg.h>
#include <stdio.h>

__attribute__((noreturn)) void panic(const char *str, ...)
{
    /* no printf formatting for now */
    printf("KERNEL PANIC: ");
    va_list ap;
    va_start(ap, str);
    vprintf(str, ap);
    va_end(ap);
    for(;;)
    {
        asm("cli");
        asm("hlt");
    }
}
