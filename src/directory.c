#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

static inline void update_path(char *path, const char *file)
{
	const size_t p_limit = _strlen(path);
	const size_t f_limit = _strlen(file);

	_bzero(path + p_limit, PATH_MAX - p_limit);

	for (size_t index = 0; index < f_limit; index++)
		path[p_limit + index] = file[index];
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void update_directory(struct directory *dir, const char *dirent)
{
	const size_t size = sizeof(struct linux_dirent64);

	if (dirent == NULL)
		Exit(0);

	for (size_t index = 0; dirent[index]; index += (size + PADDING))
		dir->entry += 1;

	const size_t r_entry = _get_random_integer(dir->entry);
	update_path(dir->path, ((struct linux_dirent64 *)(dirent + ((size + PADDING) * r_entry)))->d_name);
}
