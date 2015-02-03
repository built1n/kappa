OBJ = drivers/tty.o kernel/main.o boot/head.o kernel/io.o kernel/gdt-as.o kernel/gdt.o libc/stdio.o libc/string.o kernel/idt.o kernel/idt-as.o kernel/isr.o kernel/irq-as.o kernel/isr-as.o kernel/irq.o kernel/panic.o kernel/timer.o drivers/ps2.o
CC = gcc
LD = ld
INCLUDES = -Idrivers/include -Ikernel/include -Ilibc/include
CFLAGS = -std=gnu99 -ffreestanding -fno-stack-protector -nostdlib -Wall -Wextra -m32 $(INCLUDES) -g

AS = as
ASFLAGS=-march=i686 --32

all: kappa.bin

test: iso
	qemu-system-i386 kappa.iso

iso: kappa.bin
	mkdir -p isodir
	mkdir -p isodir/boot
	cp kappa.bin isodir/efi.img
	cp kappa.bin isodir/boot/kappa.bin
	mkdir -p isodir/boot/grub
	cp cdrom/grub.cfg isodir/boot/grub
	grub-mkrescue -o kappa.iso isodir

kappa.bin: $(OBJ) $(SOURCES) Makefile
	$(LD) -T kernel/linker.ld -o kappa.bin -melf_i386 $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) kappa.iso kappa.bin
