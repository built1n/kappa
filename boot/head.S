        .set ALIGN,      1<<0
        .set MEMINFO,    1<<1
        .set FLAGS,      ALIGN | MEMINFO
        .set MAGIC,      0x1BADB002 # multiboot magic
        .set CHECKSUM,   -(MAGIC + FLAGS)

.section .multiboot
        .align 4
multiboot_header:
        .long MAGIC
        .long FLAGS
        .long CHECKSUM

.section .stack
stack_bottom:                   # Stack grows up in addresses, so bottom is
                                # lower than the top
        .skip 16384             # 32KB stack
stack_top:

.section .text
        .global _start
        .type _start, @function
_start:
        movl $stack_top, %esp
        push %eax # multiboot magic
        push %ebp # multiboot header
        call main
        cli
        hlt
.Lhang:                   # Idle
        jmp .Lhang
        .size _start, . - _start
