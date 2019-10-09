#ifndef _FILE_H
#define _FILE_H

#define EFI_FILE_MODE_READ 0x0000000000000001
#define MAX_FILE_NAME_LEN 4
#define MAX_FILE_NUM 10
#define MAX_FILE_BUF 1024
#define FILE_INFO_BUF_SIZE 180

#include "efi.h"

struct EFI_FILE_PROTOCOL *
search_volume_contains_file(unsigned short *target_filename);

unsigned long long get_file_size(struct EFI_FILE_PROTOCOL *file);
void safety_file_read(struct EFI_FILE_PROTOCOL *src, void *dst,
                      unsigned long long size);

#endif
