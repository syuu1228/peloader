/*
 * Copyright 2012 <James.Bottomley@HansenPartnership.com>
 *
 * see COPYING file
 *
 * Simple elf loader based on Intel TianoCore
 */

#include <efi.h>
#include <efilib.h>

#include <simple_file.h>
#include <sha256.h>

CHAR16 *loader = L"\\hello.efi";

UINT8 check[SHA256_DIGEST_SIZE] = {0x6d,0x5e,0x7c,0x1d,0x1e,0x95,0x82,0x21,0x8e,0xb3,0xff,0x72,0xb8,0x20,0x53,0x08,0x04,0xdb,0xfb,0x5f,0xbb,0x7d,0x97,0xd1,0x5b,0x0c,0x56,0x73,0x9a,0xf7,0x6e,0x6d};

EFI_STATUS
efi_main (EFI_HANDLE image, EFI_SYSTEM_TABLE *systab)
{
	EFI_STATUS efi_status;
	EFI_FILE *file;
	UINTN DataSize;
	void *buffer;
	sha256_context ctx;
	UINT8 hash[SHA256_DIGEST_SIZE];
	int i;

	InitializeLib(image, systab);
	
	efi_status = simple_file_open(image, loader, &file, EFI_FILE_MODE_READ);
	if (efi_status != EFI_SUCCESS) {
		Print(L"Failed to open %s\n", loader);
		return efi_status;
	}

	efi_status = simple_file_read_all(file, &DataSize, &buffer);
	if (efi_status != EFI_SUCCESS) {
		Print(L"Failed to read %s\n", loader);
		goto out_close_file;
	}

	sha256_starts(&ctx);
	sha256_update(&ctx, buffer, DataSize);
	sha256_finish(&ctx, hash);
	for (i = 0; i < SHA256_DIGEST_SIZE; i++) {
		Print(L"%02x", hash[i]);
	}
	Print(L"\n");

	for (i = 0; i < SHA256_DIGEST_SIZE; i++) {
		if (hash[i] != check[i]) {
			Print(L"Not matched!\n");
		}
	}
	FreePool(buffer);
 out_close_file:
	simple_file_close(file);

	return efi_status;
}
