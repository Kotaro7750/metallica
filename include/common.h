#ifndef _COMMON_H_
#define _COMMON_H_

#define NULL (void *)0
#define TRUE 1
#define FALSE 0

void putc(unsigned short c);
void put_param(unsigned short *name, unsigned long long val);
void puts(unsigned short *s);
unsigned char check_warn_error(unsigned long long status,
                               unsigned short *message);
void puth(unsigned long long val, unsigned char num_digits);
void assert(unsigned long long status, unsigned short *message);
#endif
