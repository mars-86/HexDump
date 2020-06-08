#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hex_dump.h"
#include "file_manip.h"

int main(int argc, char* argv[])
{
    FILEBUF* fb = NULL;
    FILE* fd = NULL;

    //const char* fname = "F:\\Programacion\\Cpp\\GitHub\\Mario_Tennis_Open_(USA)_Decrypted.3ds";
    const char* fname = "F:\\Programacion\\Cpp\\GitHub\\openvpn-Digi_FW_GW-TCP4-1194-install-2.4.6-I602.7z";
    unsigned char *fbuff = (unsigned char*)malloc(HEX_DUMP_FILE_CHUNK_128K * sizeof(unsigned char));

    //if ((open_file_fb(&fb, fname, FILE_MANIP_READ)) != 0)
        //printf("ERROR\n");

    if ((open_file(&fd, fname, FILE_MANIP_READ)) != 0)
        printf("ERROR\n");

    long long curr_pos = 0;
    int i;

    while (curr_pos != EOF)
        curr_pos = hex_dump(fd, curr_pos, fbuff, HEX_DUMP_FILE_CHUNK_128K);

    close_file(fd);
    //close_file_fb(fb);

    free(fbuff);

    return 0;
}
