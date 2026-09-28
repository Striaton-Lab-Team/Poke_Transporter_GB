#include "text_tables.h"
#include "translated_text.h"
#include "compression_chunks.h"

void get_text_table_chunks(u32 tableIndex, const u8 ***chunkList, u32 *numChunks, u32 *chunkSize)
{
    // every table except for RSEFRLG is 2048 bytes per chunk, so we can set that by default and only change it for RSEFRLG
    *chunkSize = 2048;
    switch(tableIndex)
    {
    case PTGB_INDEX:
        *chunkList = PTGB_chunk_list;
        *numChunks = sizeof(PTGB_chunk_list) / sizeof(PTGB_chunk_list[0]);
        break;
    case GB_INDEX:
        *chunkList = GB_chunk_list;
        *numChunks = sizeof(GB_chunk_list) / sizeof(GB_chunk_list[0]);
        break;
    case GENERAL_INDEX:
        *chunkList = GENERAL_chunk_list;
        *numChunks = sizeof(GENERAL_chunk_list) / sizeof(GENERAL_chunk_list[0]);
        break;
    case CREDITS_INDEX:
        *chunkList = CREDITS_chunk_list;
        *numChunks = sizeof(CREDITS_chunk_list) / sizeof(CREDITS_chunk_list[0]);
        break;
    case PKMN_NAMES_INDEX:
        *chunkList = PKMN_NAMES_chunk_list;
        *numChunks = sizeof(PKMN_NAMES_chunk_list) / sizeof(PKMN_NAMES_chunk_list[0]);
        break;
    case TUTORIAL_INDEX:
        *chunkList = TUTORIAL_chunk_list;
        *numChunks = sizeof(TUTORIAL_chunk_list) / sizeof(TUTORIAL_chunk_list[0]);
        break;
    default:
        *chunkList = NULL;
        *numChunks = 0;
        break;
    }
}