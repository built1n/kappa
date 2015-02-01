#include "panic.h"
#include <stdio.h>

__attribute__((noreturn)) void panic(const char *str, ...)
{
    /* no printf formatting for now */
    printf("KERNEL PANIC: %s", str);
    for(;;)
    {
        asm("cli");
        asm("hlt");
    }
}
