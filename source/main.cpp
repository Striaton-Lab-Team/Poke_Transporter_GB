#include <tonc.h>
#include <cstdlib>
#include "libstd_replacements.h"
#include "flash_mem.h"
#include "interrupt.h"
#include "gb_link.h"
#include "link_handler.h"
#include "random.h"
#include "text_engine.h"
#include "background_engine.h"
#include "pokemon_data.h"
#include "script_array.h"
#include "sprite_data.h"
#include "button_handler.h"
#include "button_menu.h"
#include "dbg/debug_mode.h"
#include "dbg/debug_menu.h"
#include "dbg/ptgb_mgba_print.h"
#include "dex_handler.h"
#include "pokedex.h"
#include "global_frame_controller.h"
#include "pkmn_font.h"
#include "ptgb_save_data_manager.h"
#include "mystery_gift_injector.h"
#include "multiboot_upload.h"
#include "rom_data.h"
#include "libraries/Pokemon-Gen3-to-Gen-X/include/save.h"
#include "FileContainerReader.h"
#include "translated_text.h"
#include "text_tables.h"
#include "custom_malloc.h"
#include "sound.h"

/*

TODO:
- Add in putting item from Pkmn to bag in gen 2
- Determine if transfered Shiny Pokemon are square/star sparkles
- Music and sound effects
- Wii Channel
- Events
- MissingNo/Enigma Berry
- Text translations
- Add support for other languages
- Add in dolls for gen 2/3
- Doxygen generation
--------
*/

int delay_counter = 0;
int curr_selection = 0;
bool skip = true;
rom_data curr_GBA_rom;
Button_Menu yes_no_menu(1, 2, 40, 24, false);

// (R + G*32 + B*1024)
#define RGB(r, g, b) (r + (g * 32) + (b * 1024))

void load_graphics()
{
	//  Load opening background first so it hides everything else
	load_flex_background(FBG_Opening, 1);
	load_background();
	load_textbox_background();
	load_eternal_sprites();

	// Set up global yes no button
	yes_no_menu.clear_vector();
	yes_no_menu.add_button(Button(button_yes), true);
	yes_no_menu.add_button(Button(button_no), false);
}

void initialization_script(void)
{
	// Initializations
	REG_DISPCNT = DCNT_BLANK | DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_BG3 | DCNT_OBJ | DCNT_OBJ_1D;

	// Disable for save data read/write
	REG_IME = 0;
	REG_IE = 0;

	irq_init(&isr_master_nest);

	// Sound bank init
	// irq_init(NULL);
	// irq_enable(II_VBLANK);
	// This currently crashes when you try to transfer a Pokemon:
	// sound_init();

	// Link Cable init
  	irq_add(II_TIMER3, linkCableIRQ);

	// Graphics init
	irq_add(II_VBLANK, global_next_frame);
	irq_enable(II_VBLANK);
	oam_init(obj_buffer, 128);
	load_graphics();

	// Prepare text engine for dialogue
	init_text_engine();
	ptgb_mgba_init();

	// Set the random seed
	rand_set_seed(0x1216);

	// Clean up the main screen quick

	VBlankIntrWait();
	REG_DISPCNT &= ~DCNT_BLANK;

	PTGB_MGBA_INFO("Hello from PTGB!");
};

// attribute noinline is used to make sure it doesn't get inlined and permanently use IWRAM for the decompression_buffer
void __attribute__((noinline)) game_load_error(void)
{
	u8 general_text_buffer[2048];
	u8 lineBuffer[1024];
	const u8 **chunkList;
	u32 numChunks;
	u32 chunkSize;

	get_text_table_chunks(GENERAL_INDEX, &chunkList, &numChunks, &chunkSize);
	FileContainerReader text_reader(chunkList, numChunks, chunkSize);

	text_reader.init(general_text_buffer, sizeof(general_text_buffer));

	BG_TEXTBOX = (BG_TEXTBOX & ~BG_PRIO_MASK) | BG_PRIO(1);
	text_reader.readFile(GENERAL_cart_load_error, lineBuffer);
	ptgb_write_textbox(lineBuffer, true, false, GENERAL_INDEX, GENERAL_cart_load_error, false);

	// key_poll();
	do
	{
		VBlankIntrWait();
	} while (!key_hit(KEY_A) && !key_hit(KEY_SELECT));

	tte_erase_rect(0, 0, H_MAX, V_MAX);
	erase_textbox_tiles();

	if (key_hit(KEY_SELECT))
	{
		// We also want to give the option in this screen to upload the multiboot rom to another GBA.
		// This can be useful when the user wants to work with a flashcart in single rom mode.
		// The EZ Flash Omega (DE) for instance, triggers a reset of the gba if you insert it while the GBA is turned on.
		// So the only way to work with it, is to boot Poke Transporter GB over multiboot and have the flashcart already inserted.
		// It would be a shame not to support this flashcart, because it's awesome for pokémon fans. After all: it supports ds transfer
		// and should support connecting with the gamecube games.
		multiboot_upload_screen();
		return;
	}
	delay_counter = 0;

	while (delay_counter < 60)
	{
		delay_counter++;
		VBlankIntrWait();
	}
};

