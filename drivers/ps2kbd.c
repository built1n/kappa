/* this is a PS/2 keyboard driver */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
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

static struct ps2_specialkeys_t special_keys;

static void handle_special_key(uint8_t scancode, int release)
{
    int press = ~release;
    switch(ps2_set1_special[scancode])
    {
    case SPECIAL_SHIFT:
        special_keys.shift    = press;
        break;
    case SPECIAL_CTRL:
        special_keys.ctrl     = press;
        break;
    case SPECIAL_BKSP:
        special_keys.bksp     = press;
        break;
    case SPECIAL_ALT:
        special_keys.alt      = press;
        break;
    case SPECIAL_NUMLOCK:
        special_keys.numlock  = ~special_keys.numlock;
        ps2kbd_set_leds((special_keys.capslock << 2) | (special_keys.numlock << 1) | special_keys.scrllock);
        break;
    case SPECIAL_CAPLOCK:
        special_keys.capslock = ~special_keys.capslock;
        ps2kbd_set_leds((special_keys.capslock << 2) | (special_keys.numlock << 1) | special_keys.scrllock);
        break;
    case SPECIAL_SCRLLOCK:
        special_keys.scrllock = ~special_keys.scrllock;
        ps2kbd_set_leds((special_keys.capslock << 2) | (special_keys.numlock << 1) | special_keys.scrllock);
        break;
    case SPECIAL_ESC:
        special_keys.esc = press;
        break;
    case SPECIAL_F1:
        special_keys.f1 = press;
        break;
    case SPECIAL_F2:
        special_keys.f2 = press;
        break;
    case SPECIAL_F3:
        special_keys.f3 = press;
        break;
    case SPECIAL_F4:
        special_keys.f4 = press;
        break;
    case SPECIAL_F5:
        special_keys.f5 = press;
        break;
    case SPECIAL_F6:
        special_keys.f6 = press;
        break;
    case SPECIAL_F7:
        special_keys.f7 = press;
        break;
    case SPECIAL_F8:
        special_keys.f8 = press;
        break;
    case SPECIAL_F9:
        special_keys.f9 = press;
        break;
    case SPECIAL_F10:
        special_keys.f10 = press;
        break;
    case SPECIAL_F11:
        special_keys.f11 = press;
        break;
    case SPECIAL_F12:
        special_keys.f12 = press;
        break;
    }
}

static void handle_extended_scancode(void)
{
    uint8_t temp = inb(0x60);
    (void) temp;
    //printf("Extended scancode: 0x%x\n", temp);
}

static void key_handler(struct regs_t *regs)
{
    (void) regs;
    uint8_t scancode = inb(0x60);
    //printf("INTR SCAN: 0x%x\n", scancode);
    if(scancode == EXTENDED_SCANCODE)
    {
        handle_extended_scancode();
        return;
    }

    /* AND by 0x7F to get in the range of [0,128) */

    int type = ps2_set1_scancodes[scancode & 0x7F];
    int release = (scancode & (1<<7)) >> 7;
    switch(type)
    {
    case PRINTING_KEY:
    {
        if(!release)
        {
            int capitals = special_keys.capslock;
            if(special_keys.shift)
                capitals = ~capitals;
            if(capitals)
                putchar(ps2_set1_shift[scancode]);
            else
                putchar(ps2_set1_ascii[scancode]);
        }
        break;
    }
    case SPECIAL_KEY:
        handle_special_key(scancode & 0x7F, release);
        break;
    }
    if(special_keys.bksp)
        putchar('\b');
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
    memset(&special_keys, 0, sizeof(special_keys));
}

void ps2kbd_init(void)
{
    keyboard_init();
}
