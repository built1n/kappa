#include <stdint.h>
#include "io.h"
#include "tty.h"
#include "vga.h"

static int term_x, term_y;
static uint8_t term_col;
/* VGA buffer starts at 0xB8000 */
static uint16_t *term_buf;

void tty_init(void)
{
    term_buf = (uint16_t*)0xB8000;
    tty_set_color(VGA_MAKE_COLOR(VGA_LIGHT_GRAY, VGA_BLACK));
    tty_clear();
}

static void move_cursor(uint16_t cursor_idx)
{
    outb(0x3D4, 14);
    outb(0x3D5, cursor_idx >> 8); // high byte
    outb(0x3D4, 15);
    outb(0x3D5, cursor_idx); // low byte
}

static void update_cursor(void)
{
    move_cursor(term_y * VGA_WIDTH + term_x);
}

void tty_clear(void)
{
    term_x = 0;
    term_y = 0;
    for(int y = 0; y < VGA_HEIGHT; ++y)
    {
        for(int x = 0; x < VGA_WIDTH; ++x)
        {
            term_buf[y * VGA_WIDTH + x] = VGA_MAKE_ENTRY(' ', term_col);
        }
    }
}

void tty_set_color(uint8_t color)
{
    term_col = color;
}

uint8_t tty_get_color(void)
{
    return term_col;
}

void tty_putchar_at(char ch, uint8_t col, int x, int y)
{
    term_buf[y * VGA_WIDTH + x] = VGA_MAKE_ENTRY(ch, col);
}

void tty_putchar(char ch)
{
    if(ch != '\n')
    {
        tty_putchar_at(ch, term_col, term_x, term_y);
        if(++term_x == VGA_WIDTH)
        {
            term_x = 0;
            if(++term_y == VGA_HEIGHT)
                term_y = 0;
        }
    }
    else
    {
        term_x = 0;
        if(++term_y == VGA_HEIGHT)
            term_y = 0;
    }
    update_cursor();
}

void tty_puts(const char *str)
{
    while(*str)
    {
        tty_putchar(*str++);
    }
}
