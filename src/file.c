#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

struct elf get_file(const char *str)
{
	struct stat statbuf;
	struct elf ret;

	ret.size = 0;

	if ((ret.fd = _open(str, O_RDWR, 0000)) < 0)
	{
		ERR(str);
		return ret;
	}
	if (_stat(str, &statbuf) < 0)
	{
		ERR(str);
		return ret;
	}
	if ((ret.ptr = _mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, ret.fd, 0)) == MAP_FAILED)
	{
		ERR("NULL");
		return ret;
	}

	ret.size = statbuf.st_size;

	return ret;
}

void release_file(struct elf *file)
{
	_close(file->fd);
	_munmap(file->ptr, file->size);
}
