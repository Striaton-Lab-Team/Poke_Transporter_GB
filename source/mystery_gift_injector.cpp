#include <tonc.h>
#include "mystery_gift_injector.h"
#include "pokemon_data.h"
#include "flash_mem.h"
#include "rom_data.h"
#include "libraries/Pokemon-Gen3-to-Gen-X/include/save.h"
#include "pokemon_data.h"
#include "ptgb_save_data_manager.h"
#include "FileContainerReader.h"
#include "global_frame_controller.h"
#include "translated_text.h"
#include "script_patches.h"
#include "section30_patches.h"
#include "script_ruby_english_1_0_lz10_bin.h"
#include "script_patches_chunk0_lz10_bin.h"
#include "script_patches_chunk1_lz10_bin.h"
#include "section30_ruby_english_1_0_lz10_bin.h"
#include "section30_patches_chunk0_lz10_bin.h"
#include "section30_patches_chunk1_lz10_bin.h"
#include "RSEFRLG_chunk0_lz10_bin.h"
#include <cstdlib>
#include <cstring>
#include <array>

extern "C"
{
    void bps_patch(const u8 *src, u8 *out, const u8 *patch, u32 patch_size);
}

#define MG_SCRIPT_SIZE 0x3E8

// These defines are there because rom_data defines values that
// occupy more bytes/bits than needed.
// We use these to create a table with a packed key below: patchFileTable
// also refer to the comment on PACK_PATCH_KEY() for more details on how the key is packed.
#define MAP_RUBY 0
#define MAP_SAPPHIRE 1
#define MAP_FIRERED 2
#define MAP_LEAFGREEN 3
#define MAP_EMERALD 4

#define MAP_JAPANESE 0
#define MAP_ENGLISH 1
#define MAP_FRENCH 2
#define MAP_GERMAN 3
#define MAP_ITALIAN 4
#define MAP_SPANISH 5


// So, we need to associate the rom_data values with a specific patch file to convert the
// game-based payload to our specific language variant.
// But we only have indexes and their enum namings.
// So we need to map them.
// The thing is: using the existing RUBY_ID and LANG_ENG defines, would make this table much larger than it needs to be.
// There are 5 game variants (3 bits), and 6 languages (3 bits) and the highest version of those roms is 1.2 (2 bits)
// So we can pack this data into a single byte.
#define PACK_PATCH_KEY(game, lang, version) (((game) << 5) | ((lang) << 2) | (version))

#define SKIP_SPECIFIC_PAYLOAD_PATCH_INDEX 0xFF

typedef struct PatchFileTableEntry
{
    u8 key;
    u8 patchIndex;
} PatchFileTableEntry;

