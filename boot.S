        .set ALIGN,      1<<0
        .set MEMINFO,    1<<1
        .set FLAGS,      ALIGN | MEMINFO
        .set MAGIC,      0x1BADB002 # "1 Bad Boot"
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
        .skip 32768             # 32KB stack
stack_top:

.section .text
        .global _start
        .type _start, @function
_start:                         # This is where execution begins
        movl $stack_bottom, %esp        # Initialize the stack
        push %ebp               # Add register ebp to the arguments of kernel_main
        call kernel_main        # Start the kernel, shouldn't return
        cli
        hlt
.kernel_hang:                   # Idle
        jmp .kernel_hang
        .size _start, . - _start
