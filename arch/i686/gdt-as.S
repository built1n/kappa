        .global gdt_flush
gdt_flush:      # prototype: void gdt_flush(uint32)
        movl 4(%esp), %eax
        lgdt (%eax)
        # 0x8 is the code segment selector
        jmp $0x8, $.flush
.flush:
        mov $0x10, %ax # 0x10 is the data segment selector
        mov %ax, %ds
        mov %ax, %es
        mov %ax, %fs
        mov %ax, %gs
        mov %ax, %ss
        ret
