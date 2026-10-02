#include "custom_malloc.h"

#if USE_CUSTOM_MALLOC
#include <stddef.h>
#include <string.h>
#include <tonc.h>
///
/// This module is implemented to reduce the rom size and reduce IWRAM consumption (__malloc_av -> 1 KB)
/// However, we need to be careful to make a heap available at the right times.
/// This implementation stubs malloc_r, which is called by newlib c internally
/// That means we MUST avoid using:
/// printf, sprintf, fprintf, etc. (use iprintf from libtonc, nanoprintf or dprint alternatives)
/// fopen, freopen
/// strdup
/// locale / setlocale
/// atexit / exit / __cxa_atexit (especially in C++)
///

/// This module implements a bitmap block allocator.
/// We define a page size (mem_page_size), which is the minimum allocation unit for this malloc() implementation.
/// The allocator has a bitmap (g_alloc_map) which tracks the allocation of each page in the pool as a single bit.
/// 1 means allocated, 0 means free
/// The alloc map is structured to have a list of 32 bit unsigned integers, as that is the native unit the ARM cpu and memory bus
/// work with. So using these is the most efficient way to do it.
/// The allocator keeps track of the last position it was at while scanning and wraps around to the beginning of the allocation map
/// if necessary to find free space.


#define ALLOC_MAP_START_BIT 0x80000000
#define MALLOC_MEM_POOL_PAGE_SIZE 4

extern char __end__;

// warning: this static allocation of g_alloc_map limits the maximum g_mem_pool size to 8192 bytes.
// TODO: if this is not enough, we could change the code to allow a user to set their g_alloc_map with malloc_set_mem_pool()
static EWRAM_DATA uint32_t g_alloc_map[CUSTOM_MALLOC_POOL_SIZE / (32 * MALLOC_MEM_POOL_PAGE_SIZE)];
static uint32_t *g_alloc_map_ptr = g_alloc_map;
static uint32_t g_alloc_map_bit = ALLOC_MAP_START_BIT;

static const uint32_t mem_page_size = MALLOC_MEM_POOL_PAGE_SIZE;

// For some reason, __end__ only gives the right value if we use it directly.
// We can't store it in a variable.
// But if we create an inline function, then we're basically doing the same thing as using it directly.
static inline uint8_t *get_heap_start(void)
{
    return (uint8_t *)&__end__;
}

/// @brief This function calculates the absolute page offset/count from the start of the alloc_map until
/// the specified position in the alloc map
static uint16_t calculate_page_offset(uint32_t *alloc_map_ptr, uint32_t alloc_map_bit)
{
	uint16_t num_pages = (alloc_map_ptr - g_alloc_map) * 32;
	uint32_t bit_check = ALLOC_MAP_START_BIT;
	
	while(bit_check != alloc_map_bit)
	{
		++num_pages;
		bit_check >>= 1;
	}
	
	return num_pages;
}

/// @brief This function converts a position in the alloc map into the associated pointer in the actual memory pool.
static uint8_t* convert_alloc_map_position_into_mem_pool_ptr(uint32_t *start, uint32_t bit_start)
{
	const uint16_t page_offset = calculate_page_offset(start, bit_start);
	return get_heap_start() + (page_offset * mem_page_size);
}

/// @brief This function converts a memory pool pointer into a position in the associated allocation map.
/// @param ptr pointer in the memory pool
/// @param alloc_start output variable to indicate the start of the alloc map entry.
/// @param bit_start output variable to indcate the specific bit position inside that alloc map entry.
static void convert_mem_pool_ptr_into_alloc_map_position(uint8_t *ptr, uint32_t **alloc_start, uint32_t *bit_start)
{
	const uint16_t page_offset = (ptr - get_heap_start()) / mem_page_size;
	*alloc_start = g_alloc_map + (page_offset / 32);
	*bit_start = ALLOC_MAP_START_BIT >> (page_offset % 32);
}

/// @brief This function implements a speedup while scanning the allocation map for free space:
/// If the current bit position is the first one, it will try to skip entire alloc map entries if all bits are set.
/// This is much faster than walking through each bit individually.
/// @param end_entry Since the function can loop, we need to provide an entry position as a stop condition
/// to avoid an infinite loop
static void skip_wholy_allocated_map_entries(uint32_t *end_entry)
{
	const uint32_t *g_alloc_map_end = g_alloc_map + (sizeof(g_alloc_map) / sizeof(uint32_t));
	if(g_alloc_map_ptr == end_entry)
	{
		return;
	}

	while(*g_alloc_map_ptr == 0xFFFFFFFF)
	{
		++g_alloc_map_ptr;
		if(g_alloc_map_ptr >= g_alloc_map_end)
		{
			g_alloc_map_ptr = g_alloc_map;
		}
		if(g_alloc_map_ptr == end_entry)
		{
			return;
		}
	}
}

