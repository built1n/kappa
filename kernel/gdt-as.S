        .global gdt_flush
        .type gdt_flush, @function
gdt_flush: # prototype: void gdt_flush(uint32_t addr)
        cli
        movl 4(%esp), %eax # load the address off the stack (first param)
        lgdt (%eax)
        mov $0x10, %ax
        mov %ax, %ds
        mov %ax, %es
        mov %ax, %fs
        mov %ax, %gs
        mov %ax, %ss
        sti
        jmp .flush
.flush:
        ret
