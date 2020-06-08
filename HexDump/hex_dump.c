#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hex_dump.h"
#include "file_manip.h"

// chars from 0 to 32 are specials (i.e. NUL, NL, TAB)
#define HEX_DUMP_EVAL_ASCII_IS_SPECIAL(c) (c & 0xE0 ? c : '.' )
#define EVAL_ASCII(c) c & 0x80 ? '.' : HEX_DUMP_EVAL_ASCII_IS_SPECIAL(c)

const char* output_fmt =
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |"
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |"
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |"
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |"
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |"
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |"
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |"
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |"
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |"
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |"
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |"
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |"
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |"
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |"
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |"
    "\n%8x    %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x    | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c |";

long long print_char(long long fpos, const unsigned char* chunk, size_t bytes_read);
long long print_chunk(long long fpos, const unsigned char* chunk, size_t bytes_read);

long long hex_dump(FILE *fd, long long fpos, unsigned char* chunk, size_t csize)
{
    long long bytes_read, curr_pos;

    memset(chunk, 0, csize * sizeof(chunk[0]));
    bytes_read = read_binary_file(fd, chunk, csize);

    curr_pos = (bytes_read < csize) ? print_char(fpos, chunk, bytes_read) : print_chunk(fpos, chunk, bytes_read);

    return (feof(fd)) ? EOF : curr_pos;
}

long long print_char(long long fpos, const unsigned char* chunk, size_t bytes_read)
{
    int i;
    long long j;
    for (i = 0, j = fpos; i < bytes_read; ++i, ++j){
        if (i & 0xF) // mask -> (i % 16)
            fprintf(stdout, "%2x ", chunk[i]);
        else
            fprintf(stdout, "\n%8x    %2x ", j, chunk[i]);
    }
    return j;
}

