#include "../include/efi.h"
#include "../include/common.h"

struct EFI_SYSTEM_TABLE *ST;
struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
struct EFI_MP_SERVICES_PROTOCOL *MSP;

struct EFI_GUID sfsp_guid = {0x0964e5b22,
                             0x6459,
                             0x11d2,
                             {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

void efi_init(struct EFI_SYSTEM_TABLE *system_table) {
  struct EFI_GUID gop_guid = {0x9042a9de,
                              0x23dc,
                              0x4a38,
                              {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};

  struct EFI_GUID msp_guid = {0x3fdda605,
                              0xa76e,
                              0x4f46,
                              {0xad, 0x29, 0x12, 0xf4, 0x53, 0x1b, 0x3d, 0x08}};

  ST = system_table;

  ST->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
  ST->BootServices->LocateProtocol(&gop_guid, NULL, (void **)&GOP);
  ST->BootServices->LocateProtocol(&msp_guid, NULL, (void **)&MSP);

  puts(L"Inittializze EFI ...DONE");
  puts(L'\0');
  puts(L"\r\n");
}

void *find_efi_acpi_table(void) {
  const struct EFI_GUID efi_acpi_table = {
      0x8868e871,
      0xe4f1,
      0x11d3,
      {0xbc, 0x22, 0x00, 0x80, 0xc7, 0x3c, 0x88, 0x81}};

  unsigned long long i;
  for (i = 0; i < ST->NumberOfTableEntries; i++) {
    struct EFI_GUID *guid = &ST->ConfigurationTable[i].VendorGuid;
    if ((guid->Data1 == efi_acpi_table.Data1) &&
        (guid->Data2 == efi_acpi_table.Data2) &&
        (guid->Data3 == efi_acpi_table.Data3)) {
      unsigned char is_equal = TRUE;
      unsigned char j;
      for (j = 0; j < 8; j++) {
        if (guid->Data4[j] != efi_acpi_table.Data4[j])
          is_equal = FALSE;
      }
      if (is_equal == TRUE)
        return ST->ConfigurationTable[i].VendorTable;
    }
  }
  return NULL;
}
