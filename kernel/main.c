#include "string.h"
#include "tty.h"
#include "vga.h"

void main(void)
{
    tty_init();
    gdt_init();
    tty_puts("GDT initialized\n");
    tty_set_color(VGA_MAKE_COLOR(VGA_LIGHT_GRAY, VGA_BLACK));
    tty_puts("Hello, world!\n");
    while(1)
        ;
}
