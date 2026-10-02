#include <tonc.h>

#include "global_frame_controller.h"
#include "background_engine.h"
#include "libraries/gba-link-connection/LinkCableMultiboot.hpp"
#include "text_engine.h"
#include "translated_text.h"
#include "FileContainerReader.h"
#include "text_tables.h"

void multiboot_upload_screen()
{
	u8 decompression_buffer[2048];
	u8 lineBuffer[1024];
	const u8 **chunkList;
	u32 numChunks;
	u32 chunkSize;

	get_text_table_chunks(GENERAL_INDEX, &chunkList, &numChunks, &chunkSize);
	FileContainerReader general_text_reader(chunkList, numChunks, chunkSize);
	LinkCableMultiboot linkCableMultiboot;

	general_text_reader.init(decompression_buffer, sizeof(decompression_buffer));

	// multiboot_show_textbox();
	general_text_reader.readFile(GENERAL_send_multiboot_instructions, lineBuffer);
	ptgb_write_textbox(lineBuffer, true,
					   false, GENERAL_INDEX, GENERAL_send_multiboot_instructions, false);

	// wait for key press
	do
	{
		VBlankIntrWait();
	} while (!key_hit(KEY_A) && !key_hit(KEY_B));

	if (key_hit(KEY_B))
	{
		// cancel
		return;
	}

	// start upload
	// multiboot_show_textbox();
	general_text_reader.readFile(GENERAL_send_multiboot_wait, lineBuffer);
	ptgb_write_textbox(lineBuffer, true,
					   false, GENERAL_INDEX, GENERAL_send_multiboot_wait, false);
	VBlankIntrWait();

	const u32 romSize = 256 * 1024; // EWRAM = 256 KB
	LinkCableMultiboot::Result multibootResult = linkCableMultiboot.sendRom(
		((const u8 *)MEM_EWRAM),
		romSize,
		[]()
		{
			u16 keys = ~REG_KEYS & KEY_ANY;
			return keys & KEY_B;
			// (when this returns true, the transfer will be canceled)
		});
	// show result
	// multiboot_show_textbox();
	u32 fileIndex = (multibootResult == LinkCableMultiboot::Result::SUCCESS) ? GENERAL_send_multiboot_success : GENERAL_send_multiboot_failure;
	general_text_reader.readFile(fileIndex, lineBuffer);

	ptgb_write_textbox(lineBuffer, true, false, GENERAL_INDEX, fileIndex, false);

	// wait for keypress again.
	do
	{
		VBlankIntrWait();
	} while (!key_hit(KEY_A));
}