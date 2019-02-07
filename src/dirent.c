#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void update_buf(char *buf, const char *path)
{
	int fd = 0;
	int index = 0;
	int limit = 0;
	struct linux_dirent64 *dirp;

	if ((fd = _open(path, O_RDONLY | O_DIRECTORY, 0000)) < 0)
		Exit(0);

	while ((limit = _getdents64(fd, (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		while (index < limit)
		{
			dirp = (struct linux_dirent64 *)(buf + index);
			index += dirp->d_reclen;
		}
	}
     
	_close(fd);

}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void update_dirent(char *dirent, const char *path)
{
	char buf[BUFF_SIZE];
	size_t d_index = 0;
	size_t b_index = 0;
	int reclen = 0;
	const size_t size = sizeof(struct linux_dirent64);

	_bzero(buf, BUFF_SIZE);
	_bzero(dirent, BUFF_SIZE);

	update_buf(buf, path);

	while (buf[b_index] != 0)
	{
		reclen = ((struct linux_dirent64 *)(buf + b_index))->d_reclen;

		if (((struct linux_dirent64 *)(buf + b_index))->d_name[0] != '.')
		{
			for (size_t index = 0; index < size; index++)
			{
				dirent[d_index] = buf[b_index + index];
				d_index += 1;
			}
			d_index += PADDING;
		}
		b_index += reclen;
	}
}

