CC=x86_64-w64-mingw32-gcc
CFLAGS= -Wall -Wextra -e efi_main -nostdinc -nostdlib -fno-builtin -Wl,--subsystem,10
QEMU=qemu-system-x86_64
OBJS=metallica.o

metallica.efi: $(OBJS)
	$(CC) $(CFLAGS) -e efi_main -o $@ $+
	mv metallica.efi ./fs/EFI/BOOT/BOOTX64.efi

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $+

run:
	$(QEMU) -m 4G -bios OVMF.fd -hda fat:rw:fs -boot c

clean:
	rm -f *~ include/*~ *.o *.efi fs/EFI/BOOT/BOOTX64.efi

.PHONY:clean
