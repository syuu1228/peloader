/*
 * Copyright 2012 <James.Bottomley@HansenPartnership.com>
 *
 * see COPYING file
 */

#include <efi.h>
#include <efilib.h>

#include <PeImage.h>		/* for ALIGN_VALUE */
#include <simple_file.h>
#include <execute.h>		/* for generate_path() */

static EFI_GUID IMAGE_PROTOCOL = LOADED_IMAGE_PROTOCOL;
static EFI_GUID SIMPLE_FS_PROTOCOL = SIMPLE_FILE_SYSTEM_PROTOCOL;
static EFI_GUID FILE_INFO = EFI_FILE_INFO_ID;

EFI_STATUS
simple_file_open_by_handle(EFI_HANDLE device, CHAR16 *name, EFI_FILE **file, UINT64 mode)
{
	EFI_STATUS efi_status;
	EFI_FILE_IO_INTERFACE *drive;
	EFI_FILE *root;

	efi_status = uefi_call_wrapper(BS->HandleProtocol, 3, device,
				       &SIMPLE_FS_PROTOCOL, &drive);

	if (efi_status != EFI_SUCCESS) {
		Print(L"Unable to find simple file protocol (%d)\n", efi_status);
		goto error;
	}

	efi_status = uefi_call_wrapper(drive->OpenVolume, 2, drive, &root);

	if (efi_status != EFI_SUCCESS) {
		Print(L"Failed to open drive volume (%d)\n", efi_status);
		goto error;
	}

	efi_status = uefi_call_wrapper(root->Open, 5, root, file, name,
				       mode, 0);

 error:
	return efi_status;
}

EFI_STATUS
simple_file_open(EFI_HANDLE image, CHAR16 *name, EFI_FILE **file, UINT64 mode)
{
	EFI_STATUS efi_status;
	EFI_HANDLE device;
	EFI_LOADED_IMAGE *li;
	EFI_DEVICE_PATH *loadpath = NULL;
	CHAR16 *PathName = NULL;

	efi_status = uefi_call_wrapper(BS->HandleProtocol, 3, image,
				       &IMAGE_PROTOCOL, &li);

	if (efi_status != EFI_SUCCESS)
		return simple_file_open_by_handle(image, name, file, mode);

	efi_status = generate_path(name, li, &loadpath, &PathName);

	if (efi_status != EFI_SUCCESS) {
		Print(L"Unable to generate load path for %s\n", name);
		return efi_status;
	}

	device = li->DeviceHandle;

	efi_status = simple_file_open_by_handle(device, PathName, file, mode);

	FreePool(PathName);
	FreePool(loadpath);

	return efi_status;
}

EFI_STATUS
simple_file_read_all(EFI_FILE *file, UINTN *size, void **buffer)
{
	EFI_STATUS efi_status;
	EFI_FILE_INFO *fi;
	char buf[1024];

	*size = sizeof(buf);
	fi = (void *)buf;
	

	efi_status = uefi_call_wrapper(file->GetInfo, 4, file, &FILE_INFO,
				       size, fi);
	if (efi_status != EFI_SUCCESS) {
		Print(L"Failed to get file info\n");
		return efi_status;
	}

	*size = fi->FileSize;

	/* might use memory mapped, so align up to nearest page */
	*buffer = AllocateZeroPool(ALIGN_VALUE(*size, 4096));
	if (!*buffer) {
		Print(L"Failed to allocate buffer of size %d\n", *size);
		return EFI_OUT_OF_RESOURCES;
	}
	efi_status = uefi_call_wrapper(file->Read, 3, file, size, *buffer);

	return efi_status;
}

void
simple_file_close(EFI_FILE *file)
{
	uefi_call_wrapper(file->Close, 1, file);
}
