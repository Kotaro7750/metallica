#include "include/common.h"
#include "include/config.h"
#include "include/efi.h"
#include "include/fb.h"
#include "include/file.h"
#include "include/mem.h"

#define KERNEL_FILE_NAME L"kernel.bin"
void load_kernel(struct EFI_FILE_PROTOCOL *root,
                 unsigned short *kernel_file_name);

void put_n_bytes(unsigned char *addr, unsigned int num);

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

  load_config(root, CONF_FILE_NAME);
  load_kernel(root, KERNEL_FILE_NAME);

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

  SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

  exit_boot_services(ImageHandle);

  unsigned long long _sb = conf.stack_base, _ks = conf.kernel_start;
  __asm__("	mov	%0, %%rdx\n"
          "	mov	%1, %%rsi\n"
          "	mov	%2, %%rdi\n"
          "	mov	%3, %%rsp\n"
          "	jmp	*%4\n" ::"m"(k_arg3),
          "m"(k_arg2), "m"(k_arg1), "m"(_sb), "m"(_ks));

  while (TRUE)
    ;
  while (1)
    ;
}

void load_kernel(struct EFI_FILE_PROTOCOL *root,
                 unsigned short *kernel_file_name) {
  struct EFI_FILE_PROTOCOL *kernel;
  unsigned long long status =
      root->Open(root, &kernel, kernel_file_name, EFI_FILE_MODE_READ, 0);
  assert(status, L"root->Open(kernel)");

  unsigned long long kernel_size = get_file_size(kernel);
  put_param(L"kernel_size", kernel_size);

  struct header {
    void *bss_start;
    unsigned long long bss_size;
  } head;

  unsigned long long head_size = sizeof(head);
  puts(L"load kernel head ... ");
  safety_file_read(kernel, (void *)&head, head_size);
  puts(L"done\r\n");

  kernel_size -= sizeof(head);
  puts(L"load kernel body ... ");
  safety_file_read(kernel, (void *)conf.kernel_start, kernel_size);
  puts(L"done\r\n");
  kernel->Close(kernel);

  ST->BootServices->SetMem(head.bss_start, head.bss_size, 0);
  puts(L"kernel body first 16 bytes: 0x");
  put_n_bytes((unsigned char *)conf.kernel_start, 16);
  puts(L"\r\n");
  puts(L"kernel body last 16 bytes: 0x");
  unsigned char *kernel_last =
      (unsigned char *)(conf.kernel_start + kernel_size - 16);
  put_n_bytes(kernel_last, 16);
  puts(L"\r\n");
}

void put_n_bytes(unsigned char *addr, unsigned int num) {
  unsigned int i;
  for (i = 0; i < num; i++) {
    puth(*addr++, 2);
    putc(L' ');
  }
}
