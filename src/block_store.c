#include <stdio.h>
#include <stdint.h>

#include "bitmap.h"
#include "block_store.h"
// include more if you need


// You might find this handy. I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

/**
 *Represents a block storage device containing data blocks and a bitmap for tracking free and used blocks
 */
struct block_store {
	//2D array representing storage blocks
	uint8_t data[BLOCK_STORE_NUM_BLOCKS][BLOCK_SIZE_BYTES]; //512 blocks, 32 bytes each
	// Pointer to bitmap
	bitmap_t *bitmap;

};

/**
 * Allocates a new block storage device, initialize it to zero, set up Free Block Map
 * using bitmap, and marks blocks used by blockmap as allocated
 * Returns pointer to newly created block store, or NULL if error occurs
 */
block_store_t *block_store_create()
{
	// Allocate memory for block store struct
	block_store_t *block_store = calloc(1, sizeof(block_store_t));
	if (block_store == NULL) {
		// failure
		return NULL;
	}

	// Create bitmap overlay at specified BITMAP_START_BLOCK
	block_store->bitmap = bitmap_overlay(BITMAP_SIZE_BITS, block_store->data[BITMAP_START_BLOCK]);
	if(block_store->bitmap == NULL){
		// failure
		free(block_store);
		return NULL;
	}

	// Iterate through the blocks designated for bitmap
	for(size_t i = BITMAP_START_BLOCK; i < BITMAP_START_BLOCK + BITMAP_NUM_BLOCKS; i++){
		block_store_request(block_store, i);
	}

	// Return initialized block store pointer
	return block_store;
}

/*
 * Frees all memory allocated to the given block store (bs)
 */
void block_store_destroy(block_store_t *const bs)
{
	// checks if bs is null, if null, does nothing, otherwise frees memory
	if(bs){
		free(bs->bitmap);
		free(bs);
	}
}

/**
 * Searches for first available free block in storage
 * If free block is found, it marks the block as in use within bitmap and returns block's ID
 * Returns SIZE_MAX if block store is full, no free blocks are found, or if invalid pointer is found
 */
size_t block_store_allocate(block_store_t *const bs)
{
        // Verify that the input pointer is not NULL
        if(bs == NULL) return SIZE_MAX;

        // Find index of the first free block in bitmap
        size_t free_block_id = bitmap_ffz(bs->bitmap);

        // Verify it found free block
        if (free_block_id == SIZE_MAX || free_block_id >= BLOCK_STORE_NUM_BLOCKS) return SIZE_MAX;

        // Request and mark free block as allocated
        bool success = block_store_request(bs, free_block_id);

        // If unsuccessful, return SIZE_MAX
        if(!success) return SIZE_MAX;

        // Return ID of allocated block
        return free_block_id;
}


/**
 * Checks to see if a given bit is available
 * if not, then returns false, if so, flips the bit and returns true
 */
bool block_store_request(block_store_t *const bs, const size_t block_id)
{
        //checks to see if all parameters are valid
        if(bs == NULL || bs->bitmap == NULL || block_id >= BLOCK_STORE_NUM_BLOCKS){
                return false;
        }

        //gets the bit that represents the block
        size_t availability = bitmap_test(bs->bitmap, block_id);

        //checks to see if block is already taken (0 if no, 1 if yes)
        if(availability == 0){
                bitmap_flip(bs->bitmap, block_id);
                return true;
        }

        return false;
}


void block_store_release(block_store_t *const bs, const size_t block_id)
{
	UNUSED(bs);
	UNUSED(block_id);
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
	UNUSED(bs);
	return 0;
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
	UNUSED(bs);
	return 0;
}

size_t block_store_get_total_blocks()
{
	return 0;
}

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
	UNUSED(bs);
	UNUSED(block_id);
	UNUSED(buffer);
	return 0;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
	UNUSED(bs);
	UNUSED(block_id);
	UNUSED(buffer);
	return 0;
}

block_store_t *block_store_deserialize(const char *const filename)
{
	UNUSED(filename);
	return NULL;
}

size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
	UNUSED(bs);
	UNUSED(filename);
	return 0;
}
