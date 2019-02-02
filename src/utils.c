#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

unsigned long _strlen(const char *str)
{
	unsigned long ret = 0;

	if (str == NULL)
		return 0;

	while (str[ret])
		ret++;

	return ret;
}

void _bzero(char *str, const unsigned long size)
{
	if (str == NULL)
		return ;

	for (unsigned long index = 0; index < size; index++)
		str[index] = 0;
}

int _strcmp(const char *s1, const char *s2)
{
	unsigned long long index = 0;

	if (s1 == NULL)
		return -1;
	if (s2 == NULL)
		return -1;

	while (s1[index])
	{
		if (s1[index] != s2[index])
			return s1[index] - s2[index];
		index++;
	}

	return 0;
}
