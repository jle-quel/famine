#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

struct s_info get_info(const char *str)
{
	FUNC;

	struct s_info info;
	struct stat statbuf;

	if ((info.fd = _open(str, O_RDWR, 0000)) < 0)
	{
		ERR;
		exit(0);
	}
	if (_stat(str, &statbuf) < 0)
	{
		ERR;
		exit(0);
	}
	if ((info.ptr = _mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, info.fd, 0)) == MAP_FAILED)
	{
		ERR;
		exit(0);
	}

	info.size = statbuf.st_size;

	return info;
}

void release_info(struct s_info *info)
{
	_close(info->fd);
	_munmap(info->ptr, info->size);
}
