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

static void ps2_handler(struct regs_t regs)
{
    (void) regs;
    uint8_t scancode = inb(0x60);
    /* TODO: handle scancode */
    printf("key %x\n", scancode);
}

static void ps2_set_scancode_set(uint8_t set)
{
    ps2_wait();
    outb(0x60, 0xF0);
    outb(0x60, set);
}

void ps2_init(void)
{
    set_interrupt_handler(IRQ(1), ps2_handler);
    ps2_set_scancode_set(1);
}
