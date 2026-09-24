#include <tonc.h>
#include "ptgb_save_data_manager.h"
#include "flash_mem.h"
#include "dbg/debug_mode.h"
#include "button_menu.h"
#include "pokemon_data.h"
#include "libraries/Pokemon-Gen3-to-Gen-X/include/save.h"
#include "script_array.h"

byte save_data_array[SAVE_DATA_SIZE];

void load_custom_save_data()
{
    copy_save_to_ram(HALL_OF_FAME + 0x1000, &global_memory_buffer[0], 0x1000);
    memcpy(save_data_array, global_memory_buffer + HOF_SECTION, SAVE_DATA_SIZE);
}

void write_custom_save_data()
{
    copy_save_to_ram(HALL_OF_FAME + 0x1000, &global_memory_buffer[0], 0x1000);
    memcpy(global_memory_buffer + HOF_SECTION, save_data_array, SAVE_DATA_SIZE);

    update_memory_buffer_checksum(global_memory_buffer, true);
    erase_sector(HALL_OF_FAME + 0x1000);
    copy_ram_to_save(&global_memory_buffer[0], HALL_OF_FAME + 0x1000, 0x1000);
}

bool is_caught(int dex_num)
{
    return (((save_data_array[CAUGHT_DATA + (dex_num / 8)]) >> dex_num % 8) & 1) || g_debug_options.force_all_caught;
}

void set_caught(int dex_num)
{
    if (!is_caught(dex_num))
    {
        newPkmn |= true;
    }
    save_data_array[CAUGHT_DATA + (dex_num / 8)] = save_data_array[CAUGHT_DATA + (dex_num / 8)] | (1 << (dex_num % 8));
}

int get_def_lang_num()
{
    return save_data_array[DEFAULT_LANGUAGE];
}

void set_def_lang(int nLang)
{
    save_data_array[DEFAULT_LANGUAGE] = nLang;
}

void set_tutorial_flag(bool value)
{
    save_data_array[TUTORIAL_FLAG] = value;
}

bool get_tutorial_flag()
{
    return save_data_array[TUTORIAL_FLAG];
}

void initialize_save_data()
{
    memset(save_data_array, 0, SAVE_DATA_SIZE);
    set_tutorial_flag(true);
    write_custom_save_data();
}

int get_dex_completion(int gen, bool include_mythicals)
{
    int stop = (gen == 1 ? 151 : 251);
    int start = (gen == 1 ? 1 : 152);
    int count = 0;
    for (int i = start; i < stop + include_mythicals; i++)
    {
        if (is_caught(i))
        {
            count++;
        }
    }
    return count;
}

bool check_can_save()
{
    byte data_buffer[8] = {};
    byte test_buffer[8] = {0xC7, 0xCF, 0xCA, 0xCA, 0xBF, 0xCE, 0xA4, 0xBE};

    // Copy the current bytes and save them to the data buffer, and replace them with the test_buffer
    copy_save_to_ram(HALL_OF_FAME + 0x1000, &global_memory_buffer[0], 0x1000);
    for (int i = 0; i < 8; i++)
    {
        data_buffer[i] = global_memory_buffer[HOF_SECTION + i];
        global_memory_buffer[HOF_SECTION + i] = test_buffer[i];
    }
    erase_sector(HALL_OF_FAME + 0x1000);
    copy_ram_to_save(&global_memory_buffer[0], HALL_OF_FAME + 0x1000, 0x1000);

    // Overwrite the data in the global memory buffer to make sure that it isn't still there when testing
    for (int i = 0; i < 8; i++)
    {
        global_memory_buffer[HOF_SECTION + i] = 0;
    }

    // Grab the data again and verify it saved correctly
    copy_save_to_ram(HALL_OF_FAME + 0x1000, &global_memory_buffer[0], 0x1000);
    for (int i = 0; i < 8; i++)
    {
        if (global_memory_buffer[HOF_SECTION + i] != test_buffer[i])
        {
            // Did not save correctly, return false
            return false;
        }
        // Set the byte in the memory buffer back in preperation for saving it back again
        global_memory_buffer[HOF_SECTION + i] = data_buffer[i];
    }

    // Everything saved correctly, save the data back
    erase_sector(HALL_OF_FAME + 0x1000);
    copy_ram_to_save(&global_memory_buffer[0], HALL_OF_FAME + 0x1000, 0x1000);
    return true;
}