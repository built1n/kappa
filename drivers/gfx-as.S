        .global gfx_clear
        .extern fb_width
        .extern fb_height
        .extern fb_bpp
        .extern _gfx_bgcol
        .extern framebuffer
        # void gfx_clear(uint32_t color)
gfx_clear:
        movzwl  fb_width, %ecx
        movzwl  fb_height, %edx
        movl    framebuffer, %eax
        pushl   %ebx
        movl    _gfx_bgcol, %ebx
        imull   %ecx, %edx
        movzbl  fb_bpp, %ecx
        imull   %ecx, %edx
        addl    %eax, %edx
        cmpl    %edx, %eax
        jnb     .L2
.L6:
        movl    %ebx, (%eax)
        addl    %ecx, %eax
        cmpl    %eax, %edx
        ja      .L6
.L2:
        popl    %ebx
        ret
