/*
 * Copyright 2012 <James.Bottomley@HansenPartnership.com>
 *
 * see COPYING file
 */

#include <guid.h>
#include <stdio.h>

#define ARRAY_SIZE(a) (sizeof (a) / sizeof ((a)[0]))

/* all the necessary guids */
EFI_GUID IMAGE_PROTOCOL = LOADED_IMAGE_PROTOCOL;
EFI_GUID SIMPLE_FS_PROTOCOL = SIMPLE_FILE_SYSTEM_PROTOCOL;

