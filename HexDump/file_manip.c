#include <stdio.h>
#include <stdlib.h>
#include "file_manip.h"

struct _FILEBUF {
    FILE* fd;
    char* name;
    char mode;
    char type;
    size_t size;
};

int _open_file_base(FILE** fd, const char* path, const char* mode)
{
#ifdef _WIN32
    errno_t e;
    if ((e = fopen_s(&(*fd), path, mode)) != 0)
        return e;
#else
    // TODO: return errno_t
    if (!((*fd = fopen(path, mode)))
        return 1;
#endif // __WIN32
    return 0;
}

int open_file(FILE** fd, const char* path, const char* mode)
{
    return _open_file_base(&(*fd), path, "rb");
}

int open_file_fb(FILEBUF** fb, const char* path, int mode)
{
    if (!(*fb = (FILEBUF*)malloc(sizeof(FILEBUF)))) {
        perror("malloc");
        return 1;
    }
    (*fb)->fd = NULL;
    if (_open_file_base(&((*fb)->fd), path, "rb") != 0)
        return 1;
    //fb->name = "";
    (*fb)->type = 'b';
    (*fb)->mode = 'r';
    (*fb)->size = file_size((*fb)->fd);

    return 0;
}

int is_file_open(FILE *fd)
{
    return fd == NULL ? 0 : 1;
}

int is_file_open_fb(FILEBUF* fb)
{
    return fb->fd == NULL ? 0 : 1;
}

size_t file_size(FILE* fd)
{
    long fsize;
    fseek(fd, SEEK_SET, SEEK_END);
    fsize = ftell(fd);
    fseek(fd, SEEK_END, SEEK_SET);
    return fsize;
}

size_t file_size_fb(FILEBUF* fb)
{
    return fb->size;
}

const char file_mode_fb(FILEBUF* fb)
{
    return fb->mode;
}

const char file_type_fb(FILEBUF* fb)
{
    return fb->type;
}

void* read_file(char* buff, long buff_size)
{

}

long long read_binary_file(FILE *fd, unsigned char* fbuff, long buff_size)
{
    int i, c;
    for (i = 0; (c = fgetc(fd)) != EOF && i < buff_size; ++i)
        *(fbuff + i) = (unsigned char)c;
    return i; // bytes read
}

void* find_in_file(const char* search)
{
    return NULL;
}

int close_file(FILE* fd)
{
    return fclose(fd);
}

int close_file_fb(FILEBUF* fb)
{
    if (fclose(fb->fd) != 0) {
        free(fb);
        return -1;
    }
    free(fb);
    return 0;
}