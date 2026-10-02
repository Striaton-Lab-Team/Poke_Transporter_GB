#include <tonc.h>
#include <cmath>

#include "pokedex.h"
#include "dex_handler.h"
#include "sprite_data.h"
#include "pokemon_data.h"
#include "global_frame_controller.h"
#include "ptgb_save_data_manager.h"
#include "button_handler.h"
#include "translated_text.h"
#include "text_engine.h"
#include "FileContainerReader.h"
#include "text_tables.h"
#include "TYPES_lz10_bin.h"

Dex dex_array[DEX_MAX];
int dex_shift = 0;
int dex_x_cord = 8;
int speed = 0;
int delay = 0;
int count = 0;
Button kanto_count;
Button johto_count;
int kanto_dex_num;
int johto_dex_num;
int kanto_offset;
int johto_offset;
bool mew_caught;
bool celebi_caught;
bool missingno_caught = false;

static void load_general_table_text_entries(u8 *decompression_buffer, u32 decompression_buffer_size, u8 *kanto_buffer, u8 *johto_buffer)
{
    const u8 **chunkList;
    u32 numChunks;
    u32 chunkSize;
    get_text_table_chunks(GENERAL_INDEX, &chunkList, &numChunks, &chunkSize);
    FileContainerReader text_reader(chunkList, numChunks, chunkSize);
    text_reader.init(decompression_buffer, decompression_buffer_size);

    text_reader.readFile(GENERAL_kanto_name, kanto_buffer);
    text_reader.readFile(GENERAL_johto_name, johto_buffer);
}

void pokedex_init()
{
    for (int i = 0; i < DEX_MAX; i++)
    {
        // dex_array[i] = Dex(dex_sprites[DEX_SPRITE_LEFT][i], dex_sprites[DEX_SPRITE_MID][i], dex_sprites[DEX_SPRITE_RIGHT][i]);
        // dex_array[i].set_location(dex_x_cord, (i * (8 * 3)) + 8);
    }
    // kanto_count = Button(button_blank_left, button_blank_right, 64);
    // johto_count = Button(button_blank_left_2, button_blank_right_2, 64);
    kanto_dex_num = get_dex_completion(1, true);
    johto_dex_num = get_dex_completion(2, true);
    if (kanto_dex_num >= 100)
    {
        kanto_offset = 0;
    }
    else if (kanto_dex_num >= 10)
    {
        kanto_offset = 1;
    }
    else
    {
        kanto_offset = 2;
    }
    if (johto_dex_num >= 100)
    {
        johto_offset = 0;
    }
    else if (johto_dex_num >= 10)
    {
        johto_offset = 1;
    }
    else
    {
        johto_offset = 2;
    }
    mew_caught = is_caught(151);
    celebi_caught = is_caught(251);
    obj_unhide(up_arrow, 0);
    obj_hide(down_arrow);
}

#include "gen_3_charsets_lz10_bin.h"
#include "libstd_replacements.h"

