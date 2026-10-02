#ifndef MIRROR_H
#define MIRROR_H
#include <tonc.h>
#include "rom_data.h"

#define SAVE_A_OFFSET       0x00000000 // Offset of Game Save A
#define SAVE_B_OFFSET       0x0000E000 // Offset of Game Save B

#define SECTION_ID_OFFSET   0x00000FF4
#define SAVE_INDEX_OFFSET   0x00000FFC

extern vu32 newest_save_offset;
extern vu32 memory_section_array[];
extern char mem_name;
extern u8 global_memory_buffer[0x1000];
extern rom_data curr_GBA_rom;

/**
 * @brief Given that the save sections within a save slot are not saved in a fixed order,
 * This function will walk through the save slot to fill the memory_section_array map.
 * 
 * After calling this, memory_section_array will point to the save offset by each section ID. 
 * So memory_section_array[0] will point to the save offset of section 0, etc.
 */
void initialize_memory_locations();

void print_mem_section();
//bool insert_pokemon(Pokemon party_array[], int num);
void reverse_endian(u8 *data, size_t size);

/**
 * @brief This function will update the checksum in the given sector buffer.
 * It assumes that the sector buffer contains one of the save data sections.
 * 
 * @param sector_buffer The buffer containing the save data section.
 * @param hall_of_fame This is needed to indicate if you're trying to save the hall of fame section. The checksum offset is slightly different there.
 */
void update_memory_buffer_checksum(u8 *sector_buffer, bool hall_of_fame);
bool read_flag(u16 flag_id);
bool compare_map_and_npc_data(int map_bank, int map_id, int npc_id);

#endif