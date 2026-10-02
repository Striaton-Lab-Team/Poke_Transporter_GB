#ifndef DEBUG_MODE_H
#define DEBUG_MODE_H

#ifndef BUILD_INFO
#define BUILD_INFO "NaN"
#endif

#ifndef PTGB_BUILD_LANGUAGE
#define PTGB_BUILD_LANGUAGE 1
#endif

#ifndef DEBUG_MODE
#define DEBUG_MODE 0
#endif

#include "typeDefs.h"

enum WriteCableDataMode
{
    WRITE_CABLE_DATA_MODE_OFF = 0,
    WRITE_CABLE_DATA_MODE_SRAM = 1,
    WRITE_CABLE_DATA_MODE_CART = 2
};

/**
 * @brief This structs contains debug options
 * that can be influenced through the debug menu.
 */
typedef struct debug_options
{
    /** This option will print the link cable data. Pause the transfer with L, resume with R. Skip printing with DOWN. */
    bool print_link_data;

    /** This option will print the link cable packets. Pause the transfer with L, resume with R. Skip printing with DOWN. */
    bool print_link_packets;

    /**
     * @brief If this option has been set, we won't animate the text, but show the full text immediately.
     */
    bool instant_text_speed;

    /**
     * @brief If this option is set, we won't try to access a game cartridge. Instead, we'll pretend we're using
     * a game cartridge with predefined data. (DEBUG_GAME, DEBUG_VERS, DEBUG_LANG)
     * We won't show game sprites either.
     *
     * This is useful for testing our code in an emulator or without a game cartridge.
     */
    bool ignore_game_pak;

    /**
     * @brief If this option is set, we won't try to show sprites that are stored on the game cartridge.
     * This is useful for testing our code in an emulator or without a game cartridge.
     */
    bool ignore_game_pak_sprites;

    /**
     * @brief If this flag is set, we won't actually do a link cable connection.
     * Instead, we pretend the connection was successful and use gen1_rb_debug_box_data or gen2_debug_box_data.
     */
    bool ignore_link_cable;

    /**
     * @brief If this option is set, we won't block the user from proceeding if he/she hasn't beat the E4 or
     * hasn't activated Mystery Gift yet. This is mostly useful for debug purposes.
     */
    bool ignore_mg_e4_flags;

    /**
     * @brief If this option is set, we won't block the transfer from finishing if the player hasn't collected
     * the pokémon from previous transfers. Instead those will get overwritten. This does make you lose those pokémon.
     * But it can be useful for debugging. (because you wouldn't need to boot into the game everytime between transfer attempts)
     */
    bool ignore_unreceived_pkmn;

    /**
     * @brief This option will make PTGB show the tutorial even if the tutorial completion flag was already set before.
     * Useful for debugging purposes.
     */
    bool force_tutorial;

    /**
     * @brief This option will show the pokémon even if not a single one is valid.
     * Once again, this can be useful for debugging purposes.
     */
    bool dont_hide_invalid_pkmn;

    /**
     * @brief This option will avoid blocking the transfer if the player hasn't caught enough pokémon yet.
     */
    bool ignore_dex_completion;

    /**
     * @brief This option will pretend as if the player caught all pokémon. This is useful to get past the dex completion check
     */
    bool force_all_caught;

    /**
     * @brief If this option is set, we will write the data received over the link cable to SRAM.
     * Please make sure to only use this with loader.gba/Poke_Transporter_GB_standalone.gba and
     * make sure to ignore_game_pak and ignore_game_pak_sprites too!
     *
     * Also: you may need to reboot to your flashcart menu to make it update the .sav file.
     */
    u8 write_cable_data_to_save;

    /**
     * @brief If this option is set, we will load the data -written by write_cable_data_to_save- from the SRAM
     * instead of actually communicating over the link cable.
     */
    u8 load_cable_data_from_save;

    /**
     * @brief If this option is enabled, we will show control characters in the text.
     */
    bool display_control_char;
} debug_options;

extern debug_options g_debug_options;

// Options that affect and require payload_builder
// (and therefore can't be put in the debug menu)
#define SHOW_DATA_PACKETS (false && DEBUG_MODE)
#define PAYLOAD_EXPORT_TEST (false && DEBUG_MODE)
#define DEBUG_PAYLOADS (false && DEBUG_MODE)
#define DONT_REMOVE_PKMN (false && DEBUG_MODE)

// impractical to add to debug menu.
#define DONT_TRANSFER_POKEMON_AT_INDEX_X (false && DEBUG_MODE)
#define POKEMON_INDEX_TO_SKIP 5

// not sure if we want these in the debug menu.
#define DEBUG_GAME EMERALD_ID
#define DEBUG_VERS VERS_1_0
#define DEBUG_LANG LANG_ENG

// Compile time options
#define ENABLE_DEBUG_MENU true
#define ENABLE_TEXT_DEBUG_SCREEN true
#define ENABLE_DEBUG_PKMN_INJECTION true
#define ENABLE_MYSTERY_GIFT true
#define USE_CUSTOM_MALLOC 1
// needs to be a value divisible by 4
#define CUSTOM_MALLOC_POOL_SIZE 8192

// This option enables PTGB_MGBA_XYZ() log messages. These get printed to the mgba log window.
// This option can only be enabled in debug builds because it relies on mgba_printf,
// which will only get included in debug builds for licensing reasons.
// Compiling with this option enabled in release builds will cause a static_assert failure.
// But the && DEBUG_MODE check will already prevent this from being enabled in release builds accidentally,
// so as long as no-one removes that, it should be fine.
#define DEBUG_USE_MGBA_PRINT (true && DEBUG_MODE)

#endif