// avoid inlining to avoid permanently storing the credits_decompression_buffer in IWRAM
int __attribute__((noinline)) credits()
{
	u8 credits_decompression_buffer[2048];
	u8 lineBuffer[1024];
	const u8 **chunkList;
	u32 numChunks;
	u32 chunkSize;

	get_text_table_chunks(CREDITS_INDEX, &chunkList, &numChunks, &chunkSize);
	FileContainerReader creditsReader(chunkList, numChunks, chunkSize);
	u32 curr_credits_num = 0;

	creditsReader.init(credits_decompression_buffer, sizeof(credits_decompression_buffer));
	bool update = true;

	while (true)
	{
		if (update)
		{
			creditsReader.readFile(curr_credits_num, lineBuffer);
			ptgb_write_textbox(lineBuffer, true, false, CREDITS_INDEX, curr_credits_num, false);
			update = false;
		}

		if (key_hit(KEY_B))
		{
			tte_erase_rect(0, 0, H_MAX, V_MAX);
			hide_textbox();
			erase_textbox_tiles();
			return 0;
		}
		if (key_hit(KEY_LEFT) && curr_credits_num > 0)
		{
			curr_credits_num--;
			update = true;
		}
		if (key_hit(KEY_RIGHT) && curr_credits_num < (creditsReader.getNumberOfFiles() - 1))
		{
			curr_credits_num++;
			update = true;
		}

		VBlankIntrWait();
	}
};

// avoid inlining to avoid permanently storing the tutorial_decompression_buffer in IWRAM
int __attribute__((noinline)) tutorial()
{

	u8 tutorial_decompression_buffer[2048];
	u8 lineBuffer[1024];
	const u8 **chunkList;
	u32 numChunks;
	u32 chunkSize;

	get_text_table_chunks(TUTORIAL_INDEX, &chunkList, &numChunks, &chunkSize);
	FileContainerReader tutorialReader(chunkList, numChunks, chunkSize);
	u32 curr_tutorial_num = 0;

	tutorialReader.init(tutorial_decompression_buffer, sizeof(tutorial_decompression_buffer));
	bool update = true;

	while (true)
	{
		if (update)
		{
			tutorialReader.readFile(curr_tutorial_num, lineBuffer);
			ptgb_write_textbox(lineBuffer, true, false, TUTORIAL_INDEX, curr_tutorial_num, false);
			update = false;
		}

		if (key_hit(KEY_B))
		{
			tte_erase_rect(0, 0, H_MAX, V_MAX);
			hide_textbox();
			erase_textbox_tiles();
			return 0;
		}
		if (key_hit(KEY_LEFT) && curr_tutorial_num > 0)
		{
			curr_tutorial_num--;
			update = true;
		}
		if (key_hit(KEY_RIGHT) && curr_tutorial_num < (tutorialReader.getNumberOfFiles() - 1))
		{
			curr_tutorial_num++;
			update = true;
		}

		VBlankIntrWait();
	}
};