long long print_chunk(long long fpos, const unsigned char* chunk, size_t bytes_read)
{
    int i;
    long long j;
    for (i = 0, j = fpos; i < bytes_read; i += 256, j += 256)
        fprintf(
            stdout,
            output_fmt,
            j,
            chunk[i + 0], chunk[i + 1], chunk[i + 2], chunk[i + 3], chunk[i + 4], chunk[i + 5], chunk[i + 6], chunk[i + 7],
            chunk[i + 8], chunk[i + 9], chunk[i + 10], chunk[i + 11], chunk[i + 12], chunk[i + 13], chunk[i + 14], chunk[i + 15],
            EVAL_ASCII(chunk[i + 0]), EVAL_ASCII(chunk[i + 1]), EVAL_ASCII(chunk[i + 2]), EVAL_ASCII(chunk[i + 3]), EVAL_ASCII(chunk[i + 4]), EVAL_ASCII(chunk[i + 5]), EVAL_ASCII(chunk[i + 6]), EVAL_ASCII(chunk[i + 7]),
            EVAL_ASCII(chunk[i + 8]), EVAL_ASCII(chunk[i + 9]), EVAL_ASCII(chunk[i + 10]), EVAL_ASCII(chunk[i + 11]), EVAL_ASCII(chunk[i + 12]), EVAL_ASCII(chunk[i + 13]), EVAL_ASCII(chunk[i + 14]), EVAL_ASCII(chunk[i + 15]),
            j + 16,
            chunk[i + 16], chunk[i + 17], chunk[i + 18], chunk[i + 19], chunk[i + 20], chunk[i + 21], chunk[i + 22], chunk[i + 23],
            chunk[i + 24], chunk[i + 25], chunk[i + 26], chunk[i + 27], chunk[i + 28], chunk[i + 29], chunk[i + 30], chunk[i + 31],
            EVAL_ASCII(chunk[i + 16]), EVAL_ASCII(chunk[i + 17]), EVAL_ASCII(chunk[i + 18]), EVAL_ASCII(chunk[i + 19]), EVAL_ASCII(chunk[i + 20]), EVAL_ASCII(chunk[i + 21]), EVAL_ASCII(chunk[i + 22]), EVAL_ASCII(chunk[i + 23]),
            EVAL_ASCII(chunk[i + 24]), EVAL_ASCII(chunk[i + 25]), EVAL_ASCII(chunk[i + 26]), EVAL_ASCII(chunk[i + 27]), EVAL_ASCII(chunk[i + 28]), EVAL_ASCII(chunk[i + 29]), EVAL_ASCII(chunk[i + 30]), EVAL_ASCII(chunk[i + 31]),
            j + 32,
            chunk[i + 32], chunk[i + 33], chunk[i + 34], chunk[i + 35], chunk[i + 36], chunk[i + 37], chunk[i + 38], chunk[i + 39],
            chunk[i + 40], chunk[i + 41], chunk[i + 42], chunk[i + 43], chunk[i + 44], chunk[i + 45], chunk[i + 46], chunk[i + 47],
            EVAL_ASCII(chunk[i + 32]), EVAL_ASCII(chunk[i + 33]), EVAL_ASCII(chunk[i + 34]), EVAL_ASCII(chunk[i + 35]), EVAL_ASCII(chunk[i + 36]), EVAL_ASCII(chunk[i + 37]), EVAL_ASCII(chunk[i + 38]), EVAL_ASCII(chunk[i + 39]),
            EVAL_ASCII(chunk[i + 40]), EVAL_ASCII(chunk[i + 41]), EVAL_ASCII(chunk[i + 42]), EVAL_ASCII(chunk[i + 43]), EVAL_ASCII(chunk[i + 44]), EVAL_ASCII(chunk[i + 45]), EVAL_ASCII(chunk[i + 46]), EVAL_ASCII(chunk[i + 47]),
            j + 48,
            chunk[i + 48], chunk[i + 49], chunk[i + 50], chunk[i + 51], chunk[i + 52], chunk[i + 53], chunk[i + 54], chunk[i + 55],
            chunk[i + 56], chunk[i + 57], chunk[i + 58], chunk[i + 59], chunk[i + 60], chunk[i + 61], chunk[i + 62], chunk[i + 63],
            EVAL_ASCII(chunk[i + 48]), EVAL_ASCII(chunk[i + 49]), EVAL_ASCII(chunk[i + 50]), EVAL_ASCII(chunk[i + 51]), EVAL_ASCII(chunk[i + 52]), EVAL_ASCII(chunk[i + 53]), EVAL_ASCII(chunk[i + 54]), EVAL_ASCII(chunk[i + 55]),
            EVAL_ASCII(chunk[i + 56]), EVAL_ASCII(chunk[i + 57]), EVAL_ASCII(chunk[i + 58]), EVAL_ASCII(chunk[i + 59]), EVAL_ASCII(chunk[i + 60]), EVAL_ASCII(chunk[i + 61]), EVAL_ASCII(chunk[i + 62]), EVAL_ASCII(chunk[i + 63]),
            j + 64,
            chunk[i + 64], chunk[i + 65], chunk[i + 66], chunk[i + 67], chunk[i + 68], chunk[i + 69], chunk[i + 70], chunk[i + 71],
            chunk[i + 72], chunk[i + 73], chunk[i + 74], chunk[i + 75], chunk[i + 76], chunk[i + 77], chunk[i + 78], chunk[i + 79],
            EVAL_ASCII(chunk[i + 64]), EVAL_ASCII(chunk[i + 65]), EVAL_ASCII(chunk[i + 66]), EVAL_ASCII(chunk[i + 67]), EVAL_ASCII(chunk[i + 68]), EVAL_ASCII(chunk[i + 69]), EVAL_ASCII(chunk[i + 70]), EVAL_ASCII(chunk[i + 71]),
            EVAL_ASCII(chunk[i + 72]), EVAL_ASCII(chunk[i + 73]), EVAL_ASCII(chunk[i + 74]), EVAL_ASCII(chunk[i + 75]), EVAL_ASCII(chunk[i + 76]), EVAL_ASCII(chunk[i + 77]), EVAL_ASCII(chunk[i + 78]), EVAL_ASCII(chunk[i + 79]),
            j + 80,
            chunk[i + 80], chunk[i + 81], chunk[i + 82], chunk[i + 83], chunk[i + 84], chunk[i + 85], chunk[i + 86], chunk[i + 87],
            chunk[i + 88], chunk[i + 89], chunk[i + 90], chunk[i + 91], chunk[i + 92], chunk[i + 93], chunk[i + 94], chunk[i + 95],
            EVAL_ASCII(chunk[i + 80]), EVAL_ASCII(chunk[i + 81]), EVAL_ASCII(chunk[i + 82]), EVAL_ASCII(chunk[i + 83]), EVAL_ASCII(chunk[i + 84]), EVAL_ASCII(chunk[i + 85]), EVAL_ASCII(chunk[i + 86]), EVAL_ASCII(chunk[i + 87]),
            EVAL_ASCII(chunk[i + 88]), EVAL_ASCII(chunk[i + 89]), EVAL_ASCII(chunk[i + 90]), EVAL_ASCII(chunk[i + 91]), EVAL_ASCII(chunk[i + 92]), EVAL_ASCII(chunk[i + 93]), EVAL_ASCII(chunk[i + 94]), EVAL_ASCII(chunk[i + 95]),
            j + 96,
            chunk[i + 96], chunk[i + 97], chunk[i + 98], chunk[i + 99], chunk[i + 100], chunk[i + 101], chunk[i + 102], chunk[i + 103],
            chunk[i + 104], chunk[i + 105], chunk[i + 106], chunk[i + 107], chunk[i + 108], chunk[i + 109], chunk[i + 110], chunk[i + 111],
            EVAL_ASCII(chunk[i + 96]), EVAL_ASCII(chunk[i + 97]), EVAL_ASCII(chunk[i + 98]), EVAL_ASCII(chunk[i + 99]), EVAL_ASCII(chunk[i + 100]), EVAL_ASCII(chunk[i + 101]), EVAL_ASCII(chunk[i + 102]), EVAL_ASCII(chunk[i + 103]),
            EVAL_ASCII(chunk[i + 104]), EVAL_ASCII(chunk[i + 105]), EVAL_ASCII(chunk[i + 106]), EVAL_ASCII(chunk[i + 107]), EVAL_ASCII(chunk[i + 108]), EVAL_ASCII(chunk[i + 109]), EVAL_ASCII(chunk[i + 110]), EVAL_ASCII(chunk[i + 111]),
            j + 112,
            chunk[i + 112], chunk[i + 113], chunk[i + 114], chunk[i + 115], chunk[i + 116], chunk[i + 117], chunk[i + 118], chunk[i + 119],
            chunk[i + 120], chunk[i + 121], chunk[i + 122], chunk[i + 123], chunk[i + 124], chunk[i + 125], chunk[i + 126], chunk[i + 127],
            EVAL_ASCII(chunk[i + 112]), EVAL_ASCII(chunk[i + 113]), EVAL_ASCII(chunk[i + 114]), EVAL_ASCII(chunk[i + 115]), EVAL_ASCII(chunk[i + 116]), EVAL_ASCII(chunk[i + 117]), EVAL_ASCII(chunk[i + 118]), EVAL_ASCII(chunk[i + 119]),
            EVAL_ASCII(chunk[i + 120]), EVAL_ASCII(chunk[i + 121]), EVAL_ASCII(chunk[i + 122]), EVAL_ASCII(chunk[i + 123]), EVAL_ASCII(chunk[i + 124]), EVAL_ASCII(chunk[i + 125]), EVAL_ASCII(chunk[i + 126]), EVAL_ASCII(chunk[i + 127]),
            j + 128,
            chunk[i + 128], chunk[i + 129], chunk[i + 130], chunk[i + 131], chunk[i + 132], chunk[i + 133], chunk[i + 134], chunk[i + 135],
            chunk[i + 136], chunk[i + 137], chunk[i + 138], chunk[i + 139], chunk[i + 140], chunk[i + 141], chunk[i + 142], chunk[i + 143],
            EVAL_ASCII(chunk[i + 128]), EVAL_ASCII(chunk[i + 129]), EVAL_ASCII(chunk[i + 130]), EVAL_ASCII(chunk[i + 131]), EVAL_ASCII(chunk[i + 132]), EVAL_ASCII(chunk[i + 133]), EVAL_ASCII(chunk[i + 134]), EVAL_ASCII(chunk[i + 135]),
            EVAL_ASCII(chunk[i + 136]), EVAL_ASCII(chunk[i + 137]), EVAL_ASCII(chunk[i + 138]), EVAL_ASCII(chunk[i + 139]), EVAL_ASCII(chunk[i + 140]), EVAL_ASCII(chunk[i + 141]), EVAL_ASCII(chunk[i + 142]), EVAL_ASCII(chunk[i + 143]),
            j + 144,
            chunk[i + 144], chunk[i + 145], chunk[i + 146], chunk[i + 147], chunk[i + 148], chunk[i + 149], chunk[i + 150], chunk[i + 151],
            chunk[i + 152], chunk[i + 153], chunk[i + 154], chunk[i + 155], chunk[i + 156], chunk[i + 157], chunk[i + 158], chunk[i + 159],
            EVAL_ASCII(chunk[i + 144]), EVAL_ASCII(chunk[i + 145]), EVAL_ASCII(chunk[i + 146]), EVAL_ASCII(chunk[i + 147]), EVAL_ASCII(chunk[i + 148]), EVAL_ASCII(chunk[i + 149]), EVAL_ASCII(chunk[i + 150]), EVAL_ASCII(chunk[i + 151]),
            EVAL_ASCII(chunk[i + 152]), EVAL_ASCII(chunk[i + 153]), EVAL_ASCII(chunk[i + 154]), EVAL_ASCII(chunk[i + 155]), EVAL_ASCII(chunk[i + 156]), EVAL_ASCII(chunk[i + 157]), EVAL_ASCII(chunk[i + 158]), EVAL_ASCII(chunk[i + 159]),
            j + 160,
            chunk[i + 160], chunk[i + 161], chunk[i + 162], chunk[i + 163], chunk[i + 164], chunk[i + 165], chunk[i + 166], chunk[i + 167],
            chunk[i + 168], chunk[i + 169], chunk[i + 170], chunk[i + 171], chunk[i + 172], chunk[i + 173], chunk[i + 174], chunk[i + 175],
            EVAL_ASCII(chunk[i + 160]), EVAL_ASCII(chunk[i + 161]), EVAL_ASCII(chunk[i + 162]), EVAL_ASCII(chunk[i + 163]), EVAL_ASCII(chunk[i + 164]), EVAL_ASCII(chunk[i + 165]), EVAL_ASCII(chunk[i + 166]), EVAL_ASCII(chunk[i + 167]),
            EVAL_ASCII(chunk[i + 168]), EVAL_ASCII(chunk[i + 169]), EVAL_ASCII(chunk[i + 170]), EVAL_ASCII(chunk[i + 171]), EVAL_ASCII(chunk[i + 172]), EVAL_ASCII(chunk[i + 173]), EVAL_ASCII(chunk[i + 174]), EVAL_ASCII(chunk[i + 175]),
            j + 176,
            chunk[i + 176], chunk[i + 177], chunk[i + 178], chunk[i + 179], chunk[i + 180], chunk[i + 181], chunk[i + 182], chunk[i + 183],
            chunk[i + 184], chunk[i + 185], chunk[i + 186], chunk[i + 187], chunk[i + 188], chunk[i + 189], chunk[i + 190], chunk[i + 191],
            EVAL_ASCII(chunk[i + 176]), EVAL_ASCII(chunk[i + 177]), EVAL_ASCII(chunk[i + 178]), EVAL_ASCII(chunk[i + 179]), EVAL_ASCII(chunk[i + 180]), EVAL_ASCII(chunk[i + 181]), EVAL_ASCII(chunk[i + 182]), EVAL_ASCII(chunk[i + 183]),
            EVAL_ASCII(chunk[i + 184]), EVAL_ASCII(chunk[i + 185]), EVAL_ASCII(chunk[i + 186]), EVAL_ASCII(chunk[i + 187]), EVAL_ASCII(chunk[i + 188]), EVAL_ASCII(chunk[i + 189]), EVAL_ASCII(chunk[i + 190]), EVAL_ASCII(chunk[i + 191]),
            j + 192,
            chunk[i + 192], chunk[i + 193], chunk[i + 194], chunk[i + 195], chunk[i + 196], chunk[i + 197], chunk[i + 198], chunk[i + 199],
            chunk[i + 200], chunk[i + 201], chunk[i + 202], chunk[i + 203], chunk[i + 204], chunk[i + 205], chunk[i + 206], chunk[i + 207],
            EVAL_ASCII(chunk[i + 192]), EVAL_ASCII(chunk[i + 193]), EVAL_ASCII(chunk[i + 194]), EVAL_ASCII(chunk[i + 195]), EVAL_ASCII(chunk[i + 196]), EVAL_ASCII(chunk[i + 197]), EVAL_ASCII(chunk[i + 198]), EVAL_ASCII(chunk[i + 199]),
            EVAL_ASCII(chunk[i + 200]), EVAL_ASCII(chunk[i + 201]), EVAL_ASCII(chunk[i + 202]), EVAL_ASCII(chunk[i + 203]), EVAL_ASCII(chunk[i + 204]), EVAL_ASCII(chunk[i + 205]), EVAL_ASCII(chunk[i + 206]), EVAL_ASCII(chunk[i + 207]),
            j + 208,
            chunk[i + 208], chunk[i + 209], chunk[i + 210], chunk[i + 211], chunk[i + 212], chunk[i + 213], chunk[i + 214], chunk[i + 215],
            chunk[i + 216], chunk[i + 217], chunk[i + 218], chunk[i + 219], chunk[i + 220], chunk[i + 221], chunk[i + 222], chunk[i + 223],
            EVAL_ASCII(chunk[i + 208]), EVAL_ASCII(chunk[i + 209]), EVAL_ASCII(chunk[i + 210]), EVAL_ASCII(chunk[i + 211]), EVAL_ASCII(chunk[i + 212]), EVAL_ASCII(chunk[i + 213]), EVAL_ASCII(chunk[i + 214]), EVAL_ASCII(chunk[i + 215]),
            EVAL_ASCII(chunk[i + 216]), EVAL_ASCII(chunk[i + 217]), EVAL_ASCII(chunk[i + 218]), EVAL_ASCII(chunk[i + 219]), EVAL_ASCII(chunk[i + 220]), EVAL_ASCII(chunk[i + 221]), EVAL_ASCII(chunk[i + 222]), EVAL_ASCII(chunk[i + 223]),
            j + 224,
            chunk[i + 224], chunk[i + 225], chunk[i + 226], chunk[i + 227], chunk[i + 228], chunk[i + 229], chunk[i + 230], chunk[i + 231],
            chunk[i + 232], chunk[i + 233], chunk[i + 234], chunk[i + 235], chunk[i + 236], chunk[i + 237], chunk[i + 238], chunk[i + 239],
            EVAL_ASCII(chunk[i + 224]), EVAL_ASCII(chunk[i + 225]), EVAL_ASCII(chunk[i + 226]), EVAL_ASCII(chunk[i + 227]), EVAL_ASCII(chunk[i + 228]), EVAL_ASCII(chunk[i + 229]), EVAL_ASCII(chunk[i + 230]), EVAL_ASCII(chunk[i + 231]),
            EVAL_ASCII(chunk[i + 232]), EVAL_ASCII(chunk[i + 233]), EVAL_ASCII(chunk[i + 234]), EVAL_ASCII(chunk[i + 235]), EVAL_ASCII(chunk[i + 236]), EVAL_ASCII(chunk[i + 237]), EVAL_ASCII(chunk[i + 238]), EVAL_ASCII(chunk[i + 239]),
            j + 240,
            chunk[i + 240], chunk[i + 241], chunk[i + 242], chunk[i + 243], chunk[i + 244], chunk[i + 245], chunk[i + 246], chunk[i + 247],
            chunk[i + 248], chunk[i + 249], chunk[i + 250], chunk[i + 251], chunk[i + 252], chunk[i + 253], chunk[i + 254], chunk[i + 255],
            EVAL_ASCII(chunk[i + 240]), EVAL_ASCII(chunk[i + 241]), EVAL_ASCII(chunk[i + 242]), EVAL_ASCII(chunk[i + 243]), EVAL_ASCII(chunk[i + 244]), EVAL_ASCII(chunk[i + 245]), EVAL_ASCII(chunk[i + 246]), EVAL_ASCII(chunk[i + 247]),
            EVAL_ASCII(chunk[i + 248]), EVAL_ASCII(chunk[i + 249]), EVAL_ASCII(chunk[i + 250]), EVAL_ASCII(chunk[i + 251]), EVAL_ASCII(chunk[i + 252]), EVAL_ASCII(chunk[i + 253]), EVAL_ASCII(chunk[i + 254]), EVAL_ASCII(chunk[i + 255])
        );

    return j;
}