/// @brief This function advances the alloc_map_bit and possibly the alloc_map entry itself.
/// It also takes care of wraparound when we reach the end with alloc_map_ptr
static void advance_alloc_map_bit(uint32_t **alloc_map_ptr, uint32_t *alloc_map_bit, bool *looped)
{
	(*alloc_map_bit) >>= 1;
	if(!(*alloc_map_bit))
	{
		const uint32_t *g_alloc_map_end = g_alloc_map + (sizeof(g_alloc_map) / sizeof(uint32_t));
		++(*alloc_map_ptr);
		if((*alloc_map_ptr) >= g_alloc_map_end)
		{
			(*alloc_map_ptr) = g_alloc_map;
			if(looped)
			{
				*looped = true;
			}
		}
		
		(*alloc_map_bit) = ALLOC_MAP_START_BIT;
		
	}
}

/// @brief This function checks for num_pages of free space at the specified alloc map position.
static bool check_block_free(uint32_t **cur_entry, uint32_t *cur_bit, uint16_t num_pages, bool *looped)
{
	while(num_pages)
	{
		if(((**cur_entry) & (*cur_bit)) == 0)
		{
			--num_pages;
			advance_alloc_map_bit(cur_entry, cur_bit, looped);
			if(*cur_entry == g_alloc_map && *cur_bit == ALLOC_MAP_START_BIT)
			{
				// we looped in the middle of a block. consider it allocated.
				break;
			}
		}
		else
		{
			// the page we're currently considering is already allocated.
			// advance the bit position again for the next call, but the result of this function
			// is that we didn't find a large enough free block
			advance_alloc_map_bit(cur_entry, cur_bit, looped);
			break;
		}
	}
	return !num_pages;
}

/// @brief This function marks a block of pages as "allocated" in the allocation map at the specified map position
static void mark_block_allocated(uint32_t *cur_entry, uint32_t cur_bit, size_t num_pages)
{
	while(num_pages)
	{
		*cur_entry |= cur_bit;
		--num_pages;
		advance_alloc_map_bit(&cur_entry, &cur_bit, NULL);
	}
}

/// @brief This function marks a block of pages as "free" in the allocation map at the specified map position
static void mark_block_free(uint32_t *cur_entry, uint32_t cur_bit, size_t num_pages)
{
	while(num_pages)
	{
		// clear bit
		*cur_entry &= ~cur_bit;
		--num_pages;
		advance_alloc_map_bit(&cur_entry, &cur_bit, NULL);
	}
}

/// @brief This function calculates the number of pages needed to allocate at least the requested size.
/// It does add 1 additional page for storing the allocation size (in pages) and may add another one to ensure
/// the returned allocation is aligned to mem_page_size
static uint16_t get_num_pages_for_size(size_t size)
{
	// reserve 1 additional page for storing the number of pages (and use/waste a whole page because of alignment)
	// we need this as a "header" to be able to know how many pages to free().
	uint16_t num_pages = (size / mem_page_size) + 1;
	if(size % mem_page_size)
	{
		++num_pages;
	}
	return num_pages;
}

void malloc_init_default_pool(void)
{
	memset(g_alloc_map, 0, sizeof(g_alloc_map));  // Clear bitmap
}

