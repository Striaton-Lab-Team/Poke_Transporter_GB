#include <tonc.h>
#include <cstring>

#include "text_engine.h"
#include "global_frame_controller.h"
#include "pkmn_font.h"
#include "script_array.h"
#include "dbg/debug_mode.h"
#include "button_menu.h"
#include "sprite_data.h"
#include "fonts.h"
#include "background_engine.h"
#include "pokemon_data.h"
#include "FileContainerReader.h"
#include "text_tables.h"

script_obj curr_line;
uint char_index;
uint line_char_index;
const byte *curr_text;
bool text_exit;

// This function was separated from text_loop to reduce the scope of the text_decompression_buffer.
// if we didn't do this, the decompression_buffer would be kept on the stack (=IWRAM) for the entire duration of the
// text_loop() call. This is particularly bad because the whole mystery_gift_builder sequence is being triggered from within
// text_loop(). And there we need all the IWRAM we can muster.
// Doing it this way does mean that we need to completely restart decompression whenever we switch from dialog entry.
// but given that it requires user input to do so, I believe it's worth it and not time-critical.
// attribute noinline was used to make sure the compiler doesn't inline this code back into text_loop()
static __attribute__((noinline)) const u8 *read_dialogue_text_entry(u32 index, u32 text_section, u8 *output_buffer)
{
    u8 text_decompression_buffer[4096];
    const u8 **chunkList;
    u32 numChunks;
    u32 chunkSize;

    get_text_table_chunks(text_section, &chunkList, &numChunks, &chunkSize);
    FileContainerReader dialogueTable(chunkList, numChunks, chunkSize);

    dialogueTable.init(text_decompression_buffer, sizeof(text_decompression_buffer));
    dialogueTable.readFile(index, output_buffer);

    return output_buffer;
}

// This will have to be changed to be dynamic to support nicknamed Pokemon that are in a different language than the current build.
// Maybe combine Japanese and Latin into one larger font?

#if PTGB_BUILD_LANGUAGE == 1
#define BUILD_FONT &japanese_normalFont
#else
#define BUILD_FONT &latin_normalFont
#endif

void init_text_engine()
{
    // Load the TTE
    // tte_init_se(3, BG_CBB(TEXT_CBB) | BG_SBB(TEXT_SBB) | BG_PRIO(0), 0, CLR_WHITE, 14, &japanese_smallFont, NULL);

    tte_init_chr4c(3,                                           // BG 3
                   BG_CBB(TILESET_TEXT) | BG_SBB(TILEMAP_TEXT), // Charblock 0; screenblock 10
                   0xF000,                                      // Screen-entry offset
                   bytes2word(                                  // Color attributes:
                       13,                                      // Text color
                       15,                                      // Shadow color
                       0,                                       // Paper
                       0),                                      // Special
                   CLR_WHITE,                                   // White text
                   BUILD_FONT,                                  // Custom font
                   NULL                                         // Use default chr4 renderer
    );
    tte_init_con();

    pal_bg_bank[15][INK_WHITE] = CLR_WHITE;              // White
    pal_bg_bank[15][INK_DARK_GREY] = 0b0000110001100010; // Dark Grey
    // 14 will be changed to game color

    // Set default variables
    char_index = 0;
    line_char_index = 0;
    text_exit = false;
}

