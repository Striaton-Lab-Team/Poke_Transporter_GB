#include <tonc.h>
#include <cmath>
#include "libstd_replacements.h"
#include "global_frame_controller.h"
#include "random.h"
#include "background_engine.h"
#include "text_engine.h"
#include "sprite_data.h"
#include "string.h"
#include "translated_text.h"
#include "dbg/debug_mode.h"
#include "FileContainerReader.h"
#include "text_tables.h"

int global_frame_count = 0;
bool rand_enabled = true;
int cable_frame = 0;
int curr_link_animation_state = 0;
int fennel_blink_timer = 0;
int fennel_blink_state = 0;
bool missingno_enabled = false;
bool treecko_enabled = false;

// split off from global_next_frame to limit the stack usage of the general_text_table_buffer to
// the execution of this function
// the noinline attribute prevents the compiler from inlining this function back into the global_next_frame function
static void __attribute__((noinline)) show_pulled_cart_error()
{
    u8 decompression_buffer[2048];
    const u8 **chunkList;
    u32 numChunks;
    u32 chunkSize;

    get_text_table_chunks(GENERAL_INDEX, &chunkList, &numChunks, &chunkSize);

    FileContainerReader general_text_reader(chunkList, numChunks, chunkSize);
    general_text_reader.init(decompression_buffer, sizeof(decompression_buffer));

    ptgb_write_textbox(general_text_reader.getPointerToFileInDecompressionBuffer(GENERAL_pulled_cart_error), true, true,
                       GENERAL_INDEX, GENERAL_pulled_cart_error, true);
}

void global_next_frame()
{
    key_poll();
    rand_next_frame();
    background_frame(global_frame_count);
    determine_fennel_blink();
    if (missingno_enabled)
    {
        set_background_pal(0xFF, false, false);
    }
    oam_copy(oam_mem, obj_buffer, num_sprites);
    // mmFrame(); //Music
    if (global_frame_count % 60 == 0)
    {
        set_menu_sprite_pal(0);
        if (!curr_GBA_rom.verify_rom())
        {
            BG_BACKDROP = (BG_BACKDROP & ~BG_PRIO_MASK) | BG_PRIO(2);
            BG_TEXTBOX = (BG_TEXTBOX & ~BG_PRIO_MASK) | BG_PRIO(1);
            obj_hide_multi(ptgb_logo_l, num_sprites);

            show_pulled_cart_error();

            oam_copy(oam_mem, obj_buffer, num_sprites);
            while (true)
            {
            };
        }
    }
    else if (global_frame_count % 60 == 30)
    {
        set_menu_sprite_pal(1);
    }

    if (global_frame_count % (40 / curr_link_animation_state) == 0)
    {
        cable_frame = (cable_frame + 1) % 12;
        if (curr_link_animation_state > 0)
        {
            run_link_cable_animation(cable_frame);
        }
    }
    global_frame_count++;
};

int get_frame_count()
{
    return global_frame_count;
}

void enable_auto_random()
{
    rand_enabled = true;
}

void disable_auto_random()
{
    rand_enabled = false;
}

const unsigned short MENU_PALS[5][4] = {
    {RGB15(31, 31, 31), RGB15(31, 19, 10), RGB15(31, 7, 01), RGB15(00, 00, 00)},  // Red
    {RGB15(31, 31, 31), RGB15(31, 19, 10), RGB15(10, 9, 31), RGB15(00, 00, 00)},  // Blue
    {RGB15(31, 31, 31), RGB15(31, 19, 10), RGB15(07, 23, 03), RGB15(00, 00, 00)}, // Green
    {RGB15(31, 31, 31), RGB15(31, 19, 10), RGB15(15, 10, 03), RGB15(00, 00, 00)}, // Brown
    {RGB15(31, 31, 31), RGB15(31, 19, 10), RGB15(29, 5, 13), RGB15(00, 00, 00)},  // Pink
};

void set_menu_sprite_pal(int frame)
{
    return;
    for (int i = 0; i < 5; i++)
    {
        unsigned short curr_pal[16] = {
            //              frame: 1 | 2
            MENU_PALS[i][(!frame ? 0 : 0)],
            MENU_PALS[i][(!frame ? 0 : 1)],
            MENU_PALS[i][(!frame ? 0 : 2)],
            MENU_PALS[i][(!frame ? 0 : 3)],
            MENU_PALS[i][(!frame ? 1 : 0)],
            MENU_PALS[i][(!frame ? 1 : 1)],
            MENU_PALS[i][(!frame ? 1 : 2)],
            MENU_PALS[i][(!frame ? 1 : 3)],
            MENU_PALS[i][(!frame ? 2 : 0)],
            MENU_PALS[i][(!frame ? 2 : 1)],
            MENU_PALS[i][(!frame ? 2 : 2)],
            MENU_PALS[i][(!frame ? 2 : 3)],
            MENU_PALS[i][(!frame ? 3 : 0)],
            MENU_PALS[i][(!frame ? 3 : 1)],
            MENU_PALS[i][(!frame ? 3 : 2)],
            MENU_PALS[i][(!frame ? 3 : 3)],
        };
        memcpy(pal_obj_mem + ((MENU_PAL_START + i) * 16), curr_pal, 32);
    }
}

static const int path[12][2] = {{19, 18}, {19, 19}, {18, 19}, {17, 19}, {16, 19}, {15, 19}, {14, 19}, {13, 19}, {12, 19}, {11, 19}, {10, 19}, {24, 24}};

