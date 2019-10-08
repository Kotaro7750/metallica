#include "../include/efi.h"
#include "../include/common.h"

struct EFI_SYSTEM_TABLE *ST;

void efi_init(struct EFI_SYSTEM_TABLE* system_table){
  ST = system_table;
  ST->BootServices->SetWatchdogTimer(0,0,0,NULL);
}