// attribute noinline is used to make sure it doesn't get inlined and permanently use IWRAM for the decompression_buffer
int __attribute__((noinline)) pokedex_loop()
{
    u8 TYPES[POKEMON_ARRAY_SIZE][2];
    u8 kanto_name[12];
    u8 johto_name[12];
    u8 decompression_buffer[2048];
    //u16 charset[512];
    u8 name_buffer[16];
    const u8 **namesChunkList;
    u32 namesNumChunks;
    u32 namesChunkSize;

    LZ77UnCompWram(TYPES_lz10_bin, (u8*)TYPES);
    //LZ77UnCompWram(gen_3_charsets_lz10_bin, (u8*)charset);

    load_general_table_text_entries(decompression_buffer, sizeof(decompression_buffer), kanto_name, johto_name);

    get_text_table_chunks(PKMN_NAMES_INDEX, &namesChunkList, &namesNumChunks, &namesChunkSize);
    FileContainerReader PKMN_NAMES(namesChunkList, namesNumChunks, namesChunkSize);
    PKMN_NAMES.init(decompression_buffer, sizeof(decompression_buffer));

    pokedex_init();
    pokedex_show();
    bool update = true;

    const byte undiscovered_text[] = {0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xAE, 0xFF};
    byte temp_string[4] = {}; // Should never be longer than 4 characters (including endline)
                              // TODO: For some reason there is screen tearing here. Probably not noticable on console,
                              // but it should be removed at some point

	tte_set_ink(INK_DARK_GREY);
    tte_set_pos(8, 146);
    ptgb_write_simple(kanto_name, true);
    convert_int_to_ptgb_str(kanto_dex_num, temp_string, 3);
    ptgb_write_simple(temp_string, true);
    temp_string[0] = 0xBA; // "/"
    temp_string[1] = 0xFF;
    ptgb_write_simple(temp_string, true);
    convert_int_to_ptgb_str(mew_caught ? 151 : 150, temp_string, 3);
    ptgb_write_simple(temp_string, true);

    tte_set_pos(128, 146);
    ptgb_write_simple(johto_name, true);
    convert_int_to_ptgb_str(johto_dex_num, temp_string, 3);
    ptgb_write_simple(temp_string, true);
    temp_string[0] = 0xBA; // "/"
    temp_string[1] = 0xFF;
    ptgb_write_simple(temp_string, true);
    convert_int_to_ptgb_str(celebi_caught ? 100 : 99, temp_string, 3);
    ptgb_write_simple(temp_string, true);

    while (true)
    {
        if (key_hit(KEY_B))
        {
            pokedex_hide();
            return 0;
        }
        else if (key_hit(KEY_DOWN) || key_hit(KEY_UP))
        {
            dex_shift += key_tri_vert();
            update = true;
        }
        else if (key_held(KEY_DOWN) || key_held(KEY_UP))
        {
            if (delay > SPEED_DELAY)
            {
                if ((get_frame_count() % speed == 0))
                {
                    if (count > 3 && speed >= 5)
                    {
                        speed = speed / 2;
                        count = 0;
                    }
                    else
                    {
                        count++;
                    }
                    dex_shift += key_tri_vert();
                    update = true;
                }
            }
            else
            {
                delay++;
            }
        }
        else if (key_hit(KEY_LEFT) || key_hit(KEY_RIGHT))
        {
            dex_shift += key_tri_horz() * 7;
            update = true;
        }
        else if (key_held(KEY_LEFT) || key_held(KEY_RIGHT))
        {
            if (delay > SPEED_DELAY)
            {
                if ((get_frame_count() % speed == 0))
                {
                    if (count > 3 && speed >= 5)
                    {
                        speed = speed / 2;
                        count = 0;
                    }
                    else
                    {
                        count++;
                    }
                    dex_shift += key_tri_horz() * 7;
                    update = true;
                }
            }
            else
            {
                delay++;
            }
        }
        else
        {
            speed = 20;
            delay = 0;
            count = 0;
        }

        if (dex_shift > DEX_SHIFT_MAX - !celebi_caught)
        {
            dex_shift = DEX_SHIFT_MAX - !celebi_caught;
        }
        else if (dex_shift < 0 - missingno_caught)
        {
            dex_shift = 0 - missingno_caught;
        }

        if (dex_shift == DEX_SHIFT_MAX - !celebi_caught)
        {
            obj_unhide(up_arrow, 0);
            obj_hide(down_arrow);
        }
        else if (dex_shift == 0 - missingno_caught)
        {
            obj_hide(up_arrow);
            obj_unhide(down_arrow, 0);
        }
        else
        {
            obj_unhide(up_arrow, 0);
            obj_unhide(down_arrow, 0);
        }
        if (update)
        {
            tte_set_ink(INK_ROM_COLOR);
            tte_erase_rect(0, 0, 240, 140);
            int mythic_skip = 0;
            for (int i = 0; i < DEX_MAX; i++)
            {
                if (dex_shift + i + 1 == 151 && !mew_caught)
                {
                    mythic_skip++;
                }

                if (is_caught(dex_shift + i + 1 + mythic_skip))
                {
                    tte_set_pos(dex_x_cord + (3 * 8 / 2), (i * 8 * 2) + 28);
                    temp_string[0] = 0xF7;
                    temp_string[1] = 0xFF;
                    ptgb_write_simple(temp_string, true);
                }

                tte_set_pos(dex_x_cord + (3 * 8), (i * 8 * 2) + 28);
                convert_int_to_ptgb_str(dex_shift + i + 1 + mythic_skip, temp_string, 3);
                ptgb_write_simple(temp_string, true);

                tte_set_pos(dex_x_cord + (7 * 8), (i * 8 * 2) + 28);
                PKMN_NAMES.readFile(dex_shift + i + 1 + mythic_skip, name_buffer);
                ptgb_write_simple(is_caught(dex_shift + i + 1 + mythic_skip) ? name_buffer : undiscovered_text, true);

            }
            //global_next_frame(); // This is a bit silly, but it works. Makes the types one frame off from the text, but that's 'fine'
            // Eventually it could be optimized to move the labels around, but this honestly makes the most sense. Less code but one frame different
            for (int i = 0; i < DEX_MAX; i++)
            {
                load_type_sprites((const u8*)TYPES, dex_shift + i + 1 + mythic_skip, i, is_caught(dex_shift + i + 1 + mythic_skip));
            }
            update = false;
        }
        VBlankIntrWait();
    }
}

void pokedex_show()
{
    for (int i = 0; i < DEX_MAX; i++)
    {
        dex_array[i].show();
    }
    kanto_count.set_location(160, 40);
    johto_count.set_location(160, 88);
    kanto_count.show();
    johto_count.show();
}

void pokedex_hide()
{
    for (int i = 0; i < DEX_MAX; i++)
    {
        tte_erase_rect(0, 0, 240, 160);
        dex_array[i].hide();
    }
    kanto_count.hide();
    johto_count.hide();
    obj_hide(up_arrow);
    obj_hide(down_arrow);
    obj_hide_multi(type_sprites[0], 14);
}