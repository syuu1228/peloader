FILES = simple_file.o pecoff.o guid.o  \
	execute.o sha256.o
LIBFILES = $(FILES) kernel_efivars.o
EFILIBFILES = $(patsubst %.o,%.efi.o,$(FILES))

include ../Make.rules

lib.a: $(LIBFILES)
lib-efi.a: $(EFILIBFILES)

clean:
	rm -f lib.a
	rm -f lib-efi.a
	rm -f $(LIBFILES)
	rm -f $(EFILIBFILES)

