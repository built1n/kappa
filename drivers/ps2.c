/* this is both a PS/2 keyboard AND a PS/2 MOUSE driver */
#include <stdint.h>
#include <stdio.h>
#include "io.h"
#include "isr.h"
#include "ps2.h"

static void ps2_wait(void)
{
    /* wait for the keyboard */
    while(1)
        if ((inb(0x64) & 2) == 0) break;
}

void ps2_set_leds(uint8_t status)
{
    ps2_wait();
    outb(0x60, 0xED);
    outb(0x60, status);
}

static void key_handler(struct regs_t *regs)
{
    (void) regs;
    uint8_t scancode = inb(0x60);
    (void) scancode;
    /* TODO: handle scancode */
    /*printf("key %x\n", scancode);*/
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

static void mouse_handler(struct regs_t *regs)
{
    (void) regs;
    printf("mouse action!\n");
}

static void mouse_init(void)
{
    /* enable IRQ12 */
    set_interrupt_handler(IRQ(12), mouse_handler);
    /* make the ps/2 controller generate IRQ12 */
    outb(0x64, 0x20);
    uint8_t status = inb(0x64);
    /* set bit 1 and unset bit 5 */
    status |= (1 << 1);
    status &= ~(1 << 5);
    outb(0x64, 0x60);
    outb(0x60, status);
}

void ps2_init(void)
{
    keyboard_init();
    mouse_init();
}
