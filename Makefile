OBJ := $(shell cat OBJ)
CC = gcc
LD = ld
INCLUDES = -Idrivers/include -Ikernel/include -Ilibc/include
CFLAGS = -std=gnu99 -ffreestanding -fno-stack-protector -nostdlib -Wall -Wextra -m32 $(INCLUDES) -g
EMULATOR = qemu-system-i386

AS = as
ASFLAGS=-march=i686 --32

ISODIR = isodir

all: kappa.bin

test: iso
	@echo "EMULATOR kappa.iso"
	@$(EMULATOR) kappa.iso

iso: kappa.bin
	@echo "Building ISO under $(ISODIR)/..."
	@mkdir -p $(ISODIR)
	@mkdir -p $(ISODIR)/boot
	@cp kappa.bin $(ISODIR)/efi.img
	@cp kappa.bin $(ISODIR)/boot/kappa.bin
	@mkdir -p $(ISODIR)/boot/grub
	@cp cdrom/grub.cfg $(ISODIR)/boot/grub
	@grub-mkrescue -o kappa.iso $(ISODIR) 2> /dev/null

kappa.bin: $(OBJ) $(SOURCES) Makefile
	@$(LD) -T kernel/linker.ld -o kappa.bin -melf_i386 $(OBJ)
	@echo "LD $@"
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "CC $<"

%.o: %.S
	@$(AS) $(ASFLAGS) -c $< -o $@
	@echo "AS $<"
clean:
	@echo "Cleaning build directory..."
	@rm -f $(OBJ) kappa.iso kappa.bin
	@rm -rf $(ISODIR)