// We only need to have a single map of the patch files, because the order and size of the section30 and script patches are exactly the same.
static constexpr PatchFileTableEntry patchFileTable[] =
{
    { PACK_PATCH_KEY(MAP_RUBY, MAP_ENGLISH, VERS_1_0), SKIP_SPECIFIC_PAYLOAD_PATCH_INDEX },
    { PACK_PATCH_KEY(MAP_RUBY, MAP_ENGLISH, VERS_1_2), (u8)Script_patchesFiles::SCRIPT_RUBY_ENGLISH_1_2 },
    { PACK_PATCH_KEY(MAP_RUBY, MAP_ENGLISH, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_RUBY_ENGLISH_1_1 },
    { PACK_PATCH_KEY(MAP_SAPPHIRE, MAP_ENGLISH, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_SAPPHIRE_ENGLISH_1_1 },
    { PACK_PATCH_KEY(MAP_SAPPHIRE, MAP_ENGLISH, VERS_1_2), (u8)Script_patchesFiles::SCRIPT_SAPPHIRE_ENGLISH_1_2 },
    { PACK_PATCH_KEY(MAP_SAPPHIRE, MAP_ENGLISH, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_SAPPHIRE_ENGLISH_1_0 },
    { PACK_PATCH_KEY(MAP_EMERALD, MAP_ENGLISH, VERS_1_0), SKIP_SPECIFIC_PAYLOAD_PATCH_INDEX },
    { PACK_PATCH_KEY(MAP_FIRERED, MAP_ENGLISH, VERS_1_0), SKIP_SPECIFIC_PAYLOAD_PATCH_INDEX },
    { PACK_PATCH_KEY(MAP_FIRERED, MAP_ENGLISH, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_FIRERED_ENGLISH_1_1 },
    { PACK_PATCH_KEY(MAP_LEAFGREEN, MAP_ENGLISH, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_LEAFGREEN_ENGLISH_1_0 },
    { PACK_PATCH_KEY(MAP_LEAFGREEN, MAP_ENGLISH, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_LEAFGREEN_ENGLISH_1_1 },
    { PACK_PATCH_KEY(MAP_RUBY, MAP_FRENCH, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_RUBY_FRENCH_1_0 },
    { PACK_PATCH_KEY(MAP_RUBY, MAP_FRENCH, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_RUBY_FRENCH_1_1 },
    { PACK_PATCH_KEY(MAP_SAPPHIRE, MAP_FRENCH, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_SAPPHIRE_FRENCH_1_1 },
    { PACK_PATCH_KEY(MAP_SAPPHIRE, MAP_FRENCH, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_SAPPHIRE_FRENCH_1_0 },
    { PACK_PATCH_KEY(MAP_EMERALD, MAP_FRENCH, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_EMERALD_FRENCH_1_0 },
    { PACK_PATCH_KEY(MAP_FIRERED, MAP_FRENCH, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_FIRERED_FRENCH_1_0 },
    { PACK_PATCH_KEY(MAP_LEAFGREEN, MAP_FRENCH, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_LEAFGREEN_FRENCH_1_0 },
    { PACK_PATCH_KEY(MAP_RUBY, MAP_GERMAN, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_RUBY_GERMAN_1_1 },
    { PACK_PATCH_KEY(MAP_RUBY, MAP_GERMAN, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_RUBY_GERMAN_1_0 },
    { PACK_PATCH_KEY(MAP_SAPPHIRE, MAP_GERMAN, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_SAPPHIRE_GERMAN_1_0 },
    { PACK_PATCH_KEY(MAP_SAPPHIRE, MAP_GERMAN, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_SAPPHIRE_GERMAN_1_1 },
    { PACK_PATCH_KEY(MAP_EMERALD, MAP_GERMAN, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_EMERALD_GERMAN_1_0 },
    { PACK_PATCH_KEY(MAP_FIRERED, MAP_GERMAN, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_FIRERED_GERMAN_1_0 },
    { PACK_PATCH_KEY(MAP_LEAFGREEN, MAP_GERMAN, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_LEAFGREEN_GERMAN_1_0 },
    { PACK_PATCH_KEY(MAP_RUBY, MAP_ITALIAN, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_RUBY_ITALIAN_1_0 },
    { PACK_PATCH_KEY(MAP_RUBY, MAP_ITALIAN, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_RUBY_ITALIAN_1_1 },
    { PACK_PATCH_KEY(MAP_SAPPHIRE, MAP_ITALIAN, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_SAPPHIRE_ITALIAN_1_1 },
    { PACK_PATCH_KEY(MAP_SAPPHIRE, MAP_ITALIAN, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_SAPPHIRE_ITALIAN_1_0 },
    { PACK_PATCH_KEY(MAP_EMERALD, MAP_ITALIAN, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_EMERALD_ITALIAN_1_0 },
    { PACK_PATCH_KEY(MAP_FIRERED, MAP_ITALIAN, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_FIRERED_ITALIAN_1_0 },
    { PACK_PATCH_KEY(MAP_LEAFGREEN, MAP_ITALIAN, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_LEAFGREEN_ITALIAN_1_0 },
    { PACK_PATCH_KEY(MAP_RUBY, MAP_JAPANESE, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_RUBY_JAPANESE_1_0 },
    { PACK_PATCH_KEY(MAP_RUBY, MAP_JAPANESE, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_RUBY_JAPANESE_1_1 },
    { PACK_PATCH_KEY(MAP_SAPPHIRE, MAP_JAPANESE, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_SAPPHIRE_JAPANESE_1_1 },
    { PACK_PATCH_KEY(MAP_SAPPHIRE, MAP_JAPANESE, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_SAPPHIRE_JAPANESE_1_0 },
    { PACK_PATCH_KEY(MAP_EMERALD, MAP_JAPANESE, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_EMERALD_JAPANESE_1_0 },
    { PACK_PATCH_KEY(MAP_FIRERED, MAP_JAPANESE, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_FIRERED_JAPANESE_1_1 },
    { PACK_PATCH_KEY(MAP_FIRERED, MAP_JAPANESE, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_FIRERED_JAPANESE_1_0 },
    { PACK_PATCH_KEY(MAP_LEAFGREEN, MAP_JAPANESE, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_LEAFGREEN_JAPANESE_1_1 },
    { PACK_PATCH_KEY(MAP_LEAFGREEN, MAP_JAPANESE, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_LEAFGREEN_JAPANESE_1_0 },
    { PACK_PATCH_KEY(MAP_RUBY, MAP_SPANISH, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_RUBY_SPANISH_1_0 },
    { PACK_PATCH_KEY(MAP_RUBY, MAP_SPANISH, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_RUBY_SPANISH_1_1 },
    { PACK_PATCH_KEY(MAP_SAPPHIRE, MAP_SPANISH, VERS_1_1), (u8)Script_patchesFiles::SCRIPT_SAPPHIRE_SPANISH_1_1 },
    { PACK_PATCH_KEY(MAP_SAPPHIRE, MAP_SPANISH, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_SAPPHIRE_SPANISH_1_0 },
    { PACK_PATCH_KEY(MAP_EMERALD, MAP_SPANISH, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_EMERALD_SPANISH_1_0 },
    { PACK_PATCH_KEY(MAP_FIRERED, MAP_SPANISH, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_FIRERED_SPANISH_1_0 },
    { PACK_PATCH_KEY(MAP_LEAFGREEN, MAP_SPANISH, VERS_1_0), (u8)Script_patchesFiles::SCRIPT_LEAFGREEN_SPANISH_1_0 }
};

// This will need to be modified for the JP releases
static constexpr u8 em_wonder_card[0x14E] = {
    0x08, 0x6E, 0x00, 0x00, 0xBA, 0xB4, 0xBE, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0xCA, 0xCC, 0xC9, 0xC0, 0xBF, 0xCD, 0xCD, 0xC9, 0xCC, 0x00, 0xC0, 0xBF, 0xC8, 0xC8, 0xBF, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0x00, 0xBD, 0xD9, 0xE6, 0xE8, 0xDD, 0xDA, 0xDD, 0xD7, 0xD5, 0xE8, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0xDD, 0xE7, 0xDD, 0xE8, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xDC, 0xE3, 0xE9, 0xE7, 0xD9, 0x00, 0xE7, 0xE3, 0xE9, 0xE8, 0xDC, 0xD9, 0xD5, 0xE7, 0xE8, 0x00, 0xE3, 0xDA, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xBD, 0xBF, 0xC8, 0xCE, 0xBF, 0xCC, 0x00, 0xDD, 0xE2, 0x00, 0xCD, 0xE3, 0xE3, 0xE8, 0xE3, 0xE4, 0xE3, 0xE0, 0xDD, 0xE7, 0x00, 0xBD, 0xDD, 0xE8, 0xED, 0x00, 0xE8, 0xE3, 0x00, 0xE6, 0xD9, 0xD7, 0xDD, 0xD9, 0xEA, 0xD9, 0x00, 0x00, 0x00, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBE, 0xE3, 0x00, 0xE2, 0xE3, 0xE8, 0x00, 0xE8, 0xE3, 0xE7, 0xE7, 0x00, 0xE8, 0xDC, 0xDD, 0xE7, 0x00, 0xBF, 0xEC, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0x00, 0xBD, 0xD5, 0xE6, 0xD8, 0x00, 0xD6, 0xD9, 0xDA, 0xE3, 0xE6, 0xD9, 0x00, 0x00, 0x00, 0xE6, 0xD9, 0xD7, 0xD9, 0xDD, 0xEA, 0xDD, 0xE2, 0xDB, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// static u8 rs_wonder_card[0x14E] = {
//    0xCE, 0x7C, 0x00, 0x00, 0xBA, 0xB4, 0xBE, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0xCA, 0xCC, 0xC9, 0xC0, 0xBF, 0xCD, 0xCD, 0xC9, 0xCC, 0x00, 0xC0, 0xBF, 0xC8, 0xC8, 0xBF, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0x00, 0xBD, 0xD9, 0xE6, 0xE8, 0xDD, 0xDA, 0xDD, 0xD7, 0xD5, 0xE8, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0xDD, 0xE7, 0xDD, 0xE8, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xDC, 0xE3, 0xE9, 0xE7, 0xD9, 0x00, 0xE7, 0xE3, 0xE9, 0xE8, 0xDC, 0xD9, 0xD5, 0xE7, 0xE8, 0x00, 0xE3, 0xDA, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xBD, 0xBF, 0xC8, 0xCE, 0xBF, 0xCC, 0x00, 0xDD, 0xE2, 0x00, 0xC7, 0xE3, 0xE7, 0xE7, 0xD8, 0xD9, 0xD9, 0xE4, 0x00, 0xBD, 0xDD, 0xE8, 0xED, 0x00, 0xE8, 0xE3, 0x00, 0xE6, 0xD9, 0xD7, 0xDD, 0xD9, 0xEA, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBE, 0xE3, 0x00, 0xE2, 0xE3, 0xE8, 0x00, 0xE8, 0xE3, 0xE7, 0xE7, 0x00, 0xE8, 0xDC, 0xDD, 0xE7, 0x00, 0xBF, 0xEC, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0x00, 0xBD, 0xD5, 0xE6, 0xD8, 0x00, 0xD6, 0xD9, 0xDA, 0xE3, 0xE6, 0xD9, 0x00, 0x00, 0x00, 0xE6, 0xD9, 0xD7, 0xD9, 0xDD, 0xEA, 0xDD, 0xE2, 0xDB, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // checksum
static constexpr u8 frlg_wonder_card[0x14E] = {
    0x67, 0x18, 0x00, 0x00, 0xBA, 0xB4, 0xBE, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0xCA, 0xCC, 0xC9, 0xC0, 0xBF, 0xCD, 0xCD, 0xC9, 0xCC, 0x00, 0xC0, 0xBF, 0xC8, 0xC8, 0xBF, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0x00, 0xBD, 0xD9, 0xE6, 0xE8, 0xDD, 0xDA, 0xDD, 0xD7, 0xD5, 0xE8, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0xDD, 0xE7, 0xDD, 0xE8, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xDC, 0xE3, 0xE9, 0xE7, 0xD9, 0x00, 0xE7, 0xE3, 0xE9, 0xE8, 0xDC, 0x00, 0xE3, 0xDA, 0x00, 0xE8, 0xDC, 0xD9, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0x00, 0x00, 0x00, 0x00, 0xBD, 0xBF, 0xC8, 0xCE, 0xBF, 0xCC, 0x00, 0xE3, 0xE2, 0x00, 0xCD, 0xD9, 0xEA, 0xD9, 0xE2, 0x00, 0xC3, 0xE7, 0xE0, 0xD5, 0xE2, 0xD8, 0x00, 0xE8, 0xE3, 0x00, 0xE6, 0xD9, 0xD7, 0xDD, 0xD9, 0xEA, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBE, 0xE3, 0x00, 0xE2, 0xE3, 0xE8, 0x00, 0xE8, 0xE3, 0xE7, 0xE7, 0x00, 0xE8, 0xDC, 0xDD, 0xE7, 0x00, 0xBF, 0xEC, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0x00, 0xBD, 0xD5, 0xE6, 0xD8, 0x00, 0xD6, 0xD9, 0xDA, 0xE3, 0xE6, 0xD9, 0x00, 0x00, 0x00, 0xE6, 0xD9, 0xD7, 0xD9, 0xDD, 0xEA, 0xDD, 0xE2, 0xDB, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // checksum

static const u8 *RSEFRLG_chunklist[] = { RSEFRLG_chunk0_lz10_bin };

/// function to calculate a 32-bit checksum of a script buffer.
static u32 calc_checksum32(const u8 *buffer, u32 size)
{
    u32 i;
    u32 checksum = 0;

    for (i = 0; i < size; ++i)
    {
        checksum += buffer[i];
    }

    return checksum;
}

/// function to calculate a 16-bit CRC of a script buffer.
static u16 calc_crc16(const u8 *buffer, u32 size) // Implementation taken from PokeEmerald Decomp
{
    u32 i, j;
    u16 crc = 0x1121;

    for (i = 0; i < size; ++i)
    {
        crc ^= buffer[i];
        for (j = 0; j < 8; ++j)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x8408;
            else
                crc >>= 1;
        }
    }
    return ~crc;
};

/** Converts the lang code to the PCCS Language enum */
static Language convertLangCodeIntoLanguage(int32_t langCode)
{
    switch(langCode)
    {
        case LANG_JPN:
            return JAPANESE;
        case LANG_ENG:
            return ENGLISH;
        case LANG_FRE:
            return FRENCH;
        case LANG_ITA:
            return ITALIAN;
        case LANG_GER:
            return GERMAN;
        case LANG_SPA:
            return SPANISH;
        default:
            return ENGLISH;
    }
}

/**
 * @brief This function injects a text entry generated by text_helper into the provided buffer.
 * NOTE: bufferPos should point to the exact location at which the text entry should be injected.
 * NOTE: this code originated from text_var::insert_text() and has been adapted for use in this context.
 *
 * @param charset a charset buffer to use hen injecting the text entry. It's used to detect colored text and remove it if necessary.
 * @param bufferPos an offset into the buffer where the text entry should be injected.
 * @param textReader a FileContainerReader instance used to read the text entry.
 * @param entryIndex the index of the text entry to inject.
 * @return u32 the number of bytes injected into the buffer.
 */
static u32 __attribute__((noinline)) injectTextEntryIntoBuffer(const u16 *charset, u8 *buffer, u32 bufferSize, u32 bufferPos, FileContainerReader &textReader, u32 entryIndex)
{
    const u8 *curText = textReader.getPointerToFileInDecompressionBuffer(entryIndex);
    u8 *curBuffer = buffer + bufferPos;
    u8 *tempBuffer = (u8*)malloc(textReader.getFileSize(entryIndex));
    u32 bufferDepth = 0;

    // Process the text entry and copy it into a temporary buffer, removing any colored text if necessary.
    while((*curText) != 0xFF)
    {
        if(curr_GBA_rom.is_hoenn())
        {
            if(*curText == 0xFC && (get_char_from_charset(charset, (u16)(*(curText + 1))) == 0x01)) // Removes colored text
            {
                curText += 2;
                continue;
            }
        }

        *(tempBuffer + bufferDepth) = *curText;
        ++bufferDepth;
        ++curText;
    }
    *(tempBuffer + bufferDepth) = 0xFF; // End string
    ++bufferDepth;

    // Move the existing buffer content to make room for the new text entry
    memmove(curBuffer + bufferDepth, curBuffer, bufferSize - bufferPos - bufferDepth);
    // now copy the processed text into the buffer at the current position
    memcpy(curBuffer, tempBuffer, bufferDepth);
    free(tempBuffer);

    return bufferDepth;
}

/**
 * @brief This function re-injects the RSEFRLG text table entries into the section30 and script payload buffers.
 * In gba-payload-generator, we are stripping those texts after payload generation to improve compression and while keeping the pointers/offsets
 * inside the payload intact.
 *
 * But in order to use the payload, we actually need to reinsert those at runtime.
 */
static void __attribute__((noinline)) reinjectMysteryGiftPayloadTexts(u8 *section30Buffer, u8 *mgScriptBuffer)
{
    u8 decompressionBuffer[4096];
    u16 gen3Charset[256];
    u32 bytesInjected;
    // the reason for using std::array for these is simply to be able to use initializer lists below.
    std::array<u8, 8> section30TextSequence;
    std::array<u8, 4> scriptTextSequence;
    u8 textGreetEntryIndex;
    u8 youMustBeEntryIndex;
    u8 iAmEntryIndex;
    u32 section30Offset;
    u32 mgScriptOffset;
    bool firstTime = true;

    // This determines if the event has been done before
    for (unsigned i = 1; i <= 251; i++)
    {
        if (is_caught(i))
        {
            firstTime = false;
            break;
        }
    }

    FileContainerReader textReader(RSEFRLG_chunklist, sizeof(RSEFRLG_chunklist) / sizeof(RSEFRLG_chunklist[0]), sizeof(decompressionBuffer));
    textReader.init(decompressionBuffer, sizeof(decompressionBuffer));


    // make the section30Buffer and mgScriptBuffer pointers point to the exact insertion position within them.
    // the section30 text offset (0x97E) remains consistent across languages and games in the payload.
    // the script text offset shifts depending on the game version. (observed based on logs from gba-payload-generator)

    // Also construct text entry sequences, because we need to maintain the same order of injection
    // as during the payload generation with gba-payload-generator
    section30Offset = 0x97E;

    switch(curr_GBA_rom.gamecode)
    {
    case RUBY_ID:
    case SAPPHIRE_ID:
        mgScriptOffset = 0x268;
        textGreetEntryIndex = RSEFRLG_dia_textGreet_rse;
        iAmEntryIndex = (firstTime) ? RSEFRLG_dia_textIAm_first_rs : RSEFRLG_dia_textIAm_second_rs;
        section30TextSequence = { RSEFRLG_dia_textThank_rs, RSEFRLG_dia_textPCFull_rs, RSEFRLG_dia_textWeHere_rs, RSEFRLG_dia_textPCConvo_rs, RSEFRLG_dia_textPCThanks_rs, RSEFRLG_dia_textLookerFull_rs, RSEFRLG_dia_textMoveBox_rs, RSEFRLG_dia_textRecieved_rs };
        break;
    case FIRERED_ID:
    case LEAFGREEN_ID:
        mgScriptOffset = 0x278;
        textGreetEntryIndex = RSEFRLG_dia_textGreet_frlg;
        iAmEntryIndex = (firstTime) ? RSEFRLG_dia_textIAm_first_frlge : RSEFRLG_dia_textIAm_second_frlge;
        section30TextSequence = { RSEFRLG_dia_textThank_frlge, RSEFRLG_dia_textPCFull_frlge, RSEFRLG_dia_textWeHere_frlg, RSEFRLG_dia_textPCConvo_frlge, RSEFRLG_dia_textPCThanks_frlge, RSEFRLG_dia_textLookerFull_frlge, RSEFRLG_dia_textMoveBox_frlg, RSEFRLG_dia_textRecieved_frlge };
        break;
    default:
        mgScriptOffset = 0x298;
        textGreetEntryIndex = RSEFRLG_dia_textGreet_rse;
        iAmEntryIndex = (firstTime) ? RSEFRLG_dia_textIAm_first_frlge : RSEFRLG_dia_textIAm_second_frlge;
        section30TextSequence = { RSEFRLG_dia_textThank_frlge, RSEFRLG_dia_textPCFull_frlge, RSEFRLG_dia_textWeHere_e, RSEFRLG_dia_textPCConvo_frlge, RSEFRLG_dia_textPCThanks_frlge, RSEFRLG_dia_textLookerFull_frlge, RSEFRLG_dia_textMoveBox_e, RSEFRLG_dia_textRecieved_frlge };
        break;
    }

    youMustBeEntryIndex = (firstTime) ? RSEFRLG_dia_textYouMustBe_first : RSEFRLG_dia_textYouMustBe_second;
    scriptTextSequence = {textGreetEntryIndex, youMustBeEntryIndex, iAmEntryIndex };

    load_localized_charset(gen3Charset, 3, convertLangCodeIntoLanguage(curr_GBA_rom.language));

    for(unsigned i=0; i < 8; ++i)
    {
        bytesInjected = injectTextEntryIntoBuffer(gen3Charset, section30Buffer, 4096, section30Offset, textReader, section30TextSequence[i]);
        section30Offset += bytesInjected;
    }

    for(unsigned i=0; i < 3; ++i)
    {
        bytesInjected = injectTextEntryIntoBuffer(gen3Charset, mgScriptBuffer, MG_SCRIPT_SIZE, mgScriptOffset, textReader, scriptTextSequence[i]);
        mgScriptOffset += bytesInjected;
    }
}

/**
 * @brief This function injects the PokeBox pokémons into section30 at offset 0x0, which is what the payload expects.
 * It assumes that all the pokemons in the box are already converted. (should be done by script_array.cpp)
 *
 * @param box pointer to the PokeBox instance containing the Pokémon to inject.
 * @param section30Buffer The buffer representing section 30 of the save file.
 */
static void __attribute__((noinline)) injectBoxIntoSection30(PokeBox* box, u8* section30Buffer)
{
    u8 dex_nums[MAX_PKMN_IN_BOX] = {};
    u8 *curSection30 = section30Buffer;

    for (int i = 0; i < MAX_PKMN_IN_BOX; i++) // Add in the Pokemon data
    {
        Gen3Pokemon *curr_pkmn = box->getGen3Pokemon(i);
        if (curr_pkmn->isValid)
        {
            tonccpy(curSection30, curr_pkmn->dataArrayPtr, POKEMON_SIZE);

            curr_pkmn->decryptSubstructures();
            dex_nums[i] = curr_pkmn->getSpeciesIndexNumber();
        }

        curSection30 += POKEMON_SIZE;
    }

    // Add in the dex numbers
    tonccpy(curSection30, dex_nums, MAX_PKMN_IN_BOX);
}

static void pickPatchFiles(u32 &gamePatchFile, u32 &specificPatchFile)
{
    u32 simplifiedGameCode;
    u32 simplifiedLanguageCode;

    switch(curr_GBA_rom.gamecode)
    {
        case RUBY_ID:
            gamePatchFile = (u32)Script_patchesFiles::SCRIPT_RUBY_ENGLISH_1_0;
            simplifiedGameCode = MAP_RUBY;
            break;
        case SAPPHIRE_ID:
            gamePatchFile = (u32)Script_patchesFiles::SCRIPT_RUBY_ENGLISH_1_0;
            simplifiedGameCode = MAP_SAPPHIRE;
            break;
        case FIRERED_ID:
            gamePatchFile = (u32)Script_patchesFiles::SCRIPT_FIRERED_ENGLISH_1_0;
            simplifiedGameCode = MAP_FIRERED;
            break;
        case LEAFGREEN_ID:
            gamePatchFile = (u32)Script_patchesFiles::SCRIPT_FIRERED_ENGLISH_1_0;
            simplifiedGameCode = MAP_LEAFGREEN;
            break;
        case EMERALD_ID:
        default:
            gamePatchFile = (u32)Script_patchesFiles::SCRIPT_EMERALD_ENGLISH_1_0;
            simplifiedGameCode = MAP_EMERALD;
            break;
    }

    switch(curr_GBA_rom.language)
    {
        case LANG_JPN:
            simplifiedLanguageCode = MAP_JAPANESE;
            break;
        case LANG_ENG:
            simplifiedLanguageCode = MAP_ENGLISH;
            break;
        case LANG_FRE:
            simplifiedLanguageCode = MAP_FRENCH;
            break;
        case LANG_GER:
            simplifiedLanguageCode = MAP_GERMAN;
            break;
        case LANG_ITA:
            simplifiedLanguageCode = MAP_ITALIAN;
            break;
        case LANG_SPA:
            simplifiedLanguageCode = MAP_SPANISH;
            break;
        default:
            simplifiedLanguageCode = MAP_ENGLISH;
            break;
    }

    const u8 mapKey = PACK_PATCH_KEY(simplifiedGameCode, simplifiedLanguageCode, (u8)curr_GBA_rom.version);
    for(u32 i=0; i < sizeof(patchFileTable) / sizeof(patchFileTable[0]); ++i)
    {
        if(patchFileTable[i].key == mapKey)
        {
            specificPatchFile = patchFileTable[i].patchIndex;
            break;
        }
    }
}

/**
 * @brief This helper function will apply the BPS patch with index patchFileIndex stored in reader
 * to the srcBuffer data and store the result in dstBuffer. srcBuffer remains untouched.
 */
static void __attribute__((noinline)) applyBPSPatchToBuffer(FileContainerReader &reader, u8* dstBuffer, u8 *srcBuffer, u32 patchFileIndex)
{
    // Note: you may think it's peculiar that we're malloc'ing patchBuffer below.
    // But here's the dilemma: the files in the file container may span multiple chunks.
    // (mostly the last file in each chunk)
    // Therefore using getPointerToFileInDecompressionBuffer() would be unsafe.
    // We could've used the @noSpanChunks directive in the .containerdef file to overcome that.
    // However, then we're hit by an additional problem: We've constructed patchFileTable above using the script_patches
    // indexes, relying on those to be consistent/representative for both the section30 and script patch filecontainers.
    // but section30 is much larger than script. Therefore the patches don't have the same filesizes either.
    // This would result in @noSpanChunks injecting a different number of dummy files in each filecontainer,
    // which would break the index consistency.
    //
    // You might argue that we might as well use a statically allocated buffer for the patch.
    // However, our IWRAM consumption is already quite high here. And malloc() will just use unused space at the end of EWRAM.
    // (=where our multiboot rom resides)
    const u32 patchFileSize = reader.getFileSize(patchFileIndex);
    u8 *patchBuffer = (u8*)malloc(patchFileSize);
    reader.readFile(patchFileIndex, patchBuffer);
    bps_patch(srcBuffer, dstBuffer, patchBuffer, patchFileSize);
    free(patchBuffer);
}

static void __attribute__((noinline)) patchBuffer(FileContainerReader &reader, u8 *buffer, u32 bufferSize, u32 gamePatchFileIndex, u32 specificPatchFileIndex)
{
    u8 tempBuffer[4096];
    memset(tempBuffer, 0, bufferSize);

    applyBPSPatchToBuffer(reader, tempBuffer, buffer, gamePatchFileIndex);

    // We shouldn't attempt to apply the same base payload patch twice.
    // that's what the magic SKIP_SPECIFIC_PAYLOAD_PATCH_INDEX value is for.
    if(specificPatchFileIndex != SKIP_SPECIFIC_PAYLOAD_PATCH_INDEX)
    {
        // now turn the english <gametype> payload into the specific language payload.
        // the result will be stored back in buffer
        applyBPSPatchToBuffer(reader, buffer, tempBuffer, specificPatchFileIndex);
    }
    else
    {
        // no specific patch file, so just copy the tempBuffer back to buffer
        memcpy(buffer, tempBuffer, bufferSize);
    }
}

/**
 * @brief This function reconstructs the pregenerated payload for section30 and the script buffer.
 * The way it works is this:
 * - We have stored the english ruby 1.0 payload (section30 + scriptbuffer) in compressed form.
 * - We have created BPS patches to convert this payload from english ruby to english <gametype> (e.g. sapphire, emerald, ...)
 * - We have created another BPS patch to convert the english <gametype> payload to the specific language variant of the game.
 * 
 * This is all done to conserve space as much as possible and benefit most from compression. 
 * 
 * So, in order to reconstruct the payload for a specific game and language, we need to:
 * 1. Decompress the english ruby payload into section30Buffer and scriptBuffer.
 * 2. Determine the 2 patches required (transform to right gametype, transform to right language) for both scriptBuffer and section30Buffer.
 *    (4 patches total)
 * 3. uncompress these patches from the filecontainer.
 * 4. Apply these patches.
 * 
 * This function attempts to do it in a way that requires the least amount of IWRAM possible.
 * One way we do that is by reusing the same decompression buffer for both file containers.
 * 
 * WARNING: This function will require more than 6KB of additional IWRAM for decompression buffer and temp buffer!
 * NOTE: __attribute__((noinline)) is used to ensure this function gets its own stack frame and therefore the local
 * buffers don't leak into the caller's stack frame. This is important because IWRAM consumption is a big concern in PTGB.
 */
static void __attribute__((noinline)) reconstructPregeneratedPayloads(u8* section30Buffer, u8* scriptBuffer)
{
    u8 decompressionBuffer[DEFAULT_CHUNK_SIZE];
    u32 gamePatchFile;
    u32 specificPatchFile = (u8)Script_patchesFiles::SCRIPT_RUBY_ENGLISH_1_0;

    const u8* section30PatchesChunkList[] = {
        section30_patches_chunk0_lz10_bin,
        section30_patches_chunk1_lz10_bin
    };

    const u8* scriptPatchesChunkList[] = {
        script_patches_chunk0_lz10_bin,
        script_patches_chunk1_lz10_bin
    };

    FileContainerReader section30PatchesReader(section30PatchesChunkList, sizeof(section30PatchesChunkList) / sizeof(section30PatchesChunkList[0]));
    FileContainerReader scriptPatchesReader(scriptPatchesChunkList, sizeof(scriptPatchesChunkList) / sizeof(scriptPatchesChunkList[0]));

    // first determine the patch indexes needed to convert the base payloads to the specific game.
    pickPatchFiles(gamePatchFile, specificPatchFile);

    // assume we found the entry.
    memset(section30Buffer, 0, 0x1000);
    memset(scriptBuffer, 0, MG_SCRIPT_SIZE);
    LZ77UnCompWram(section30_ruby_english_1_0_lz10_bin, section30Buffer);
    LZ77UnCompWram(script_ruby_english_1_0_lz10_bin, scriptBuffer);

    // patch section30
    section30PatchesReader.init(decompressionBuffer, DEFAULT_CHUNK_SIZE);
    patchBuffer(section30PatchesReader, section30Buffer, 0x1000, gamePatchFile, specificPatchFile);

    // patch script
    scriptPatchesReader.init(decompressionBuffer, DEFAULT_CHUNK_SIZE);
    patchBuffer(scriptPatchesReader, scriptBuffer, MG_SCRIPT_SIZE, gamePatchFile, specificPatchFile);
}


bool inject_mystery(PokeBox* box)
{
    u32 checksum;
    u8 script_buffer[MG_SCRIPT_SIZE];

    reconstructPregeneratedPayloads(global_memory_buffer, script_buffer);
    reinjectMysteryGiftPayloadTexts(global_memory_buffer, script_buffer);

    // One thing our pregeneration process obviously leaves out is the pokemon data.
    // so let's add that in now.
    injectBoxIntoSection30(box, global_memory_buffer);

    if (curr_GBA_rom.is_ruby_sapphire())
    {
        checksum = calc_checksum32(script_buffer, MG_SCRIPT_SIZE);
    }
    else
    {
        checksum = calc_crc16(script_buffer, MG_SCRIPT_SIZE);
    }

    // Add in Pokemon and Dex data
    // In the steps after this, we will be recycling the global_memory_buffer to read and write data to other sections of the save.
    // So we really MUST write the generated data now, before we lose it.

    update_memory_buffer_checksum(global_memory_buffer, false);
    erase_sector(0x1E000);
    copy_ram_to_save(&global_memory_buffer[0], 0x1E000, 0x1000);

    // section_30 data has been stored, so now we can safely re-use the global_memory_buffer for other sections.
    // Let's move on to the next step.

    // Add in Wonder Card
    copy_save_to_ram(memory_section_array[4], &global_memory_buffer[0], 0x1000);
    switch (curr_GBA_rom.gamecode)
    {
    case RUBY_ID:
    case SAPPHIRE_ID:
        // Wonder Card doesn't exist
        break;
    case FIRERED_ID:
    case LEAFGREEN_ID:
        memcpy(global_memory_buffer + curr_GBA_rom.offset_wondercard, frlg_wonder_card, 0x14E);
        break;
    case EMERALD_ID:
    default:
        memcpy(global_memory_buffer + curr_GBA_rom.offset_wondercard, em_wonder_card, 0x14E);
        break;
    }

    // Set checksum and padding
    global_memory_buffer[curr_GBA_rom.offset_script] = checksum >> 0;
    global_memory_buffer[curr_GBA_rom.offset_script + 1] = checksum >> 8;
    global_memory_buffer[curr_GBA_rom.offset_script + 2] = checksum >> 16;
    global_memory_buffer[curr_GBA_rom.offset_script + 3] = checksum >> 24;

    // Add in Mystery Script data
    memcpy(global_memory_buffer + curr_GBA_rom.offset_script + 4, script_buffer, MG_SCRIPT_SIZE);

    update_memory_buffer_checksum(global_memory_buffer, false);
    erase_sector(memory_section_array[4]);
    copy_ram_to_save(&global_memory_buffer[0], memory_section_array[4], 0x1000);

    // Set flags
    int memory_section = 1 + ((curr_GBA_rom.offset_flags + (curr_GBA_rom.unused_flag_start / 8)) / 0xF80); // This sets the correct memory section, since flags stretch between section 1 and 2.
    copy_save_to_ram(memory_section_array[memory_section], &global_memory_buffer[0], 0x1000);
    global_memory_buffer[(curr_GBA_rom.offset_flags + (curr_GBA_rom.all_collected_flag / 8)) % 0xF80] &= ~(1 << (curr_GBA_rom.all_collected_flag % 8)); // Set "collected all" flag to 0

    for (int i = 0; i < box->getNumInBox(); i++)
    {
        int curr_flag;
        curr_flag = curr_GBA_rom.pkmn_collected_flag_start + i;
        global_memory_buffer[(curr_GBA_rom.offset_flags + (curr_flag / 8)) % 0xF80] &= ~(1 << (curr_flag % 8)); // Reset the flag
        if (box->getGen3Pokemon(i)->isValid)
        {
            global_memory_buffer[(curr_GBA_rom.offset_flags + (curr_flag / 8)) % 0xF80] |= (1 << (curr_flag % 8)); // Set flag accordingly
        }
    }

    update_memory_buffer_checksum(global_memory_buffer, false);
    erase_sector(memory_section_array[memory_section]);
    copy_ram_to_save(&global_memory_buffer[0], memory_section_array[memory_section], 0x1000);

    // Save custom save data
    write_custom_save_data();
    return true;
}
