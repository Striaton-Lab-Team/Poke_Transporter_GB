#ifndef _TEXT_TABLES_H
#define _TEXT_TABLES_H

#include "typeDefs.h"

extern "C"
{

/**
 * @brief This function returns a list of pointers to the text table chunks and the number of chunks.
 */
void get_text_table_chunks(u32 tableIndex, const u8 ***chunkList, u32 *numChunks, u32 *chunkSize);

}

#endif