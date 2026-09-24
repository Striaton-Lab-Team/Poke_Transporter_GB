#ifndef _SCRIPT_VAR_H
#define _SCRIPT_VAR_H

#include <vector>
#include <cstdint>
#include <cstddef>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using byte = uint8_t;

struct ROM_DATA;

typedef struct TextBoxVarInsertionPoint
{
    u32 offset;
    u32 size;
} TextBoxVarInsertionPoint;

class script_var
{
public:
    script_var(u32 nValue, std::vector<script_var *> &var_list_ref, int *nCurr_loc_ptr);
    script_var(std::vector<script_var *> &var_list_ref, int *nCurr_loc_ptr);
    virtual void fill_references(const struct ROM_DATA &curr_GBA_rom, u8 mg_array_ptr[]); // goes through all the script locations and updates them to point to the start
    virtual void set_start();                     // Add a pointer to where the start is
    u32 place_word();                               // Place the value in memory
    int *curr_loc_ptr;
    u32 value;
    std::vector<u32> location_list; // stores all the locations in the script that need to be updated
    u32 start_location_in_script;
};

class asm_var : public script_var
{
public:
    using script_var::script_var;
    void set_start() override;                       // Add a pointer to where the start is
    void set_start(bool nIsDirect);                       // Add a pointer to where the start is
    u8 add_reference();  // Add a location for the variable to be refrenced in the script
    u8 add_reference(int nCommand_offset);  // Add a location for the variable to be refrenced in the script
    void fill_references(const struct ROM_DATA &curr_GBA_rom, u8 mg_array_ptr[]) override; // goes through all the script locations and updates them to point to the start
    u32 get_loc_in_sec30(const struct ROM_DATA &curr_GBA_rom);
    bool isDirect;
};

class xse_var : public script_var
{
public:
    using script_var::script_var;
    int command_offset;    // The amount of bytes in the command before the variable
    xse_var *refrence_var; // The offset from a different location in the script. Used to point to functions through addition/subtraction
    bool has_reference_var;
    void set_start() override;                      // Add a pointer to where the start is
    u8 add_reference(int nCommand_offset); // Add a location for the variable to be refrenced in the script
    u8 add_reference(int nCommand_offset, xse_var *offset_from);
    void fill_references(const struct ROM_DATA &curr_GBA_rom, u8 mg_array_ptr[]) override; // goes through all the script locations and updates them to point to the start
    u32 get_loc_in_sec30(const struct ROM_DATA &curr_GBA_rom);
};

class textbox_var : public xse_var
{
public:
    textbox_var(std::vector<script_var *> &var_list_ref, int *nCurr_loc_ptr, TextBoxVarInsertionPoint &outInsertionPoint);
    void set_text(const byte nText[]);
    void insert_text(const u16 *charset, u8 mg_array[], bool is_hoenn, bool should_set_virtual_start = false);
    void set_start() override;
    const byte *text;
    TextBoxVarInsertionPoint &outInsertionPoint;
};

class movement_var : public xse_var
{
public:
    using xse_var::xse_var;
    void set_movement(const byte movement[], unsigned int nSize, bool is_hoenn_var);
    void insert_movement(u8 mg_array[]);
    void set_start() override;
    const byte *movement;
    unsigned int size;
    bool is_hoenn;
};

class sprite_var : public xse_var
{
public:
    using xse_var::xse_var;
    void insert_sprite_data(const ROM_DATA &curr_GBA_rom, u8 mg_array[], const unsigned int sprite_array[], unsigned int size, const unsigned short palette_array[]);
    void set_start() override;
};

class music_var : public xse_var
{
public:
    using xse_var::xse_var;
    void insert_music_data(const ROM_DATA &curr_GBA_rom, u8 mg_array[], u8 blockCount, u8 priority, u8 reverb, u32 toneDataPointer);
    void set_start() override;
    void add_track(const byte* trackBytes, size_t trackSize);
    int numTracks;
    std::vector<u32> trackPointers;
    std::vector<std::vector<byte>> trackArrays;
};

#endif