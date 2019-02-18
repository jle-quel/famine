#include <famine.h>

static void label1(void);

void test(void)
{
	__asm__ volatile (
			"push rbx\n"
			"push rsp\n"
			"push rbp\n"
			"push rax\n"
			"push rdi\n"
			"push rsi\n"
			"push rdx\n"
			"push rcx\n"
			"push r8\n"
			"push r9\n"
			"push r10\n"
			"push r11\n"
			"push r12\n"
			"push r13\n"
			"push r14\n"
			"push r15\n"
			);

	label1();

	__asm__ volatile (
			"pop r15\n"
			"pop r14\n"
			"pop r13\n"
			"pop r12\n"
			"pop r11\n"
			"pop r10\n"
			"pop r9\n"
			"pop r8\n"
			"pop rcx\n"
			"pop rdx\n"
			"pop rsi\n"
			"pop rdi\n"
			"pop rax\n"
			"pop rbp\n"
			"pop rsp\n"
			"pop rbx\n"
			);

	__asm__ volatile (
			"add rsp, 8\n"
			"jmp 0xffffffffdeedcef0\n"
			);
}


int _o(const char *pathname, int flags, long mode)
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

int _c(int fd)
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

int _w(int fd, const void *buf, long count)
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

static void label1(void)
{
	int fd = 0;
	char buf[] = "Hello\n";
	char file[] = "lol";

	fd = _o(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	_w(fd, buf, 6);
	_c(fd);
}