// attribute noinline is used to avoid permanently storing the general_text_table_buffer in IWRAM
int __attribute__((noinline)) main_menu_loop()
{
	uint8_t general_text_table_buffer[2048];
	u8 lineBuffer[1024];
#define NUM_MENU_OPTIONS 4
	const uint8_t menu_options[NUM_MENU_OPTIONS] = {GENERAL_option_transfer, GENERAL_option_dreamdex, GENERAL_option_credits, GENERAL_option_tutorial};
	int return_values[NUM_MENU_OPTIONS] = {BTN_TRANSFER, BTN_POKEDEX, BTN_CREDITS, BTN_TUTORIAL};
	const u8 **chunkList;
	u32 numChunks;
	u32 chunkSize;

	get_text_table_chunks(GENERAL_INDEX, &chunkList, &numChunks, &chunkSize);
	FileContainerReader text_reader(chunkList, numChunks, chunkSize);
	bool update = true;
	u16 test = 0;

	text_reader.init(general_text_table_buffer, sizeof(general_text_table_buffer));

	while (true)
	{
		if (update)
		{
			for (int i = 0; i < NUM_MENU_OPTIONS; i++)
			{
				text_reader.readFile(menu_options[i], lineBuffer);
				int string_length = get_string_length(lineBuffer);
				int x = ((240 - string_length) / 2);
				tte_set_pos(x, ((i * (16 + 6)) + 70));
				if (i == curr_selection)
				{
					tte_set_ink(INK_WHITE);
				}
				else
				{
					tte_set_ink(INK_ROM_COLOR);
				}
				ptgb_write_simple(lineBuffer, true);
				test++;
			}
		}

		update = true;
		if (key_hit(KEY_DOWN))
		{
			curr_selection = ((curr_selection + 1) % NUM_MENU_OPTIONS);
		}
		else if (key_hit(KEY_UP))
		{
			if (curr_selection == 0) {
				curr_selection = NUM_MENU_OPTIONS - 1;
			}
			else {
				curr_selection = ((curr_selection + (NUM_MENU_OPTIONS - 1)) % NUM_MENU_OPTIONS);
			}
		}
		else if (key_hit(KEY_A))
		{
			tte_erase_rect(0, test, H_MAX, V_MAX);
			return return_values[curr_selection];
		}
		else if ((key_held(KEY_L) && key_held(KEY_R)))
		{
			return BTN_DEBUG_MENU;
		}
		else
		{
			update = false;
		}
		VBlankIntrWait();
	}
}

// Gears of Progress
static void show_gears_of_progress()
{
	REG_BG1VOFS = 0;
	delay_counter = 0;
	while (delay_counter < (15 * 60))
	{
		VBlankIntrWait();
		delay_counter++;
		if (key_hit(KEY_A))
		{
			delay_counter = (15 * 60);
		}
	}
}

// split off from the main function in order to keep the scope of the variables limited to the execution of this function
// otherwise they stick around for the entire runtime of the program
// the attribute noinline is used to prevent the compiler from inlining this function back into the main function
// this decision was based on the output of build/main.su after adding the -fstack-usage compile flag
static void __attribute__((noinline)) show_intro()
{
	u8 general_text_table_buffer[2048];
	u8 lineBuffer[1024];
	const u8 **chunkList;
	u32 numChunks;
	u32 chunkSize;

	get_text_table_chunks(GENERAL_INDEX, &chunkList, &numChunks, &chunkSize);
	FileContainerReader general_text_reader(chunkList, numChunks, chunkSize);
	bool start_pressed = false;

	general_text_reader.init(general_text_table_buffer, sizeof(general_text_table_buffer));

	tte_set_ink(INK_ROM_COLOR);
	general_text_reader.readFile(GENERAL_intro_legal, lineBuffer);
	ptgb_write_textbox(lineBuffer, true, true, GENERAL_INDEX, GENERAL_intro_legal, true);

	show_gears_of_progress();

	BG_FLEX = BG_FLEX | BG_PRIO(3);

	// key_poll(); // Reset the keys
	curr_GBA_rom.load_rom(false);

	obj_set_pos(ptgb_logo_l, 56, 12);
	obj_set_pos(ptgb_logo_r, 56 + 64, 12);
	obj_unhide_multi(ptgb_logo_l, 1, 2);

	REG_BLDCNT = BLD_BUILD(BLD_BG3, BLD_BG0, 1);

	general_text_reader.readFile(GENERAL_press_start, lineBuffer);

	tte_set_pos(0, 12 * 8);
	tte_set_ink(INK_DARK_GREY);
	ptgb_write_simple(lineBuffer, true);

	int fade = 0;
	while (!start_pressed)
	{
		fade = abs(((get_frame_count() / 6) % 24) - 12);
		VBlankIntrWait();
		start_pressed = key_hit(KEY_START) | key_hit(KEY_A);
		REG_BLDALPHA = BLDA_BUILD(0b10000, fade);
	}
}

