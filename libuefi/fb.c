#include "../include/fb.h"
#include "../include/efi.h"

struct fb fb;

void init_fb(void){
  fb.base = GOP->Mode->FrameBufferBase;
  fb.size = GOP->Mode->FrameBufferSize;
  fb.hr = GOP->Mode->Info->HorizontalResolution;
  fb.vr = GOP->Mode->Info->VerticalResolution;
}
