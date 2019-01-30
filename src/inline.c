#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

int _open(const char *pathname, int flags, long mode)
{
	int ret = 0;

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
	int ret = 0;

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
	int ret = 0;

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
	int ret = 0;

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
	int ret = 0;

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
	int ret = 0;

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
