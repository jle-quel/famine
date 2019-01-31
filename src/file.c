#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline void defer(int *fd)
{
	_close(*fd);
}

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

struct elf get_file(const char *str)
{
	__attribute__((cleanup(defer))) int fd = 0;
	struct stat statbuf;
	struct elf ret;

	ret.size = 0;

	if ((fd = _open(str, O_RDONLY, 0000)) == -1)
		return ret;
	if (_stat(str, &statbuf) == -1)
		return ret;
	if ((ret.ptr = _mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return ret;

	ret.size = statbuf.st_size;

	return ret;
}
