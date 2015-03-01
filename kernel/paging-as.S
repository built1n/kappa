        .global do_paging_enable
do_paging_enable:
        movl 4(%esp), %eax # loads page directory address
        mov %eax, %cr3
        mov %cr0, %eax
        orl $0x80000000, %eax # set PG bit
        sti
        mov %eax, %cr0
        ret
