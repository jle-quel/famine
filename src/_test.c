#include <famine.h>

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

	char buf[] = "Hello\n";
	
	__asm__ volatile (
			"mov rdi, 1\n"
			"mov rsi, %0\n"
			"mov rdx, 6\n"
			"mov rax, 1\n"
			"syscall\n"
			:
			: "g"(buf)
			);

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
			"leave\n"
			"jmp 0xffffffffdeedcef0\n"
			);
}