void run_link_cable_animation(int frame)
{
    switch (curr_link_animation_state)
    {
    case STATE_CONNECTION:
        frame %= 4;
        obj_hide_multi(link_frame1, 4);
        obj_unhide_multi(link_frame1, 0, frame);
        break;
    case STATE_TRANSFER:
        obj_set_pos(link_blob1, path[frame][0] * 8, path[frame][1] * 8);
        obj_set_pos(link_blob2, path[frame][0] * 8, path[frame][1] * 8);
        obj_set_pos(link_blob3, path[frame][0] * 8, path[frame][1] * 8);

        obj_hide_multi(link_blob1, 3);
        switch (frame % 3)
        {
        case 0:
            obj_unhide(link_blob1, 0);
            break;
        case 1:
            obj_unhide(link_blob2, 0);
            break;
        case 2:
            obj_unhide(link_blob3, 0);
            break;
        }
        break;
    }
}

void link_animation_state(int state)
{
    cable_frame = 0;
    switch (state)
    {
    case STATE_CONNECTION:
        obj_unhide(gba_cart, 0);
        obj_set_pos(gba_cart, 17 * 8, 14 * 8);

        obj_unhide(cart_shell, 0);
        obj_set_pos(cart_shell, (8 * 8), (11 * 8) + 11);
        obj_unhide(cart_label, 0);
        obj_set_pos(cart_label, (8 * 8) + 8, (11 * 8) + 11 + 13);

        obj_set_pos(link_frame1, 17 * 8, 17 * 8);
        obj_set_pos(link_frame2, 13 * 8, 19 * 8);
        obj_set_pos(link_frame3, 9 * 8, 18 * 8);
        break;
    case STATE_TRANSFER:
        obj_unhide_multi(link_blob1, 0, 3);
    case STATE_NO_ANIM:
        obj_unhide(gba_cart, 0);
        obj_set_pos(gba_cart, 17 * 8, 14 * 8);

        obj_unhide(cart_shell, 0);
        obj_set_pos(cart_shell, (8 * 8), (11 * 8) + 11);
        obj_unhide(cart_label, 0);
        obj_set_pos(cart_label, (8 * 8) + 8, (11 * 8) + 11 + 13);

        obj_unhide(link_frame1, 0);
        obj_set_pos(link_frame1, 17 * 8, 17 * 8);
        obj_unhide(link_frame2, 0);
        obj_set_pos(link_frame2, 13 * 8, 19 * 8);
        obj_unhide(link_frame3, 0);
        obj_set_pos(link_frame3, 9 * 8, 18 * 8);
        break;

    default:
        obj_hide_multi(link_frame1, 3);
        obj_hide_multi(link_blob1, 3);
        obj_hide(gba_cart);
        obj_hide(cart_shell);
        obj_hide(cart_label);
        obj_hide(gb_flag);
        obj_hide(gba_flag);
        break;
    }
    curr_link_animation_state = state;

    if (g_debug_options.print_link_data)
    {
        obj_hide(cart_shell);
        obj_hide(cart_label);
    }
}

void determine_fennel_blink()
{
    if (get_curr_flex_background() == FBG_Fennel)
    {
        if (fennel_blink_timer == 0)
        {
            fennel_blink(fennel_blink_state);
            fennel_blink_state = (fennel_blink_state + 1) % 4;
            if (fennel_blink_state == 3) // Wait a random amount of time
            {
                fennel_blink_timer = get_rand_range(4 * 60, 8 * 60);
            }
            else // Continue with the animation
            {

                fennel_blink_timer = 4;
            }
        }
        else
        {
            fennel_blink_timer--;
        }
    }
    else
    {
        fennel_blink_timer = get_rand_range(4 * 60, 8 * 60);
    }
}

void set_missingno(bool val)
{
    missingno_enabled = val;
    if (val == false)
    {
        set_background_pal(curr_GBA_rom.gamecode, false, false);
        fennel_blink_timer = 0;
    }
}

void set_treecko(bool val)
{
    treecko_enabled = val;
}

bool get_missingno_enabled()
{
    return missingno_enabled;
}

bool get_treecko_enabled()
{
    return treecko_enabled;
}

int get_string_char_count(const byte *str)
{
    int size = 0;
    while (str[size] != 0xFF)
    {
        size++;
    }
    return size;
}

int get_string_length(const byte *str)
{
    int size = 0;
    int length = 0;
    while (str[size] != 0xFF)
    {
        length += tte_get_glyph_width(str[size]);
        size++;
    }
    return length;
}

void convert_int_to_ptgb_str(int val, byte str[], int min_length)
{
    int div = 1;
    int count = 0;
    int num;
    bool non_zero = false;
    bool first = true;

    // Set it up so the number has all the zeros it needs
    for (int i = 0; i < min_length; i++)
    {
        div *= 10;
    }

    // Increase it if the number is still larger
    while (div <= val)
    {
        div *= 10;
    }

    while (div != 0)
    {
        num = val / div;
        if (num != 0 || non_zero)
        {
            non_zero = true;
            str[count] = num + 0xA1; // 0xA1 is 0 in the chart
            count++;
        }
        else
        {
            if (!first)
            {
                str[count] = 0xA1; // 0xA1 is 0 in the chart
                count++;
            }
            else
            {
                first = false;
            }
        }

        val %= div;
        div /= 10;
    }
    str[count] = 0xFF;
}

void convert_int_to_ptgb_str(int val, byte str[])
{
    convert_int_to_ptgb_str(val, str, 0);
}