int text_loop(int script)
{
    // we have restricted the dialog entries to 1024 bytes in the text_helper main.py
    // so we shouldn't run into problems when we only use 1 KB to contain a text entry.
    u8 diag_entry_text_buffer[1024];
    switch (script)
    {
    case SCRIPT_DEBUG:
        curr_line = transfer_script_params[0];
        break;

    case SCRIPT_TRANSFER:
        curr_line = transfer_script_params[T_SCRIPT_START];
        break;

    case SCRIPT_EVENT:
        curr_line = event_script_params[E_SCRIPT_START];
        break;
    }

    curr_text = (curr_line.has_text()) ? read_dialogue_text_entry(curr_line.get_text_entry_index(), PTGB_INDEX, diag_entry_text_buffer) : NULL;

    // tte_set_margins(LEFT, TOP, RIGHT, BOTTOM);
    if (script != SCRIPT_DEBUG)
    {
        while (true) // This loops through all the connected script objects
        {
            if (curr_text != NULL && curr_text[char_index] != 0xFF && curr_text[char_index] != 0xFB)
            {
                ptgb_write_textbox(curr_text, false, true,
                                   PTGB_INDEX, curr_line.get_text_entry_index(), false);
            }

            // wait_for_user_to_continue();

            line_char_index = 0;
            switch (script)
            {
            case BTN_TRANSFER:
                curr_line = transfer_script_params[text_next_obj_id(curr_line)];
                break;
            case BTN_EVENTS:
                curr_line = event_script_params[text_next_obj_id(curr_line)];
                break;
            }

            curr_text = (curr_line.has_text()) ? read_dialogue_text_entry(curr_line.get_text_entry_index(), PTGB_INDEX, diag_entry_text_buffer) : NULL;
            char_index = 0;

            if (text_exit)
            {
                hide_textbox();
                erase_textbox_tiles();
                tte_erase_screen();
                text_exit = false;
                return 0;
            }
        }
    }
    else // Debug script loop
    {
        u16 debug_charset[256];
        load_localized_charset(debug_charset, 3, ENGLISH);

        int text_section = 0;
        int text_key = 0;
        while (true)
        {
            bool exit = false;
            bool update_text = true;
            bool instant_text = false;
            VBlankIntrWait();
            while (!exit)
            {
                if (key_hit(KEY_LEFT))
                {
                    text_key = (text_key + (text_section_lengths[text_section] - 1)) % text_section_lengths[text_section];
                    update_text = true;
                }
                else if (key_hit(KEY_RIGHT))
                {
                    text_key = (text_key + 1) % text_section_lengths[text_section];
                    update_text = true;
                }
                else if (key_hit(KEY_UP))
                {
                    if (text_section == 0) {
                        text_section = NUM_TEXT_SECTIONS - 1;
                    }
                    else {
                        text_section = ((text_section + (NUM_TEXT_SECTIONS - 1)) % NUM_TEXT_SECTIONS);
                    }
                    update_text = true;
                }
                else if (key_hit(KEY_DOWN))
                {
                    text_section = (text_section + (NUM_TEXT_SECTIONS - 1)) % NUM_TEXT_SECTIONS;
                    update_text = true;
                }
                else if (key_hit(KEY_START) || key_hit(KEY_SELECT))
                {
                    instant_text = key_hit(KEY_START); // instant with start, not with select
                    exit = true;
                    tte_erase_line();
                }
                if (update_text)
                {
                    if (text_key >= text_section_lengths[text_section])
                    {
                        text_key = text_section_lengths[text_section] - 1;
                    }
                    if (text_section >= NUM_TEXT_SECTIONS)
                    {
                        text_section = NUM_TEXT_SECTIONS - 1;
                    }
                    tte_set_pos(0, 0);
                    tte_erase_rect(0, 0, 240, 160);
                    ptgb_write_debug(debug_charset, "(", true);
                    ptgb_write_debug(debug_charset, ptgb::to_string(text_section), true);
                    ptgb_write_debug(debug_charset, ", ", true);
                    ptgb_write_debug(debug_charset, ptgb::to_string(text_key), true);
                    ptgb_write_debug(debug_charset, ")", true);
                    update_text = false;
                }
                VBlankIntrWait();
            }

            line_char_index = 0;
            curr_text = read_dialogue_text_entry(text_key, text_section, diag_entry_text_buffer);
            char_index = 0;

            if (curr_text != NULL && curr_text[char_index] != 0xFF && curr_text[char_index] != 0xFB)
            {
                ptgb_write_textbox(curr_text, instant_text, true,
                                   text_section, text_key, true);
            }

            update_text = true;
            hide_textbox();
            tte_erase_rect(0, 0, H_MAX, V_MAX);

            if (text_exit)
            {
                text_exit = false;
                return 0;
            }
        }
    }
}

int text_next_obj_id(script_obj current_line)
{
    if (current_line.get_cond_id() == 0)
    {
        return current_line.get_true_index();
    }
    else
    {
        const bool ret = run_conditional(current_line.get_cond_id());
        VBlankIntrWait(); // this is needed to handle interrupts
        if (ret)
        {
            return current_line.get_true_index();
        }
        return current_line.get_false_index();
    }
}

