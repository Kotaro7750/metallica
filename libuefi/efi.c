#include "../include/efi.h"
#include "../include/common.h"

struct EFI_SYSTEM_TABLE *ST;

struct EFI_GUID sfsp_guid =  {0x0964e5b22,0x6459,0x11d2,{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}};

void efi_init(struct EFI_SYSTEM_TABLE* system_table){
  ST = system_table;
  ST->BootServices->SetWatchdogTimer(0,0,0,NULL);
  puts(L"Inittializze EFI ...DONE");
  puts(L'\0');
  puts(L"\r\n");
}
