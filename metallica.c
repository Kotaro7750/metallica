#include "include/efi.h"
#include "include/config.h"
#include "include/common.h"

#define KERNEL_FILE_NAME	L"kernel.bin"

void efi_main(void *ImageHandle __attribute__((unused)),
              struct EFI_SYSTEM_TABLE *SystemTable) {
  efi_init(SystemTable);
  config_init();

  struct EFI_FILE_PROTOCOL *root =
		search_volume_contains_file(KERNEL_FILE_NAME);
	if (root == NULL) {
		assert(1, L"No volume contains kernel.bin.");
	}

  SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
  SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello UEFI!\n");
  while (1)
    ;
}
