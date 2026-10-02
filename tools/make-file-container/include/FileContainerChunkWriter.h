#ifndef _FILECONTAINERCHUNKWITER_H
#define _FILECONTAINERCHUNKWITER_H

#include "utils.h"

/**
 * @brief This class is responsible for divvying up the data into chunks and writing them to separate files. 
 * This is necessary to avoid having to load the entire container into memory at once, which could be a problem for large containers.
 * 
 */
class FileContainerChunkWriter
{
public:
    FileContainerChunkWriter(const char* outPath, const char* filenameBase, uint16_t chunkSize);
    ~FileContainerChunkWriter();

    // Delete copy and move constructors and assignment operators
    FileContainerChunkWriter(const FileContainerChunkWriter&) = delete;
    FileContainerChunkWriter(FileContainerChunkWriter&&) = delete;
    
    FileContainerChunkWriter& operator=(const FileContainerChunkWriter&) = delete;
    FileContainerChunkWriter& operator=(FileContainerChunkWriter&&) = delete;

    void write(const uint8_t* buffer, uint32_t bytesToWrite);
    void writeUint8(uint8_t value);
    void writeUint16(uint16_t value, Endianness fieldEndianness = Endianness::LITTLE);
    void writeUint32(uint32_t value, Endianness fieldEndianness = Endianness::LITTLE);

protected:
private:
    uint16_t getBytesRemainingInCurrentChunk() const;
    bool startChunk();

    uint8_t numChunks_;
    uint16_t currentChunkSize_;
    FILE *currentFile_;
    const char* outPath_;
    const char* filenameBase_;
    const uint16_t chunkSize_;
};

#endif