#include <stdio.h>
#include <stdint.h>

#include "bitmap.h"
#include "block_store.h"
// include more if you need
#include <string.h>
#include "sys/stat.h"


// You might find this handy. I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

/**
 * Represents a block storage device containing data blocks and a bitmap for tracking free and used blocks
 */
struct block_store {
	// 2D array representing storage blocks
	uint8_t data[BLOCK_STORE_NUM_BLOCKS][BLOCK_SIZE_BYTES]; // 512 blocks, 32 bytes each
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
	if (block_store->bitmap == NULL) {
		// failure
		free(block_store);
		return NULL;
	}

	// Iterate through the blocks designated for bitmap
	for (size_t i = BITMAP_START_BLOCK; i < BITMAP_START_BLOCK + BITMAP_NUM_BLOCKS; i++) {
		// Mark bitmap blocks as allocated
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


/*
 * Frees the specified block by clearing its bit in the bitmap
 */
void block_store_release(block_store_t *const bs, const size_t block_id)
{
	//checks if all parameters are valid
	if (bs == NULL || bs->bitmap == NULL || block_id >= BLOCK_STORE_NUM_BLOCKS) {
		return;
	}

	//clears the bit to mark the block as free
	bitmap_reset(bs->bitmap, block_id);
}

/*
* counts all of the blocks that are currently listed as used
*/
size_t block_store_get_used_blocks(const block_store_t *const bs)
{
        //checks if input is valid
        if(bs == NULL || bs->bitmap == NULL){
                return SIZE_MAX;
        }

        //counter for used blocks
        size_t used_count = 0;

        //iterates through all bits in bitmap, testing them
        for(size_t i = 0; i < BLOCK_STORE_NUM_BLOCKS; i++){
                if(bitmap_test(bs->bitmap, i) == 1){
                        used_count++;
                }
        }

        return used_count;

}

/**
 * Counts all of the blocks that are currently listed as free
 * Returns SIZE_MAX if invalid pointer is found
 * Returns number of free blocks otherwise
 */
size_t block_store_get_free_blocks(const block_store_t *const bs)
{
	// input check
	if (bs == NULL || bs->bitmap == NULL) {
		return SIZE_MAX;
	}

	// current count of used blocks
	size_t used_count = block_store_get_used_blocks(bs);

	// check if used count failed
	if (used_count == SIZE_MAX) {
		return SIZE_MAX;
	}

	// free blocks is total blocks minus used blocks
	size_t free_count = BLOCK_STORE_NUM_BLOCKS - used_count;

	return free_count;
}

/*
 * Returns the total number of blocks in the block store
 */
size_t block_store_get_total_blocks()
{
	//total blocks is a constant, no object needed
	return BLOCK_STORE_NUM_BLOCKS;
}

/**
 * Reads data from the specified block and writes it to the designated buffer
 * Returns 0 if invalid pointer is found or block_id is out of range
 * Returns number of bytes read otherwise
 */
size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
	//checks if all parameters are valid
	if (bs == NULL || buffer == NULL || block_id >= BLOCK_STORE_NUM_BLOCKS) {
		return 0;
	}

	//copies data from the specified block into the buffer
	memcpy(buffer, bs->data[block_id], BLOCK_SIZE_BYTES);

	//return number of bytes read
	return BLOCK_SIZE_BYTES;
}

/*
 * Writes data from the designated buffer to the specified block
 * Returns 0 if invalid pointer is found or block_id is out of range
 * Returns number of bytes written otherwise
 */
size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
	// checks if all parameters are valid
	if (bs == NULL || buffer == NULL || block_id >= BLOCK_STORE_NUM_BLOCKS) {
		return 0;
	}

	//copies data from the buffer into the specified block
	memcpy(bs->data[block_id], buffer, BLOCK_SIZE_BYTES);

	//return number of bytes written
	return BLOCK_SIZE_BYTES;
}

block_store_t *block_store_deserialize(const char *const filename)
{
	UNUSED(filename);
	return NULL;
}

/*
 * Writes all of the BS device to file
 */
size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
        //checks parameters
        if(bs == NULL || bs->data == NULL || filename == NULL){
                return 0;
        }

        FILE *fptr = fopen(filename, "wb");

        //check for failure
        if(fptr == NULL){
                fclose(fptr);
                return 0;
        }

        //the total number of bytes that will be written
        size_t num_bytes_written = BLOCK_STORE_NUM_BYTES;

        //writes to file, then closes fptr
        fwrite(bs, BLOCK_SIZE_BYTES, BLOCK_STORE_NUM_BLOCKS, fptr);
        fclose(fptr);

        //creates a stat file to read the size of the file just written to
        //so that the actual filesize can be returned
        struct stat st;
        stat(filename, &st);

        //returns the total number of bytes written
        return st.st_size;


}
