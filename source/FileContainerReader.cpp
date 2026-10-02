#include "FileContainerReader.h"
#include <tonc.h>

#include <cstring>

#define FILE_CONTAINER_HEADER_SIZE 4
#define FILE_INDEX_ENTRY_SIZE 2

FileContainerReader::FileContainerReader(const u8 **chunkList, u32 chunkCount, u32 chunkSize)
    : fileNameBuffer_()
    , decompressionBuffer_(nullptr)
    , chunkList_(chunkList)
    , chunkCount_(chunkCount)
    , chunkSize_(chunkSize)
    , fileIndex_(nullptr)
    , curChunkIndex_(0)
    , curPos_(0)
    , fileCount_(0)
    , hasNames_(false)
{
}

FileContainerReader::~FileContainerReader()
{
    if (fileIndex_)
    {
        delete[] fileIndex_;
        fileIndex_ = nullptr;
    }
}

bool FileContainerReader::init(u8 *decompressionBuffer, u32 decompressionBufferSize)
{
    if(decompressionBufferSize < chunkSize_)
    {
        return false;
    }

    decompressionBuffer_ = decompressionBuffer;

    // decompress the first chunk to get the header and file index information.
    uncompressChunk(0);

    fileCount_ = decompressionBuffer_[0];
    hasNames_ = decompressionBuffer_[1];

    // The header size is 4 bytes.
    curPos_ = FILE_CONTAINER_HEADER_SIZE;

    // The file index just stores the file sizes for each file entry.
    // We can calculate the file offset based on them.
    const u32 indexSize = fileCount_ * FILE_INDEX_ENTRY_SIZE;
    fileIndex_ = new u16[fileCount_];
    read((u8*)fileIndex_, indexSize);

    return true;
}

u32 FileContainerReader::getNumberOfFiles() const
{
    return fileCount_;
}

const char* FileContainerReader::getFileName(u32 fileIndex)
{
    if(!hasNames_)
    {
        return nullptr;
    }

    // the file names are stored right after the file index, and each name is 16 bytes long.
    const u32 indexSize = fileCount_ * FILE_INDEX_ENTRY_SIZE;
    const u32 nameTableStartPos = FILE_CONTAINER_HEADER_SIZE + indexSize;
    seek(nameTableStartPos + (fileIndex * FILE_NAME_LENGTH));

    // Now copy the file name to a local buffer.
    // the reason is that not every string is necessarily null terminated in the fileocontainer data.
    read((u8*)fileNameBuffer_, FILE_NAME_LENGTH);
    fileNameBuffer_[FILE_NAME_LENGTH] = '\0';
    return fileNameBuffer_;
}

u32 FileContainerReader::getFileSize(u32 index) const
{
    // fileIndex_ stores the file sizes for each entry.
    return *(fileIndex_ + index);
}

void FileContainerReader::seekToFile(u32 fileIndex)
{
    const u32 fileOffset = getFileOffset(fileIndex);

    seek(fileOffset);
}

void FileContainerReader::read(u8 *buffer, u32 size)
{
    u32 bytesRemaining = size;

    while(bytesRemaining > 0)
    {
        if(curPos_ == chunkSize_)
        {
            // we need to decompress a different chunk
            ++curChunkIndex_;
            uncompressChunk(curChunkIndex_);
        }

        const u32 bytesRemainingInChunk = chunkSize_ - curPos_;
        const u32 bytesToRead = bytesRemaining < bytesRemainingInChunk ? bytesRemaining : bytesRemainingInChunk;

        memcpy(buffer, decompressionBuffer_ + curPos_, bytesToRead);
        bytesRemaining -= bytesToRead;
        buffer += bytesToRead;
        curPos_ += bytesToRead;
    }
}

u8 *FileContainerReader::getPointerToFileInDecompressionBuffer(u32 fileIndex)
{
    seekToFile(fileIndex);
    return decompressionBuffer_ + curPos_;
}

void FileContainerReader::seekAndRead(u32 fileIndex, u8 *buffer, u32 size)
{
    seekToFile(fileIndex);
    read(buffer, size);
}

void FileContainerReader::readFile(u32 fileIndex, u8 *buffer)
{
    const u32 fileSize = getFileSize(fileIndex);
    seekAndRead(fileIndex, buffer, fileSize);
}

u32 FileContainerReader::getFileOffset(u32 entryIndex) const
{
    // files data start after:
    // - header (4 bytes)
    // - file index (fileCount * 2 bytes)
    // - optional name table (fileCount * 16 bytes)
    //
    u32 offset = FILE_CONTAINER_HEADER_SIZE + (fileCount_ * FILE_INDEX_ENTRY_SIZE);
    if(hasNames_)
    {
        offset += fileCount_ * FILE_NAME_LENGTH;
    }

    // accumulate the sizes of all previous entries to get the file offset
    for(u32 i = 0; i < entryIndex; ++i)
    {
        offset += getFileSize(i);
    }

    return offset;
}

void FileContainerReader::seek(u32 offset)
{
    const u32 targetChunkIndex = offset / chunkSize_;

    if(curChunkIndex_ != targetChunkIndex)
    {
        // we need to decompress a different chunk
        uncompressChunk(targetChunkIndex);
    }

    curPos_ = offset % chunkSize_;
}

void FileContainerReader::uncompressChunk(u32 chunkIndex)
{
    LZ77UnCompWram(chunkList_[chunkIndex], decompressionBuffer_);
    curChunkIndex_ = chunkIndex;
    curPos_ = 0;
}