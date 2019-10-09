#ifndef _FILE_H
#define _FILE_H

#define EFI_FILE_MODE_READ 0x0000000000000001
#include "efi.h"

struct EFI_FILE_PROTOCOL* search_volume_contains_file(unsigned short* target_filename);

#endif
