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
