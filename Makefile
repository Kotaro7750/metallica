CC=x86_64-w64-mingw32-gcc
CFLAGS= -Wall -Wextra -e efi_main -nostdinc -nostdlib -fno-builtin -Wl,--subsystem,10
QEMU=qemu-system-x86_64
OBJS=metallica.o config.o

metallica.efi: $(OBJS) libuefi/libuefi.a
	$(CC) $(CFLAGS) -e efi_main -o $@ $+
	mv metallica.efi ../nova/fs/EFI/BOOT/BOOTX64.efi

%.o: %.c
	$(CC) $(CFLAGS) -Iinclude -c -o $@ $<

libuefi/libuefi.a:
	make -C libuefi CC=$(CC) CFLAGS="$(CFLAGS)"

run:
	$(QEMU) -m 4G -bios OVMF.fd -hda fat:rw:../nova/fs -boot c

clean:
	rm -f *~ include/*~ *.o *.efi fs/EFI/BOOT/BOOTX64.efi
	make -C libuefi clean

.PHONY:clean
