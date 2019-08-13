
#include "itoa.h"
#include <string.h>
#include <circle/types.h>

char *strrev(char *str)
{
    if (!str || ! *str)
        return str;

    int i = strlen(str) - 1, j = 0;

    char ch;
    while (i > j)
    {
        ch = str[i];
        str[i] = str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return str;
}

char* itoa(int num, char* str, int base)
{
	int idx = 0;
	boolean negative = FALSE;

	if (num == 0)
	{
		str[idx++] = '0';
		str[idx] = '\0';
		return str;
	}
	if (num < 0 && base == 10)
	{
		negative = TRUE;
		num = -num;
	}

	while (num != 0)
	{
		int rem = num % base;
		str[idx++] = (rem > 9) ? (rem-10) + 'a' : rem + '0';
		num = num / base;
	}

	if (negative)
		str[idx++] = '-';

	str[idx] = '\0';

	strrev(str);

	return str;

}

char* ltoa(long num, char* str, int base)
{
	long idx = 0;
	boolean negative = FALSE;

	if (num == 0)
	{
		str[idx++] = '0';
		str[idx] = '\0';
		return str;
	}
	if (num < 0 && base == 10)
	{
		negative = TRUE;
		num = -num;
	}

	while (num != 0)
	{
		long rem = num % base;
		str[idx++] = (rem > 9) ? (rem-10) + 'a' : rem + '0';
		num = num / base;
	}

	if (negative)
		str[idx++] = '-';

	str[idx] = '\0';

	strrev(str);

	return str;

}

char* ultoa(unsigned long num, char* str, int base)
{
	unsigned long idx = 0;
	boolean negative = FALSE;

	if (num == 0)
	{
		str[idx++] = '0';
		str[idx] = '\0';
		return str;
	}
	if (base == 10)
	{
		negative = TRUE;
		num = -num;
	}

	while (num != 0)
	{
		unsigned long rem = num % base;
		str[idx++] = (rem > 9) ? (rem-10) + 'a' : rem + '0';
		num = num / base;
	}

	if (negative)
		str[idx++] = '-';

	str[idx] = '\0';

	strrev(str);

	return str;

}