#ifndef _FILECONTAINERREADER_H
#define _FILECONTAINERREADER_H

#include <cstdint>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;

#define FILE_NAME_LENGTH 16
#define DEFAULT_CHUNK_SIZE 2048

/**
 * @brief This class provides functionality to read files from the file container format
 * created by the make-file-container tool.
 * 
 * While in PTGB itself, these chunks would be compressed, this class is designed to read uncompressed chunks instead.
 * I don't want to re-implement LZ77UnCompWram() in gba-payload-generator and the uncompressed chunks are produced
 * during the PTGB build process anyway, so we can just use those.
 *
 * Beyond the fact that this variant uses the uncompressed chunks, it should be functionally equivalent to the FileContainerReader class in PTGB.
 */
class UncompressedFileContainerReader
{
public:
    UncompressedFileContainerReader(const u8 **chunkList, u32 chunkCount, u32 chunkSize = DEFAULT_CHUNK_SIZE);
    ~UncompressedFileContainerReader();

    /**
     * @brief This function initializes the UncompressedFileContainerReader. It should be called before any other function is used.
     * The reason for this initialization step is to give you control over the moment when the first chunk is being read.
     */
    void init();

    /**
     * @brief Returns the number of files stored in the file container.
     */
    u32 getNumberOfFiles() const;

    /**
     * @brief IF the file container stores the file names,
     * this function returns a pointer to the file name of the file at the given index.
     * Otherwise, it returns nullptr.
     */
    const char* getFileName(u32 fileIndex);

    /**
     * @brief This function returns the size of the file at the given index.
     */
    u32 getFileSize(u32 fileIndex) const;

    /**
     * @brief This function seeks to the beginning of the file at the given index.
     */
    void seekToFile(u32 fileIndex);

    /**
     * @brief Read data from the current position in the file container into the provided buffer.
     */
    void read(u8 *buffer, u32 size);

    /**
     * @brief Gives you a direct pointer to the specified file in the current chunk. (unsafe!)
     *
     * This is useful to use data directly from the current chunk without having to allocate and
     * copy another buffer. This is essential in high memory pressure scenarios, such as mystery_gift_builder.
     * We can't afford to keep multiple linebuffers in memory in addition to the current chunk and all the other variables there.
     *
     * WARNING: this is only safe if you know the file is stored fully in the current chunk, because files may span multiple chunks.
     * Or if the container only consists of a single chunk.
     * Pointers acquired this way will become stale whenever a seek is done to a position in a different chunk, 
     * as that will cause the current chunk to be updated with the new chunk's data.
     */
    const u8 *getPointerToFile(u32 fileIndex);

    /**
     * @brief Combines seekToFile and read().
     * Just a convenience function to make our code shorter :-)
     */
    void seekAndRead(u32 fileIndex, u8 *buffer, u32 size);

    /**
     * @brief Even shorter variant of seekAndRead that reads the entire file at once.
     * But it assumes you have provided a large enough buffer to hold the entire file.
     *
     * (useful for text table reading)
     */
    void readFile(u32 fileIndex, u8 *buffer);
protected:
private:
    /**
     * @brief This function calculates the file offset for the specified file index
     * The file offset is determined by reading the file index (which contains the file entry sizes)
     * and summing up the sizes of all previous entries + header size + optional name table size
     * (if names are stored).
     */
    u32 getFileOffset(u32 fileIndex) const;

    /**
     * @brief This function seeks to the specified absolute offset in the file container data.
     * Absolute offset refers to an offset across the file container chunks.
     */
    void seek(u32 offset);

    /**
     * @brief This function just sets currentChunk_ accordingly
     */
    void selectChunk(u32 chunkIndex);

    char fileNameBuffer_[FILE_NAME_LENGTH + 1];
    const u8 *currentChunk_;
    const u8 **chunkList_;
    u32 chunkCount_;
    u32 chunkSize_;
    u16 *fileIndex_;
    u32 curChunkIndex_;
    u32 curPos_;
    u32 fileCount_;
    bool hasNames_;
};

#endif