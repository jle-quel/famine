#include <famine.h>

void __attribute__((constructor)) constructor(void)
{
	dprintf(0x1, "Hello World\n");
}

int main(VOID int ac, VOID char **av)
{
	return 0;
}
