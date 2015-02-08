        .set ALIGN,      1<<0
        .set MEMINFO,    1<<1
        .set GFXMODE,    1<<2 # this flag enables the graphics fields
        .set FLAGS,      ALIGN | MEMINFO | GFXMODE
        .set MAGIC,      0x1BADB002 # multiboot magic
        .set CHECKSUM,   -(MAGIC + FLAGS)

.section .multiboot
        .align 4
multiboot_header:
        .long MAGIC
        .long FLAGS
        .long CHECKSUM
        .long 0
        .long 0
        .long 0
        .long 0
        .long 0
        .long 0 # 1=text mode
        .long 640 # screen height (don't care)
        .long 480 # screen width (don't care)
        .long 32 # screen BPP: MUST be 32

.section .stack
stack_bottom:                   # Stack grows up in addresses, so bottom is
                                # lower in memory than the top
        .skip 16384             # 16KB stack
stack_top:

.section .text
        .global _start
        .type _start, @function
_start:
        cli
        movl $stack_top, %esp
        push %eax # multiboot magic
        push %ebx # multiboot header
        call main
        cli
        hlt
.Lhang:                   # Idle
        jmp .Lhang
