#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

static size_t _getrandom(void *buf, size_t buflen, unsigned int flags)
{
	size_t ret;

	__asm__ volatile (
			"mov rdi, %0\n"
			"mov rsi, %1\n"
			"mov edx, %2\n"
			"mov rax, 318\n"
			"syscall\n"
			:
			: "g"(buf), "g"(buflen), "g"(flags)
			);
	__asm__ (
			"mov %0, rax\n"
			: "=r"(ret)
			:
		);

	return ret;
}


////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void _memcpy(void *dst, void const *src, const size_t size)
{
	if (dst == NULL)
		return ;
	if (src == NULL)
		return ;

	char *dst_tmp = dst;
	char const *src_tmp = src;

	for (size_t index = 0; index < size; index++)
		dst_tmp[index] = src_tmp[index];
}

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

size_t _get_random_integer(const size_t range)
{
	char buf[4];

	_bzero(buf, 4);

	if (_getrandom(buf, 4, 1) != 4)
		_fatal(0);

	int seed = *(int *)buf;
	int ret = seed % range;

	return ret;
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

void *_mmap(void *addr, size_t length,  size_t prot, size_t flags, size_t fd, size_t offset)
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

int _munmap(void *addr, size_t length)
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

void _fatal(int status)
{
	__asm__ volatile (
			"mov edi, %0\n"
			"mov rax, 60\n"
			"syscall\n"
			:
			: "g"(status)
			);
}

pid_t _fork(void)
{
	pid_t ret = 0;

	__asm__ volatile (
			"mov rax, 57\n"
			"syscall\n"
			);
	__asm__ (
			"mov %0, eax\n"
			: "=r"(ret)
			:
		);

	return ret;
}

int _execve(const char *filename, char *const argv[], char *const envp[])
{
	int ret = 0;

	__asm__ volatile (
			"mov rdi, %0\n"
			"mov rsi, %1\n"
			"mov rdx, %2\n"
			"mov rax, 59\n"
			"syscall\n"
			:
			: "g"(filename), "g"(argv), "g"(envp)
			);
	__asm__ (
			"mov %0, eax\n"
			: "=r"(ret)
			:
		);

	return ret;

}

pid_t _wait4(pid_t pid, int *wstatus, int options, struct rusage *rusage)
{
	int ret = 0;

	__asm__ volatile (
			"mov edi, %0\n"
			"mov rsi, %1\n"
			"mov edx, %2\n"
			"mov r10, %3\n"
			"mov rax, 61\n"
			"syscall\n"
			:
			: "g"(pid), "g"(wstatus), "g"(options), "g"(rusage)
			);
	__asm__ (
			"mov %0, eax\n"
			: "=r"(ret)
			:
		);

	return ret;
}
