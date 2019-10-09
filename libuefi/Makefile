AR = x86_64-w64-mingw32-ar
OBJS = common.o efi.o file.o fb.o

libuefi.a: $(OBJS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -I../include -c -o $@ $<

clean:
	rm -f *~ *.o *.a

.PHONY: clean
