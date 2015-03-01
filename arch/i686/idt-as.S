        .global idt_flush
        .type idt_flush, @function
idt_flush:      # prototype: void idt_flush(uint32)
        movl 4(%esp), %eax
        lidt (%eax)
        ret
