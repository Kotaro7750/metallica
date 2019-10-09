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
