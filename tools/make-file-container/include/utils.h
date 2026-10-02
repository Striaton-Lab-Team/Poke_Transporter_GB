#ifndef _UTILS_H
#define _UTILS_H

#include <cstdint>
#include <cstdio>

extern const bool g_isLittleEndian;

enum class Endianness
{
    LITTLE,
    BIG
};

void extractFilenameAndExtension(const char *path, char *outFilename, char *outExtension = nullptr, uint16_t extensionBufferSize = 0);

#endif