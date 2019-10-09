#include "../include/common.h"
#include "../include/config.h"
#include "../include/efi.h"

#define MAX_STR_BUF	100

#ifdef OUTPUT_DISABLE
void puts(unsigned short *s __attribute__((unused)))
{
	return;
}
#else
void puts(unsigned short *s)
{
	ST->ConOut->OutputString(ST->ConOut, s);
}
#endif
void puth(unsigned long long val, unsigned char num_digits)
{
	int i;
	unsigned short unicode_val;
	unsigned short str[MAX_STR_BUF];

	for (i = num_digits - 1; i >= 0; i--) {
		unicode_val = (unsigned short)(val & 0x0f);
		if (unicode_val < 0xa)
			str[i] = L'0' + unicode_val;
		else
			str[i] = L'A' + (unicode_val - 0xa);
		val >>= 4;
	}
	str[num_digits] = L'\0';

	puts(str);
}

unsigned char check_warn_error(unsigned long long status, unsigned short *message)
{
	if (status) {
		puts(message);
		puts(L":");
		puth(status, 16);
		puts(L"\r\n");
	}

	return !status;
}

void put_param(unsigned short *name, unsigned long long val)
{
	puts(name);
	puts(L": 0x");
	puth(val, 16);
	puts(L"\r\n");
}

void assert(unsigned long long status, unsigned short *message)
{
	if (!check_warn_error(status, message))
		while (1);
}
