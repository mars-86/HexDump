#ifndef HEX_DUMP_H
#define HEX_DUMP_H
#pragma once

#define HEX_DUMP_FILE_CHUNK_128K 0x20000 // 131072
#define HEX_DUMP_FILE_CHUNK_256K 0x40000 // 262144
#define HEX_DUMP_FILE_CHUNK_500K 0x80000 // 524288
#define HEX_DUMP_FILE_CHUNK_1M 0x100000  // 1048576

long long hex_dump(FILE *fd, long long fpos, unsigned char* chunk, size_t csize);

#endif // !HEX_DUMP_H