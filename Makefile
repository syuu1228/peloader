EFIFILES = peloader.efi sha256sum.efi hello.efi

export TOPDIR	:= $(shell pwd)/

include Make.rules

all: $(EFIFILES)

lib/lib.a lib/lib-efi.a: FORCE
	$(MAKE) -C lib $(notdir $@)

.SUFFIXES: .crt

.KEEP: $(EFIFILES)

peloader.so: lib/lib-efi.a
sha256sum.so: lib/lib-efi.a
hello.so: lib/lib-efi.a
hashcheck.so: lib/lib-efi.a

clean:
	rm -f $(EFIFILES) *.o *.so
	$(MAKE) -C lib clean

FORCE:



