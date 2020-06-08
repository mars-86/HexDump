#ifndef FILE_MANIP_H
#define FILE_MANIP_H
#pragma once

#include <stdio.h>

typedef struct _FILEBUF FILEBUF;
typedef struct FILEBUF** FILEBUFL;

enum _FILE_MANIP_MODES {
	FILE_MANIP_READ,
	FILE_MANIP_WRITE,
	FILE_MANIP_APPEND
};

int open_file(FILE** fd, const char* path, const char* mode);
int open_file_fb(FILEBUF** fb, const char* path, int mode);
int is_file_open(FILE* fd);
int is_file_open_fb(FILEBUF* fb);
size_t file_size(FILE* fd);
size_t file_size_fb(FILEBUF* fb);
const char file_mode_fb(FILEBUF* fb);
const char file_type_fb(FILEBUF* fb);
void* read_file(char* buff, long buff_size);
long long read_binary_file(FILE* fd, unsigned char* fbuff, long buff_size);
void* find_in_file(const char* search);
int close_file(FILE* fd);
int close_file_fb(FILEBUF* fb);

#endif // !FILE_MANIP_H