void set_text_exit()
{
    text_exit = true;
    VBlankIntrWait(); // This removes the "A Hit" when exiting the text
}

// Implement a version that creates the textbox as well
int ptgb_write_textbox(const byte *text, bool instant, bool waitForUser,
                       int text_section, int text_key, bool eraseMainBox)
{
    tte_erase_rect(0, 0, H_MAX, V_MAX);
    erase_textbox_tiles();
    create_textbox(text_section, text_key, eraseMainBox);
    // Set up Fennel if we are in a PTGB dialogue box
    if (get_curr_flex_background() == FBG_Fennel && text_section == PTGB_INDEX)
    {
        load_flex_background(FBG_Fennel, 2);
    }
    int out = ptgb_write(text, instant, 9999, text_box_type_tables[text_section][text_key]); // This is kinda silly but it'll work.
    if (waitForUser)
    {
        
        int right = H_MAX - 5;
        int bottom = V_MAX - 5;
        wait_for_user_to_continue(right, bottom);
    }
    if (eraseMainBox)
    {
        tte_erase_rect(0, 0, H_MAX, V_MAX);
        hide_textbox();
        erase_textbox_tiles();
    }
    return out;
}

// Implement a version that just writes the whole string
int ptgb_write_simple(const byte *text, bool instant)
{
    return ptgb_write(text, instant, 9999, -1); // This is kinda silly but it'll work.
}

// Re-implementing TTE's "tte_write" to use the gen 3 character encoding chart
int ptgb_write(const byte *text, bool instant, int length, int box_type)
{
    instant = instant || g_debug_options.instant_text_speed;
    if (text == NULL)
        return 0;

    int left, top, right, bottom;

    if (box_type == -1)
    {
        left = 0;
        top = 0;
        right = H_MAX;
        bottom = V_MAX;
    }
    else
    {
        left = 8 * (box_type_info[box_type][BOX_TYPE_VAL_START_TILE_X] + 1);
        top = 8 * (box_type_info[box_type][BOX_TYPE_VAL_START_TILE_Y] + 1);
        right = left + box_type_info[box_type][BOX_TYPE_VAL_PIXELS_PER_LINE];
        bottom = top + box_type_info[box_type][BOX_TYPE_VAL_NUM_OF_LINES] * 16;
    }

    uint ch, gid;
    char *str = (char *)text;
    TTC *tc = tte_get_context();
    TFont *font;
    int num = 0;

    while ((ch = *str) != 0xFF && num < length)
    {
        if (get_frame_count() % 2 == 0 || key_held(KEY_B) || key_held(KEY_A) || instant)
        {
            str++;
            switch (ch)
            {
            case 0xFA:
                if (g_debug_options.display_control_char)
                {
                    tc->drawgProc(0x79);
                }
                wait_for_user_to_continue(right, bottom);
                scroll_text(instant, tc, true, left, top, right, bottom);
                break;
            case 0xFB:
                if (g_debug_options.display_control_char)
                {
                    tc->drawgProc(0xB9);
                }
                wait_for_user_to_continue(right, bottom);
                tte_erase_rect(left, top, right, bottom);
                tte_set_pos(left, top);
                break;
            case 0xFC:
                ch = *str;
                str++;
                num += 1;
                if (g_debug_options.display_control_char)
                {
                    for (uint i = 0; i < ch; i++)
                    {
                        tc->drawgProc(0xB9);
                    }
                }
                else
                {
                    tc->cursorX += tc->font->widths[0xFC] * ch;
                }
                break;
            case 0xFE:
                if (g_debug_options.display_control_char)
                {
                    tc->drawgProc(0xEF);
                }
                tc->cursorY += tc->font->charH;
                tc->cursorX = tc->marginLeft;
                break;
            default:

                // Get glyph index and call renderer
                font = tc->font;
                gid = ch - font->charOffset;
                if (tc->charLut)
                    gid = tc->charLut[gid];

                // Character wrap
                int charW = font->widths ? font->widths[gid] : font->charW;

                // Draw and update position
                tc->drawgProc(gid);
                tc->cursorX += charW;
            }
            num += 1;
        }
        if (get_curr_flex_background() == FBG_Fennel && !instant)
        {
            fennel_speak(((num / 4) % 4) + 1);
        }
        if (!instant)
        {
            VBlankIntrWait();
        }
    }

    // Return characters used (PONDER: is this really the right thing?)
    return 0; // str - text;
}
// This is mostly used for debug stuff, I shouldn't rely it on it much.
int ptgb_write_debug(const u16 *charset, const char *text, bool instant)
{
    byte temp_holding[256];
    int i;
    for (i = 0; i < 256; i++)
    {
        if (text[i] == '\0')
        {
            temp_holding[i] = 0xFF;
            i = 256;
        }
        else if (text[i] == '\n')
        {
            temp_holding[i] = 0xFE;
        }
        else
        {
            u16 utf16_char;
            // we need to use this conversion function in order to convert char values >= 0x80
            // correctly to UTF-16 (which is used by the charset)
            convert_utf8_to_utf16_char((const u8 *)(text + i), utf16_char);
            // WARNING: the conversion of the text characters from u8 to u16 done here, is incorrect
            // for every character value >= 0x80. The character set uses UTF-16 to represent characters.
            // But the input text is in UTF-8.
            temp_holding[i] = get_char_from_charset(charset, utf16_char);
        }
    }
    return ptgb_write_simple(temp_holding, instant);
}

