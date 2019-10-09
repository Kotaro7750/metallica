#include "../include/mem.h"
#include "../include/common.h"
#include "../include/efi.h"

#define MEM_DESC_SIZE 4800
unsigned char mem_desc[MEM_DESC_SIZE];
unsigned long long mem_desc_num;
unsigned long long mem_desc_unit_size;
unsigned long long map_key;

void exit_boot_services(void *IH) {
  unsigned long long status;
  unsigned long long mmap_size;
  unsigned int desc_ver;

  do {
    mmap_size = MEM_DESC_SIZE;
    status = ST->BootServices->GetMemoryMap(
        &mmap_size, (struct EFI_MEMORY_DESCRIPTOR *)mem_desc, &map_key,
        &mem_desc_unit_size, &desc_ver);
  } while (!check_warn_error(status, L"GetMemoryMap"));

  status = ST->BootServices->ExitBootServices(IH, map_key);
  assert(status, L"ExitBootServices");
}
