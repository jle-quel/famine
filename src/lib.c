#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

size_t _strlen(const char *str)
{
	size_t ret = 0;

	if (str == NULL)
		return 0;

	while (str[ret])
		ret++;

	return ret;
}

void _bzero(char *str, const size_t size)
{
	if (str == NULL)
		return ;

	for (size_t index = 0; index < size; index++)
		str[index] = 0;
}

size_t get_random_integer(const size_t range)
{
	size_t seed = 0;

	return ((size_t)(&seed) >> 8) % range;
}

int _open(const char *pathname, int flags, long mode)
{
	int ret;

	__asm__ volatile (
			"mov rdi, %0\n"
			"mov esi, %1\n"
			"mov rdx, %2\n"
			"mov rax, 2\n"
			"syscall\n"
			:
			: "g"(pathname), "g"(flags), "g"(mode)
			);
	__asm__ (
			"mov %0, eax\n"
			: "=r"(ret)
			:
		);

	return ret;
}

int _close(int fd)
{
	int ret;

	__asm__ volatile (
			"mov edi, %0\n"
			"mov rax, 3\n"
			"syscall\n"
			:
			: "g"(fd)
			);
	__asm__ (
			"mov %0, eax\n"
			: "=r"(ret)
			:
		);

	return ret;
}

int _write(int fd, const void *buf, long count)
{
	int ret;

	__asm__ volatile (
			"mov edi, %0\n"
			"mov rsi, %1\n"
			"mov rdx, %2\n"
			"mov rax, 1\n"
			"syscall\n"
			:
			: "g"(fd), "g"(buf), "g"(count)
		);
	__asm__ (
			"mov %0, eax\n"
			: "=r"(ret)
			:
		);

        return ret;
}

int _getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count)
{
	int ret;

	__asm__ volatile (
			"mov edi, %0\n"
			"mov rsi, %1\n"
			"mov edx, %2\n"
			"mov rax, 217\n"
			"syscall\n"
			:
			: "g"(fd), "g"(dirp), "g"(count)
			);
	__asm__ (
			"mov %0, eax\n"
			: "=r"(ret)
			:
		);

	return ret;
}

int _stat(const char *pathname, struct stat *statbuf)
{
	int ret;

	__asm__ volatile (
			"mov rdi, %0\n"
			"mov rsi, %1\n"
			"mov rax, 4\n"
			"syscall\n"
			:
			: "g"(pathname), "g"(statbuf)
			);
	__asm__ (
			"mov %0, eax\n"
			: "=r"(ret)
			:
		);

	return ret;
}

int _getuid(void)
{
	int ret;

	__asm__ volatile (
			"mov rax, 102\n"
			"syscall\n"
			);
	__asm__ (
			"mov %0, eax\n"
			: "=r"(ret)
			:
		);

	return ret;
}

void *_mmap(void *addr, unsigned long length,  unsigned long prot, unsigned long flags, unsigned long fd, unsigned long offset)
{
	void *ret;

	__asm__ volatile (
			"mov rdi, %0\n"
			"mov rsi, %1\n"
			"mov rdx, %2\n"
			"mov r10, %3\n"
			"mov r9, %4\n"
			"mov r9, %5\n"
			"mov rax, 9\n"
			"syscall\n"
			:
			: "g"(addr), "g"(length), "g"(prot), "g"(flags), "g"(fd), "g"(offset)
			);
	__asm__ (
			"mov %0, rax\n"
			: "=r"(ret)
			:
		);

	return ret;
}

int _munmap(void *addr, unsigned long length)
{
	int ret = 0;

	__asm__ volatile (
			"mov rdi, %0\n"
			"mov rsi, %1\n"
			"mov rax, 11\n"
			"syscall\n"
			:
			: "g"(addr), "g"(length)
			);
	__asm__ (
			"mov %0, eax\n"
			: "=r"(ret)
			:
		);

	return ret;
}
