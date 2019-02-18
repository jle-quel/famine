#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

struct s_info get_info(const char *file)
{
	struct s_info info;
	struct stat statbuf;

	info.name = NULL;
	
	if (_stat(file, &statbuf) < 0)
		return info;
	if (statbuf.st_size == 0)
		return info;

	if ((info.fd = _open(file, O_RDWR, 0644)) < 0)
		return info;

	if ((info.ptr = _mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, info.fd, 0)) == MAP_FAILED)
	{
		_close(info.fd);
		return info;
	}

	info.size = statbuf.st_size;
	info.name = file;

	return info;
}

void release_info(struct s_info *info)
{
	_close(info->fd);
	_munmap(info->ptr, info->size);
}