extern "C"
{

/// @brief The custom implementation of the bitmap allocator described at the top of this module.
void* malloc(size_t size)
{
	uint32_t* const alloc_start = g_alloc_map_ptr;
	const uint32_t alloc_bit_start = g_alloc_map_bit;
	uint32_t *current_start;
	uint32_t current_bit_start;
	uint8_t *pool_ptr;
	const uint16_t num_pages = get_num_pages_for_size(size);
	bool looped = false;
	
	while(true)
	{
		current_start = g_alloc_map_ptr;
		current_bit_start = g_alloc_map_bit;
		if(check_block_free(&g_alloc_map_ptr, &g_alloc_map_bit, num_pages, &looped))
		{
			mark_block_allocated(current_start, current_bit_start, num_pages);
			pool_ptr = convert_alloc_map_position_into_mem_pool_ptr(current_start, current_bit_start);
			// store the number of pages as a header in the first page
			*((uint16_t*)pool_ptr) = num_pages;
			// return a pointer beyond the first page
			return pool_ptr + mem_page_size;
		}
		else
		{
			if(looped && g_alloc_map_ptr == alloc_start && g_alloc_map_bit <= alloc_bit_start)
			{
				// we looped back to our start position. Ran out of memory I suppose
				return NULL;
			}
			// if we're at the start of an alloc_map 32 bit entry then avoid walking through each bit one-by-one
			// if all bits are set
			skip_wholy_allocated_map_entries(alloc_start);
		}
	}
}

/// @brief The custom implementation of free() of the bitmap allocator described at the top of this module.
void free(void *ptr)
{
	uint8_t* const pool_ptr = ((uint8_t*)ptr) - mem_page_size;
	uint32_t *alloc_map_entry;
	uint32_t alloc_map_bit;
	const uint16_t num_pages = *((uint16_t*)pool_ptr);

	convert_mem_pool_ptr_into_alloc_map_position(pool_ptr, &alloc_map_entry, &alloc_map_bit);
	mark_block_free(alloc_map_entry, alloc_map_bit, num_pages);
}

/// @brief The custom implementation of realloc() of the bitmap allocator described at the top of this module.
void* realloc(void *ptr, size_t new_size)
{
	if(!ptr)
	{
		return malloc(new_size);
	}
	
	uint8_t* const pool_ptr = ((uint8_t*)ptr) - mem_page_size;
	uint32_t *alloc_map_entry;
	uint32_t alloc_map_bit;
	const uint16_t original_num_pages = *((uint16_t*)pool_ptr);
	const uint16_t new_num_pages = get_num_pages_for_size(new_size);
	
	convert_mem_pool_ptr_into_alloc_map_position(pool_ptr, &alloc_map_entry, &alloc_map_bit);
	
	// first mark the associated allocmap bits as free in order make sure the current block is correctly detected as free
	mark_block_free(alloc_map_entry, alloc_map_bit, original_num_pages);
	
	if(check_block_free(&alloc_map_entry, &alloc_map_bit, new_num_pages, NULL))
	{
		// reset the entry and bit because check_block_free advanced it.
		convert_mem_pool_ptr_into_alloc_map_position(pool_ptr, &alloc_map_entry, &alloc_map_bit);
		
		// allright, we can simply extend/contract the allocation because there's enough consecutive trailing space free
		mark_block_allocated(alloc_map_entry, alloc_map_bit, new_num_pages);
		
		*((uint16_t*)pool_ptr) = new_num_pages;
		// the original pointer is still valid.
		return ptr;
	}
	else
	{
		// not enough space to extend the allocation. allocate a new block and copy the content
		void *new_block = malloc(new_size);
		uint16_t copy_pages = (original_num_pages < new_num_pages) ? original_num_pages : new_num_pages;
		if(!new_block)
		{
			// even though we can't return the requested block, the old pointer should remain valid.
			// but we already marked it as free. Let's correct that.
			
			// reset the entry and bit because check_block_free advanced it.
			convert_mem_pool_ptr_into_alloc_map_position(pool_ptr, &alloc_map_entry, &alloc_map_bit);
			mark_block_allocated(alloc_map_entry, alloc_map_bit, original_num_pages);
			return NULL;
		}
		memcpy(new_block, ptr, (copy_pages - 1) * mem_page_size);
		// Okay, the original block's data was copied over to a newly allocated block
		// and the original block was marked as free.
		return new_block;
	}
}

/// @brief Custom implementation of calloc based on our custom malloc()
void* calloc(size_t num, size_t size)
{
	size_t actual_size = num * size;
	void *result = malloc(actual_size);
	if(result)
	{
		memset(result, 0, actual_size);
	}
	return result;
}

/// dummy implementation to ensure the linker doesn't pull newlibs' malloc code (which also pulls in a 1 KB __malloc_av buffer)
/// WARNING: Please consider the warning at the top of this module: We should avoid certain functionality in newlib as it may
/// try to call this function to allocate memory. We don't want to establish a permanent heap in this project.
void* _malloc_r(struct _reent *r, size_t size)
{
	(void)r;
    return malloc(size);
}

/// dummy implementation to ensure the linker doesn't pull newlibs' malloc code (which also pulls in a 1 KB __malloc_av buffer)
/// WARNING: Please consider the warning at the top of this module: We should avoid certain functionality in newlib as it may
/// try to call this function to allocate memory. We don't want to establish a permanent heap in this project.
void _free_r(struct _reent *r, void* ptr) {
    (void)r;
    return free(ptr);
}

}

#else

void malloc_init_default_pool(void)
{	
}
#endif

