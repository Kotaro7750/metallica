#include "../include/file.h"
#include "../include/common.h"
#include "../include/efi.h"

#define SAFETY_READ_UNIT 16384

struct EFI_FILE_PROTOCOL *
search_volume_contains_file(unsigned short *target_filename) {
  void **sfs_handlers;
  unsigned long long sfs_handlers_num = 0;
  unsigned long long status;

  status = ST->BootServices->LocateHandleBuffer(
      ByProtocol, &sfsp_guid, NULL, &sfs_handlers_num, (void ***)&sfs_handlers);
  assert(status, L"LocateHandleBuffer");

  put_param(L"Number of Volumes", sfs_handlers_num);

  struct EFI_FILE_PROTOCOL *fp;

  for (unsigned int i = 0; i < sfs_handlers_num; i++) {
    struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *tmp_sfsp;
    status = ST->BootServices->HandleProtocol(sfs_handlers[i], &sfsp_guid,
                                              (void ***)&tmp_sfsp);

    if (!check_warn_error(status, L"HandleProtocol(sfs_handles)"))
      continue;

    status = tmp_sfsp->OpenVolume(tmp_sfsp, &fp);
    if (!check_warn_error(status, L"OpenVolume(tmp_sfsp)"))
      continue;
    struct EFI_FILE_PROTOCOL *_target_fp;
    status = fp->Open(fp, &_target_fp, target_filename, EFI_FILE_MODE_READ, 0);
    if (!check_warn_error(status, L"This volume don't have target file."))
      continue;

    put_param(L"Volume containing kernel Found", i);
    return fp;
  }
  return NULL;
}

unsigned long long get_file_size(struct EFI_FILE_PROTOCOL *file) {
  unsigned long long status;
  unsigned long long fi_size = FILE_INFO_BUF_SIZE;
  unsigned long long fi_buf[FILE_INFO_BUF_SIZE];
  struct EFI_FILE_INFO *fi_ptr;

  status = file->GetInfo(file, &fi_guid, &fi_size, fi_buf);
  if (!check_warn_error(status, L"file->GetInfo failure."))
    return 0;

  fi_ptr = (struct EFI_FILE_INFO *)fi_buf;

  return fi_ptr->FileSize;
}

void safety_file_read(struct EFI_FILE_PROTOCOL *src, void *dst,
                      unsigned long long size) {
  unsigned long long status;

  unsigned char *d = dst;
  while (size > SAFETY_READ_UNIT) {
    unsigned long long unit = SAFETY_READ_UNIT;
    status = src->Read(src, &unit, (void *)d);
    assert(status, L"safety_read");
    d += unit;
    size -= unit;
  }

  while (size > 0) {
    unsigned long long tmp_size = size;
    status = src->Read(src, &tmp_size, (void *)d);
    assert(status, L"safety_read");
    size -= tmp_size;
  }
}
