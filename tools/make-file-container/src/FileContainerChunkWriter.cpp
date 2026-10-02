#include "FileContainerChunkWriter.h"

#define PATH_BUFFER_SIZE 4096

static void determineChunkFilePath(char *outBuffer, const char *containerPath, const char *filenameBase, int chunkIndex)
{
    std::snprintf(outBuffer, PATH_BUFFER_SIZE, "%s/%s.chunk%d.bin", containerPath, filenameBase, chunkIndex);
}

FileContainerChunkWriter::FileContainerChunkWriter(const char* outPath, const char* filenameBase,uint16_t chunkSize)
    : numChunks_(0)
    , currentChunkSize_(0)
    , currentFile_(NULL)
    , outPath_(outPath)
    , filenameBase_(filenameBase)
    , chunkSize_(chunkSize)
{
}

FileContainerChunkWriter::~FileContainerChunkWriter()
{
    if(currentFile_)
    {
        fclose(currentFile_);
        currentFile_ = NULL;
    }
}

void FileContainerChunkWriter::write(const uint8_t* buffer, uint32_t bytesToWrite)
{
    uint32_t bytesToWriteNow;
    uint16_t chunkSpace = getBytesRemainingInCurrentChunk();

    while(bytesToWrite > 0)
    {
        if(chunkSpace == 0)
        {
            if(!startChunk())
            {
                return;
            }
            chunkSpace = getBytesRemainingInCurrentChunk();
        }

        bytesToWriteNow = (bytesToWrite > chunkSpace) ? chunkSpace : bytesToWrite;
        fwrite(buffer, 1, bytesToWriteNow, currentFile_);
        
        currentChunkSize_ += bytesToWriteNow;
        buffer += bytesToWriteNow;
        chunkSpace -= bytesToWriteNow;
        bytesToWrite -= bytesToWriteNow;
    }
}

void FileContainerChunkWriter::writeUint8(uint8_t value)
{
    write(&value, 1);
}

void FileContainerChunkWriter::writeUint16(uint16_t value, Endianness fieldEndianness)
{
    uint8_t buffer[2];
    if(fieldEndianness == Endianness::LITTLE && !g_isLittleEndian)
    {
        value = __builtin_bswap16(value);
    }
    else if(fieldEndianness == Endianness::BIG && g_isLittleEndian)
    {
        value = __builtin_bswap16(value);
    }
    buffer[0] = value & 0xFF;
    buffer[1] = (value >> 8) & 0xFF;
    write(buffer, 2);
}

void FileContainerChunkWriter::writeUint32(uint32_t value, Endianness fieldEndianness)
{
    uint8_t buffer[4];
    if(fieldEndianness == Endianness::LITTLE && !g_isLittleEndian)
    {
        value = __builtin_bswap32(value);
    }
    else if(fieldEndianness == Endianness::BIG && g_isLittleEndian)
    {
        value = __builtin_bswap32(value);
    }
    buffer[0] = value & 0xFF;
    buffer[1] = (value >> 8) & 0xFF;
    buffer[2] = (value >> 16) & 0xFF;
    buffer[3] = (value >> 24) & 0xFF;
    write(buffer, 4);
}

uint16_t FileContainerChunkWriter::getBytesRemainingInCurrentChunk() const
{
    if(!currentFile_)
    {
        return 0;
    }
    return chunkSize_ - currentChunkSize_;
}

bool FileContainerChunkWriter::startChunk()
{
    if(currentFile_)
    {
        fclose(currentFile_);
        currentFile_ = NULL;
    }
    currentChunkSize_ = 0;

    char chunk_path[PATH_BUFFER_SIZE];
    determineChunkFilePath(chunk_path, outPath_, filenameBase_, numChunks_);
    currentFile_ = fopen(chunk_path, "wb");
    if(!currentFile_)
    {
        fprintf(stderr, "Failed to open output file: %s\n", chunk_path);
        return false;
    }
    ++numChunks_;
    return true;
}