void wait_for_user_to_continue(int right, int bottom)
{
    if (get_curr_flex_background() == FBG_Fennel)
    {
        if (get_missingno_enabled())
        {
            fennel_speak(0);
            fennel_blink(4);
        }
        else
        {
            fennel_speak(0);
        }
    }

    //obj_set_pos(scroll_indicator_red, right-16, bottom-11);
    //obj_set_pos(scroll_indicator_gray, right-16, bottom-10);
    //obj_unhide(scroll_indicator_red, 0);
    
    VBlankIntrWait();
    while (!(key_hit(KEY_A) || key_hit(KEY_B)))
    {
        /*
        int frameCount = get_frame_count();
        if (frameCount % 60 == 0) {
            obj_unhide(scroll_indicator_red, 0);
            obj_hide(scroll_indicator_gray);
        }
        else if (frameCount % 30 == 0) {
            obj_hide(scroll_indicator_red);
            obj_unhide(scroll_indicator_gray, 0);
        }
        */
        VBlankIntrWait();
    }
    //obj_hide(scroll_indicator_red);
    //obj_hide(scroll_indicator_gray);
}

void scroll_text(bool instant, TTC *tc, bool scrollUp, int left, int top, int right, int bottom)
{
    int direction = scrollUp ? 1 : -1;

    for (int i = 1; i <= tc->font->charH; i++)
    {
        REG_BG3VOFS = i * direction;
        tte_erase_rect(left, top - tc->font->charH, right, top + (i * direction));
        if (!instant) // This is somewhat of a silly way to do this - can be optimized
        {
            VBlankIntrWait();
        }
    }
    REG_BG3VOFS = 0;
    
    // The map starts at tile 0 in the top left, increases by 1 as you go down, and then loops back at the top.
    if (scrollUp)
    {
        for (int i = 0; i < 30; i++)
        {
            tonccpy(&tile_mem[TILESET_TEXT][0 + (i * 20)], &tile_mem[TILESET_TEXT][2 + (i * 20)], 20 * 32);
        }
    }
    else
    {
        for (int i = 29; i >= 0; i--)
        {
            // Due to the order of tonccpy, we need to store the data in a temporary buffer
            byte buffer[20 * 32];
            tonccpy(&buffer, &tile_mem[TILESET_TEXT][0 + (i * 20)], 20 * 32);
            tonccpy(&tile_mem[TILESET_TEXT][2 + (i * 20)], &buffer, 20 * 32);
        }
    }
    // Remove text that went outside of the box and set the position
    tte_erase_rect(left, scrollUp ? (top - tc->font->charH) : bottom, right, scrollUp ? top : (bottom + tc->font->charH));

    tc->cursorY = (scrollUp ? bottom - tc->font->charH : top); // The newline will trigger after this and move it down a line
    tc->cursorX = left;
}