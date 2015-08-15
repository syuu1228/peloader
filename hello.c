/*
 * Copyright 2012 <James.Bottomley@HansenPartnership.com>
 *
 * see COPYING file
 *
 * Simple elf loader based on Intel TianoCore
 */

#include <efi.h>
#include <efilib.h>

EFI_STATUS
efi_main (EFI_HANDLE image, EFI_SYSTEM_TABLE *systab)
{
	EFI_STATUS efi_status;
	EFI_FILE *file;

	InitializeLib(image, systab);

	Print(L"Hello World!\n");

	return EFI_SUCCESS;
}
