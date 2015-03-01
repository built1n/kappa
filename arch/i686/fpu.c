/* Copyright (C) 2011-2013 Kevin Lange */
/* this code from toaruos */
#include <stddef.h>
#include "fpu.h"

/**
 * Enable the FPU and SSE
 */
void fpu_enable(void) {
        asm volatile ("clts");
        size_t t;
        asm volatile ("mov %%cr4, %0" : "=r"(t));
        t |= 3 << 9;
        asm volatile ("mov %0, %%cr4" :: "r"(t));
}

/**
 * Disable FPU and SSE so it traps to the kernel
 */
void fpu_disable(void) {
        size_t t;
        asm volatile ("mov %%cr0, %0" : "=r"(t));
        t |= 1 << 3;
        asm volatile ("mov %0, %%cr0" :: "r"(t));
}
