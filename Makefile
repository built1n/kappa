OBJ := $(shell cat OBJ)
CC = gcc
LD = ld
INCLUDES = -Idrivers/include -Ikernel/include -Ilibc/include -Iapps/

CFLAGS = -std=gnu99 -ffreestanding -fno-stack-protector -nostdlib -Wall -Wextra -m32 $(INCLUDES) -g
OPTFLAGS = -fexpensive-optimizations -ftree-loop-vectorize -finline-functions -fomit-frame-pointer -ftree-vectorize -O1 -mtune=generic

QEMU = qemu-system-i386
BOCHS = bochs

AS = as
ASFLAGS = -march=i686 --32

ISODIR = isodir

all: kappa.bin

test: kappa.iso
	@echo "EMULATOR kappa.iso"
	@$(QEMU) kappa.iso

test-bochs: kappa.iso
	@echo "BOCHS bochs.cfg"
	@$(BOCHS) -f bochs.cfg -q

kappa.iso: kappa.bin
	@echo "Building ISO under $(ISODIR)/..."
	@mkdir -p $(ISODIR)
	@mkdir -p $(ISODIR)/boot
	@cp kappa.bin $(ISODIR)/efi.img
	@cp kappa.bin $(ISODIR)/boot/kappa.bin
	@mkdir -p $(ISODIR)/boot/grub
	@cp cdrom/grub.cfg $(ISODIR)/boot/grub
	@grub-mkrescue -o kappa.iso $(ISODIR) 2> /dev/null

kappa.bin: $(OBJ) $(SOURCES) Makefile
	@$(LD) -T kernel/linker.ld -o kappa.bin -melf_i386 $(OBJ) -L /usr/lib32 -lgcc_s -static
	@echo "LD $@"

drivers/gfx.o: drivers/gfx.c Makefile
	@echo "CC $<"
	@$(CC) $(CFLAGS) -O3 -msse -c $< -o $@

%.o: %.c Makefile
	@echo "CC $<"
	@$(CC) $(CFLAGS) $(OPTFLAGS) -c $< -o $@


%.o: %.S Makefile
	@echo "AS $<"
	@$(AS) $(ASFLAGS) -c $< -o $@
clean:
	@echo "Cleaning build directory..."
	@rm -f $(OBJ) kappa.iso kappa.bin
	@rm -rf $(ISODIR)

include autodeps