int main(void)
{
	// Clear VRAM. We use it as extra BSS.
	RegisterRamReset(RESET_VRAM);
	malloc_init_default_pool();
	initialization_script();

	// Set colors based on current ROM
	set_background_pal(0, false, false);

	show_intro();

	// key_poll();
	tte_erase_rect(0, 0, H_MAX, V_MAX);
	REG_BLDALPHA = BLDA_BUILD(0b10000, 0); // Reset fade

	//  Check if the game has been loaded correctly.
	bool debug = false;
	while (!curr_GBA_rom.load_rom(debug))
	{
		if (g_debug_options.ignore_game_pak)
		{
			debug = true;
		}
		else
		{
			obj_hide_multi(ptgb_logo_l, 2);
			VBlankIntrWait();
			game_load_error();
			// initialization_script();
		}
	}

	// Initialize memory and save data after loading the game
	BG_TEXTBOX = BG_TEXTBOX | BG_PRIO(3);
	init_bank();
	initialize_memory_locations();
	load_custom_save_data();

	set_background_pal(curr_GBA_rom.gamecode, false, true);

	if (!get_tutorial_flag() || g_debug_options.force_tutorial)
	{
		obj_hide_multi(ptgb_logo_l, 2);
		load_flex_background(FBG_Fennel, 2);
		text_loop(BTN_TRANSFER);
		initialize_save_data();
		// TODO: We should be able to test for a Bootleg rom in here- if the save data isn't written, then it is bootleg.
	}

	obj_unhide_multi(ptgb_logo_l, 1, 2);

	// MAIN LOOP
	while (true)
	{
		if (DEBUG_MODE && false) // This isn't really needed anymore
		{
			print_mem_section();
			curr_GBA_rom.print_rom_info();
		}
		load_flex_background(FBG_Main_Menu, 2);

		obj_unhide_multi(ptgb_logo_l, 1, 2);
		obj_set_pos(ptgb_logo_l, 56, 12);
		obj_set_pos(ptgb_logo_r, 56 + 64, 12);

		switch (main_menu_loop())
		{
		case (BTN_TRANSFER):
			tte_set_ink(INK_DARK_GREY);
			obj_hide_multi(ptgb_logo_l, 2);
			load_flex_background(FBG_Fennel, 3);
			text_loop(SCRIPT_TRANSFER);
			break;
		case (BTN_POKEDEX):
			if (get_tutorial_flag())
			{
				obj_hide_multi(ptgb_logo_l, 2);
				// global_next_frame();
				load_flex_background(FBG_Dex, 2);
				set_background_pal(curr_GBA_rom.gamecode, true, false);
				pokedex_loop();
				load_flex_background(FBG_Dex, 3);
				set_background_pal(curr_GBA_rom.gamecode, false, false);
			}
			break;
		case (BTN_CREDITS):
			tte_set_ink(INK_DARK_GREY);
			BG_FLEX = (BG_FLEX & ~BG_PRIO_MASK) | BG_PRIO(3);
			obj_hide_multi(ptgb_logo_l, 2);
			credits();
			break;
		case (BTN_TUTORIAL):
			tte_set_ink(INK_DARK_GREY);
			BG_FLEX = (BG_FLEX & ~BG_PRIO_MASK) | BG_PRIO(3);
			obj_hide_multi(ptgb_logo_l, 2);
			tutorial();
			break;
		case (BTN_EVENTS):
			obj_hide_multi(ptgb_logo_l, 2);
			text_loop(SCRIPT_EVENT);
			break;
#if ENABLE_DEBUG_MENU
		case (BTN_DEBUG_MENU):
			obj_hide_multi(ptgb_logo_l, 2);
			load_flex_background(FBG_Fennel, 3);
			show_debug_menu();
			break;
#endif
		default:
			VBlankIntrWait();
		}
	}
}