#ifndef _FB_H_
#define _FB_H_

void init_fb(void);

struct fb {
	unsigned long long base;
	unsigned long long size;
	unsigned int hr;
	unsigned int vr;
};

extern struct fb fb;

#endif
