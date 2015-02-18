/* this is both a PS/2 keyboard driver */
#include <stdint.h>
#include <stdio.h>
#include "io.h"
#include "isr.h"
#include "ps2kbd.h"
#include "ps2_keymaps.h"

static void ps2_wait(void)
{
    /* wait for the keyboard */
    while(1)
        if ((inb(0x64) & 2) == 0) break;
}

void ps2kbd_set_leds(uint8_t status)
{
    ps2_wait();
    outb(0x60, 0xED);
    outb(0x60, status);
}

#define IDX_UP 0
#define IDX_LEFT 1
#define IDX_DOWN 2
#define IDX_RIGHT 3

static uint8_t ps2_arrowkeys[4];

uint8_t ps2kbd_button_get(void)
{
    uint8_t ret = 0;
    if(ps2_arrowkeys[IDX_UP])
        ret |= BUTTON_UP;
    if(ps2_arrowkeys[IDX_LEFT])
        ret |= BUTTON_LEFT;
    if(ps2_arrowkeys[IDX_DOWN])
        ret |= BUTTON_DOWN;
    if(ps2_arrowkeys[IDX_RIGHT])
        ret |= BUTTON_RIGHT;
    return ret;
}

static uint8_t ps2_ctrl;
static uint8_t ps2_shift;
static uint8_t ps2_alt;

uint8_t ps2kbd_modifier_get(void)
{
    uint8_t ret = 0;
    if(ps2_ctrl)
        ret |= MODIFIER_CTRL;
    if(ps2_shift)
        ret |= MODIFIER_SHIFT;
    if(ps2_alt)
        ret |= MODIFIER_ALT;
    return ret;
}

static void key_handler(struct regs_t *regs)
{
    (void) regs;
    uint8_t scancode = inb(0x60);
    switch(scancode)
    {
    case 0x2A:
        ps2_shift = 1;
        break;
    case 0xAA:
        ps2_shift = 0;
        break;
    case 0x1D:
        ps2_ctrl = 1;
        break;
    case 0x9D:
        ps2_ctrl = 0;
        break;
    case 0x38:
        ps2_alt = 1;
        break;
    case 0xB8:
        ps2_alt = 0;
        break;
    case 0xE0:
    {
        uint8_t spec = inb(0x60);
        switch(spec)
        {
        case 0x48:
            ps2_arrowkeys[IDX_UP] = 1;
            break;
        case 0x4B:
            ps2_arrowkeys[IDX_LEFT] = 1;
            break;
        case 0x50:
            ps2_arrowkeys[IDX_DOWN] = 1;
            break;
        case 0x4D:
            ps2_arrowkeys[IDX_RIGHT] = 1;
            break;
        case 0xC8:
            ps2_arrowkeys[IDX_UP] = 0;
            break;
        case 0xCB:
            ps2_arrowkeys[IDX_LEFT] = 0;
            break;
        case 0xD0:
            ps2_arrowkeys[IDX_DOWN] = 0;
            break;
        case 0xCD:
            ps2_arrowkeys[IDX_RIGHT] = 0;
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void ps2_set_scancode_set(uint8_t set)
{
    ps2_wait();
    outb(0x60, 0xF0);
    outb(0x60, set);
}

static void keyboard_init(void)
{
    set_interrupt_handler(IRQ(1), key_handler);
    ps2_set_scancode_set(1);
}

void ps2kbd_init(void)
{
    keyboard_init();
}
