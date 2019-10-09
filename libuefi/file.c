#include "../include/file.h"
#include "../include/common.h"
#include "../include/efi.h"

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
