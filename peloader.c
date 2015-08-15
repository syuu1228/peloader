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
#include <pecoff.h>

CHAR16 *loader = L"\\linux-loader.efi";


EFI_STATUS
efi_main (EFI_HANDLE image, EFI_SYSTEM_TABLE *systab)
{
	EFI_STATUS efi_status;
	EFI_FILE *file;

	InitializeLib(image, systab);

	efi_status = simple_file_open(image, loader, &file, EFI_FILE_MODE_READ);
	if (efi_status != EFI_SUCCESS) {
		Print(L"Failed to open %s\n", loader);
		return efi_status;
	}

	efi_status = pecoff_execute_image(file, loader, image, systab);
	simple_file_close(file);

	return efi_status;
}
