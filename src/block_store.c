#include <stdio.h>
#include <stdint.h>

#include "bitmap.h"
#include "block_store.h"
// include more if you need


// You might find this handy. I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

/**
<<<<<<< HEAD
 *Represents a block storage device containing data blocks and a bitmap for tracking free and used blocks
 */
struct block_store {
	//2D array representing storage blocks
	uint8_t data[BLOCK_STORE_NUM_BLOCKS][BLOCK_SIZE_BYTES]; //512 blocks, 32 bytes each
	// Pointer to bitmap
	bitmap_t *bitmap;

=======
 * Represents a block storage device containing data blocks and a bitmap for tracking free and used blocks
 */
struct block_store {
	// 2D array representing storage blocks
	uint8_t data[BLOCK_STORE_NUM_BLOCKS][BLOCK_SIZE_BYTES]; // 512 blocks, 32 bytes each
	// Pointer to bitmap
	bitmap_t *bitmap;
>>>>>>> fb0b17149625498de98ac8259d1173e6c95b3e81
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
<<<<<<< HEAD
	if(block_store == NULL){
=======
	if (block_store == NULL) {
>>>>>>> fb0b17149625498de98ac8259d1173e6c95b3e81
		// failure
		return NULL;
	}

	// Create bitmap overlay at specified BITMAP_START_BLOCK
	block_store->bitmap = bitmap_overlay(BITMAP_SIZE_BITS, block_store->data[BITMAP_START_BLOCK]);
<<<<<<< HEAD
	if(block_store->bitmap == NULL){
=======
	if (block_store->bitmap == NULL) {
>>>>>>> fb0b17149625498de98ac8259d1173e6c95b3e81
		// failure
		free(block_store);
		return NULL;
	}

	// Iterate through the blocks designated for bitmap
<<<<<<< HEAD
	for(size_t i = BITMAP_START_BLOCK; i < BITMAP_START_BLOCK + BITMAP_NUM_BLOCKS; i++){
=======
	for (size_t i = BITMAP_START_BLOCK; i < BITMAP_START_BLOCK + BITMAP_NUM_BLOCKS; i++) {
		// Mark bitmap blocks as allocated
>>>>>>> fb0b17149625498de98ac8259d1173e6c95b3e81
		block_store_request(block_store, i);
	}

	// Return initialized block store pointer
	return block_store;
<<<<<<< HEAD
=======

>>>>>>> fb0b17149625498de98ac8259d1173e6c95b3e81
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
	if (bs == NULL) return SIZE_MAX;

	// Find index of the first free block in bitmap
	size_t free_block_id = bitmap_ffz(bs->bitmap);

	// Verify it found free block
	if (free_block_id == SIZE_MAX || free_block_id >= BLOCK_STORE_NUM_BLOCKS) return SIZE_MAX;

	// Request and mark free block as allocated
	bool success = block_store_request(bs, free_block_id);

	// If unsuccessful, return SIZE_MAX
	if (!success) return SIZE_MAX;

	// Return ID of allocated block
	return free_block_id;

}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
	UNUSED(bs);
	UNUSED(block_id);
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
