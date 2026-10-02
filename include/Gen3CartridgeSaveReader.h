#ifndef _GEN3CARTRIDGE_SAVE_READER_H
#define _GEN3CARTRIDGE_SAVE_READER_H

#include "typeDefs.h"
#include <cstdint>

/**
 * @brief This class is an adapter implementation for Gen3SaveManager,
 * which is used by libPCCS' Gen3SaveManager class.
 * 
 * This specific implementation will allow read/write operations to the cartridge save.
 * That way, actual data origin is abstracted away from libPCCS' Gen3SaveManager.
 * 
 * Note: to avoid runtime bounds-checking overhead, we assume that the caller takes care 
 * not to cross sector boundaries with its read/write operations.
 * 
 * When moving to a different sector with seek(), flush() will get called if there are pending changes in
 * the current buffer.
 * 
 * Note: this class won't automatically load any sector when initialized.
 * Make sure to trigger seek() before doing anything with it.
 */
class Gen3CartridgeSaveReader
{
public:
    Gen3CartridgeSaveReader(u8 *sector_buffer);
    ~Gen3CartridgeSaveReader();

    void read(u8 *buffer, u32 size);
    void readUint8(u8& outByte);
    void readUint16(u16& outWord, Endianness fieldEndianness);
    void readUint32(u32& outDWord, Endianness fieldEndianness);

    void write(const u8 *buffer, u32 size);
    void writeUint8(u8 value);
    void writeUint16(u16 value, Endianness fieldEndianness);
    void writeUint32(u32 value, Endianness fieldEndianness);

    void seek(u32 offset);

    // WARNING: advance and rewind won't cross sector boundaries, so make sure to trigger seek()
    // if you want to move to a different sector.
    void advance(u32 numBytes);
    void rewind(u32 numBytes);

    /**
     * @brief Flush the current buffer to the cartridge save.
     */
    void flush();

    /**
     * @brief Indicates whether the Gen3SaveManager should recalculate checksums when finishSave is called.
     */
    bool shouldRecalculateChecksumsOnFinish() const;
protected:
private:
    uintptr_t sector_start_;
    u8 *sector_buffer_;
    u8 *cur_;
    // whether the current buffer has pending changes that need to be flushed
    bool dirty_;
};

#endif