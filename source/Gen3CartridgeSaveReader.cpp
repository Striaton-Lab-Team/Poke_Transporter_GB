#include "Gen3CartridgeSaveReader.h"
#include "libraries/Pokemon-Gen3-to-Gen-X/include/save.h"
#include "flash_mem.h"
#include <cstring>

#define HALL_OF_FAME 0x01C000

Gen3CartridgeSaveReader::Gen3CartridgeSaveReader(u8 *sector_buffer)
    : sector_start_(0xFFFFFFFF)
    , sector_buffer_(sector_buffer)
    , cur_(sector_buffer)
    , dirty_(false)
{
    seek(0);
}

Gen3CartridgeSaveReader::~Gen3CartridgeSaveReader()
{
}

void Gen3CartridgeSaveReader::read(u8 *buffer, u32 size)
{
    memcpy(buffer, cur_, size);
    cur_ += size;
}

void Gen3CartridgeSaveReader::readUint8(u8& outByte)
{
    outByte = *cur_;
    ++cur_;
}

void Gen3CartridgeSaveReader::readUint16(u16& outWord, Endianness fieldEndianness)
{
    // Right now we only support little endian (no need for big endian thus far)
    (void)fieldEndianness;
    // The read is implemented this way to avoid any issues with unaligned reads.
    // see writeUint16 for more details.
    outWord = static_cast<u16>(cur_[0]) |
              (static_cast<u16>(cur_[1]) << 8);
    cur_ += sizeof(u16);
}

void Gen3CartridgeSaveReader::readUint32(u32& outDWord, Endianness fieldEndianness)
{
    // Right now we only support little endian (no need for big endian thus far)
    (void)fieldEndianness;
    // The read is implemented this way to avoid any issues with unaligned reads.
    // see writeUint16 for more details.
    outDWord = static_cast<u32>(cur_[0]) |
               (static_cast<u32>(cur_[1]) << 8) |
               (static_cast<u32>(cur_[2]) << 16) |
               (static_cast<u32>(cur_[3]) << 24);
    cur_ += sizeof(u32);
}

void Gen3CartridgeSaveReader::write(const u8 *buffer, u32 size)
{
    memcpy(cur_, buffer, size);
    cur_ += size;
    dirty_ = true;

}

void Gen3CartridgeSaveReader::writeUint8(u8 value)
{
    *cur_ = value;
    ++cur_;
    dirty_ = true;
}

void Gen3CartridgeSaveReader::writeUint16(u16 value, Endianness fieldEndianness)
{
    // Right now we only support little endian (no need for big endian thus far)
    (void)fieldEndianness;
    // The write is implemented this way to avoid any issues with unaligned writes.
    // I tried
    // *((u16*)cur_) = value;
    // earlier, but when cur_ was set to 0x0019, this caused undefined behaviour.
    // (specifically the word was written 1 byte earlier than it should've been)
    cur_[0] = static_cast<u8>(value & 0xFF);
    cur_[1] = static_cast<u8>(value >> 8);
    cur_ += sizeof(u16);
    dirty_ = true;
}

void Gen3CartridgeSaveReader::writeUint32(u32 value, Endianness fieldEndianness)
{
    // Right now we only support little endian (no need for big endian thus far)
    (void)fieldEndianness;
    // The write is implemented this way to avoid any issues with unaligned writes.
    // see writeUint16 for more details.
    cur_[0] = static_cast<u8>(value & 0xFF);
    cur_[1] = static_cast<u8>((value >> 8) & 0xFF);
    cur_[2] = static_cast<u8>((value >> 16) & 0xFF);
    cur_[3] = static_cast<u8>((value >> 24) & 0xFF);
    cur_ += sizeof(u32);
    dirty_ = true;
}

void Gen3CartridgeSaveReader::seek(u32 offset)
{
    const u32 sector_offset = offset % SECTOR_SIZE;
    const uintptr_t sector_start = offset - sector_offset;
    if(sector_start != sector_start_)
    {
        // write any pending changes.
        flush();
        sector_start_ = sector_start;
        copy_save_to_ram(sector_start, sector_buffer_, SECTOR_SIZE);
    }

    cur_ = sector_buffer_ + (offset % SECTOR_SIZE);
}

void Gen3CartridgeSaveReader::advance(u32 numBytes)
{
    u8 *sector_end = sector_buffer_ + SECTOR_SIZE;
    cur_ += numBytes;
    if (cur_ > sector_end)
    {
        cur_ = sector_end;
    }
}

void Gen3CartridgeSaveReader::rewind(u32 numBytes)
{
    if(static_cast<uintptr_t>(cur_ - sector_buffer_) >= numBytes)
    {
        cur_ -= numBytes;
    }
    else
    {
        cur_ = sector_buffer_;
    }
}

void Gen3CartridgeSaveReader::flush()
{
    if(!dirty_)
    {
        return;
    }

    update_memory_buffer_checksum(sector_buffer_, (sector_start_ == HALL_OF_FAME));
    // Real flash requires erase before rewriting bytes that may need 0->1 transitions.
    erase_sector(sector_start_);
    copy_ram_to_save(sector_buffer_, sector_start_, SECTOR_SIZE);
    dirty_ = false;
}

bool Gen3CartridgeSaveReader::shouldRecalculateChecksumsOnFinish() const
{
    return false;
}