#include <cstdio>
#include <cstdint>
#include <cstring>
#include <unistd.h>

/**
 * @brief This tool strips unnecessary data from BPS patch files:
 * - magic bytes (BPS1)
 * - header fields (source size, target size, metadata size)
 * - CRC values (footer)
 */


static void printUsage()
{
    printf("Usage: bps-patch-trim <file>\n");
}

/// @brief Taken from https://github.com/blakesmith/rombp/blob/master/docs/bps_spec.md
unsigned readVarInt(uint8_t *&buffer)
{
    unsigned result = 0;
    unsigned shift = 1;

    while (true) {
        const uint8_t byte = *buffer++;
        result += (byte & 0x7F) * shift;
        if (byte & 0x80)
            return result;
        shift <<= 7;
        result += shift;
    }
}

int main(int argc, char* argv[])
{
    long fileSize;
    uint8_t *curBuf;

    if(argc != 2)
    {
        printUsage();
        return 1;
    }

    FILE *f = fopen(argv[1], "rb+");
    if (!f) {
        perror("[bps-patch-trim]: Failed to open file");
        printUsage();
        return 1;
    }

    fseek(f, 0, SEEK_END);
    fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);

    uint8_t *buffer = new uint8_t[fileSize];
    fread((void*)buffer, 1, fileSize, f);
    
    // check and skip BPS1 magic value bytes
    if(memcmp(buffer, "BPS1", 4) != 0)
    {
        printf("[bps-patch-trim]: Not a valid BPS patch file\n");
        delete[] buffer;
        fclose(f);
        return 1;
    }
    curBuf = buffer + 4;

    const unsigned sourceSize =  readVarInt(curBuf); // read and skip the source size
    const unsigned targetSize = readVarInt(curBuf); // read and skip the target size
    const unsigned metadataSize = readVarInt(curBuf); // read and skip the metadata size

    printf("[bps-patch-trim]: Successfully read BPS header:\n");
    printf("\t\tSource size: %u\n", sourceSize);
    printf("\t\tTarget size: %u\n", targetSize);
    printf("\t\tMetadata size: %u\n", metadataSize);
    
    // skip these header fields
    fileSize -= (curBuf - buffer);

    // strip the CRC values (the BPS footer)
    fileSize -= 12;

    // now start writing the relevant data
    fseek(f, 0, SEEK_SET);
    fwrite((void*)curBuf, 1, fileSize, f);
    delete[] buffer;
    buffer = nullptr;

    // make sure we flush the written data to the file
    // before we call ftruncate() to resize the file
    fflush(f);

    // now truncate the file (make it shorter)
    int fd = fileno(f);
    ftruncate(fd, fileSize);
    fclose(f);

    return 0;
}