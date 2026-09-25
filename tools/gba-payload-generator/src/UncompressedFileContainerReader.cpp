#include "UncompressedFileContainerReader.h"

#include <cstring>

#define FILE_CONTAINER_HEADER_SIZE 4
#define FILE_INDEX_ENTRY_SIZE 2

UncompressedFileContainerReader::UncompressedFileContainerReader(const u8 **chunkList, u32 chunkCount, u32 chunkSize)
    : fileNameBuffer_()
    , currentChunk_(nullptr)
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

UncompressedFileContainerReader::~UncompressedFileContainerReader()
{
    if (fileIndex_)
    {
        delete[] fileIndex_;
        fileIndex_ = nullptr;
    }
}

void UncompressedFileContainerReader::init()
{
    // selects the first chunk to get the header and file index information.
    selectChunk(0);

    fileCount_ = currentChunk_[0];
    hasNames_ = currentChunk_[1];

    // The header size is 4 bytes.
    curPos_ = FILE_CONTAINER_HEADER_SIZE;

    // The file index just stores the file sizes for each file entry.
    // We can calculate the file offset based on them.
    const u32 indexSize = fileCount_ * FILE_INDEX_ENTRY_SIZE;
    fileIndex_ = new u16[fileCount_];
    read((u8*)fileIndex_, indexSize);
}

u32 UncompressedFileContainerReader::getNumberOfFiles() const
{
    return fileCount_;
}

const char* UncompressedFileContainerReader::getFileName(u32 fileIndex)
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

u32 UncompressedFileContainerReader::getFileSize(u32 index) const
{
    // fileIndex_ stores the file sizes for each entry.
    return *(fileIndex_ + index);
}

void UncompressedFileContainerReader::seekToFile(u32 fileIndex)
{
    const u32 fileOffset = getFileOffset(fileIndex);

    seek(fileOffset);
}

void UncompressedFileContainerReader::read(u8 *buffer, u32 size)
{
    u32 bytesRemaining = size;

    while(bytesRemaining > 0)
    {
        if(curPos_ == chunkSize_)
        {
            // we need to select a different chunk
            ++curChunkIndex_;
            selectChunk(curChunkIndex_);
        }

        const u32 bytesRemainingInChunk = chunkSize_ - curPos_;
        const u32 bytesToRead = bytesRemaining < bytesRemainingInChunk ? bytesRemaining : bytesRemainingInChunk;

        memcpy(buffer, currentChunk_ + curPos_, bytesToRead);
        bytesRemaining -= bytesToRead;
        buffer += bytesToRead;
        curPos_ += bytesToRead;
    }
}

const u8 *UncompressedFileContainerReader::getPointerToFile(u32 fileIndex)
{
    seekToFile(fileIndex);
    return currentChunk_ + curPos_;
}

void UncompressedFileContainerReader::seekAndRead(u32 fileIndex, u8 *buffer, u32 size)
{
    seekToFile(fileIndex);
    read(buffer, size);
}

void UncompressedFileContainerReader::readFile(u32 fileIndex, u8 *buffer)
{
    const u32 fileSize = getFileSize(fileIndex);
    seekAndRead(fileIndex, buffer, fileSize);
}

u32 UncompressedFileContainerReader::getFileOffset(u32 entryIndex) const
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

void UncompressedFileContainerReader::seek(u32 offset)
{
    const u32 targetChunkIndex = offset / chunkSize_;

    if(curChunkIndex_ != targetChunkIndex)
    {
        // we need to select a different chunk
        selectChunk(targetChunkIndex);
    }

    curPos_ = offset % chunkSize_;
}

void UncompressedFileContainerReader::selectChunk(u32 chunkIndex)
{
    currentChunk_ = chunkList_[chunkIndex];
    curChunkIndex_ = chunkIndex;
    curPos_ = 0;
}