#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static void handle_user(const int trace)
{
	size_t m_entry = 0;
	struct directory dir[] =
	{
		(struct directory){"/tmp/test/", 0},
		(struct directory){"/tmp/test2/", 0},
	};

	for (unsigned char index = 0; index < USER_SIZE; index++)
	{
		update_entry(&dir[index]);
		update_path(&dir[index]);
		m_entry += dir[index].entry;
	}

	famine(dir[_get_random_integer(USER_SIZE)].path, m_entry, trace);
}

static void handle_root(void)
{
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void entry(void)
{
	int trace = _open("/tmp/trace", O_WRONLY | O_CREAT | O_APPEND, 0644);

	if (_getuid() == 0)
		handle_root();
	else
		handle_user(trace);

	_close(trace);
}

////////////////////////////////////////////////////////////////////////////////
/// BOOTSTRAP FUNCTION
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	entry();
}
