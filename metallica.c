#include "include/common.h"
#include "include/config.h"
#include "include/efi.h"
#include "include/file.h"

#define KERNEL_FILE_NAME L"kernel.bin"

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

  //set argument of kernel
  unsigned long long k_arg1 = (unsigned long long)ST;
  put_param(L"kernel_arg1", k_arg1);

  //SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
  SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello UEFI!\n");
  while (1)
    ;
}
