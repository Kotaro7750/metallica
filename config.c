#include "include/config.h"
#include "include/common.h"
#include "include/efi.h"

struct config_list conf;

void config_init(void) {
  conf.kernel_start = CONF_DEFAULT_KERNEL_START;
  conf.stack_base = CONF_DEFAULT_STACK_BASE;
  conf.fs_start = CONF_DEFAULT_FS_START;
  conf.enable_ap = CONF_DEFAULT_ENABLE_AP;
  puts(L"Inittializze Config ...DONE");
  puts(L'\0');
  puts(L"\r\n");
}

void load_config(struct EFI_FILE_PROTOCOL *root,
                 unsigned short *conf_file_name) {
  put_param(L"kernel_start", conf.kernel_start);
  put_param(L"stack_base", conf.stack_base);
  put_param(L"fs_start", conf.fs_start);
  put_param(L"enable_ap", conf.enable_ap);
}
