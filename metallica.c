#include "include/common.h"
#include "include/config.h"
#include "include/efi.h"
#include "include/fb.h"
#include "include/file.h"

#define KERNEL_FILE_NAME L"kernel.bin"

struct __attribute__((packed)) platform_info {
  struct fb fb;
  void *rsdp;
  void *fs_start;
  unsigned int nproc;
} platform_info;

void efi_main(void *ImageHandle __attribute__((unused)),
              struct EFI_SYSTEM_TABLE *SystemTable) {

  // set system table to global valiable and disable watchdog timer
  efi_init(SystemTable);

  // set start address of kernel,file system and etc to structure conf
  config_init();

  struct EFI_FILE_PROTOCOL *root =
      search_volume_contains_file(KERNEL_FILE_NAME);
  if (root == NULL) {
    assert(1, L"No volume contains kernel.bin.");
  }

  unsigned char has_fs = TRUE;

  // init frame buffer
  init_fb();
  // set platform_info
  platform_info.fb.base = fb.base;
  platform_info.fb.size = fb.size;
  platform_info.fb.hr = fb.hr;
  platform_info.fb.vr = fb.vr;
  platform_info.rsdp = find_efi_acpi_table();

  if (has_fs == TRUE) {
    platform_info.fs_start = (void *)conf.fs_start;
  } else {
    platform_info.fs_start = NULL;
  }

  // get number of processor
  unsigned long long nproc, nproc_en;
  unsigned long long status =
      MSP->GetNumberOfProcessors(MSP, &nproc, &nproc_en);
  assert(status, L"MSP->GetNumberOfProcessors");

  platform_info.nproc = nproc_en;

  unsigned long long pnum;
	status = MSP->WhoAmI(MSP, &pnum);
	assert(status, L"MSP->WhoAmI");

  // set argument of kernel
  unsigned long long k_arg1 = (unsigned long long)ST;
  put_param(L"kernel_arg1", k_arg1);
  unsigned long long k_arg2 = (unsigned long long)&platform_info;
  put_param(L"kernel_arg2", k_arg2);
	unsigned long long k_arg3 = pnum;
	put_param(L"kernel_arg3", k_arg3);

  // SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
  while (1)
    ;
}
