#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

static inline void update_to_fullpath(char *path, const char *file)
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

void update_entry(struct directory *dir)
{
	int fd = 0;
	int index = 0;
	int limit = 0;
	char buf[BUFF_SIZE];
	struct linux_dirent64 *dirp;

	if ((fd = _open(dir->path, O_RDONLY | O_DIRECTORY, 0000)) < 0)
	{
		ERR;
		exit(0);
	}

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
}

void update_path(struct directory *dir)
{
	int fd = 0;
	int index = 0;
	int limit = 0;
	char buf[BUFF_SIZE];
	struct linux_dirent64 *dirp;

	size_t entry = 0;
	const size_t r_entry = get_random_integer(dir->entry);
     
	if ((fd = _open(dir->path, O_RDONLY | O_DIRECTORY, 0000)) < 0)
	{
		ERR;
		exit(0);
	}

	while ((limit = _getdents64(fd, (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		while (index < limit)
		{
			dirp = (struct linux_dirent64 *)(buf + index);

			if (entry >= r_entry && dirp->d_name[0] != '.')
				break ;

			index += dirp->d_reclen;
			entry += 1;
		}
	}

	_close(fd);
	update_to_fullpath(dir->path, dirp->d_name);
}
