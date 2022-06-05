#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "cache.h"
#include "print_helpers.h"

cache_t *make_cache(int capacity, int block_size, int assoc, enum protocol_t protocol, bool lru_on_invalidate_f){
  cache_t *cache = malloc(sizeof(cache_t));
  cache->stats = make_cache_stats();
  
  cache->capacity = capacity;      // in Bytes
  cache->block_size = block_size;  // in Bytes
  cache->assoc = assoc;            // 1, 2, 3... etc.

  // FIX THIS CODE!
  // first, correctly set these 5 variables. THEY ARE ALL WRONG
  // note: you may find math.h's log2 function useful
  cache->n_cache_line = capacity / block_size;
  cache->n_set = capacity / (assoc * block_size);
  cache->n_offset_bit = log2(block_size);
  cache->n_index_bit = log2(cache->n_set);
  cache->n_tag_bit = 32 - (cache->n_index_bit) - (cache->n_offset_bit);

  // next create the cache lines and the array of LRU bits
  // - malloc an array with n_rows
  // - for each element in the array, malloc another array with n_col
  // FIX THIS CODE!

  cache->lines = malloc((cache->n_set)*sizeof(cache_line_t*));
  for (int i = 0; i < (cache->n_set); i++){
    (cache->lines)[i] = malloc(assoc * sizeof(cache_line_t));
  }
  cache->lru_way = malloc((cache->n_set)*sizeof(int));

  // initializes cache tags to 0, dirty bits to false,
  // state to INVALID, and LRU bits to 0
  // FIX THIS CODE!
  for (int i = 0; i < (cache->n_set); i++) {
    for (int j = 0; j < assoc; j++) {
      cache->lines[i][j].tag = 0;
      cache->lines[i][j].dirty_f = false;
      cache->lines[i][j].state = INVALID;
    }
    cache->lru_way[i]=0;
  }

  cache->protocol = protocol;
  cache->lru_on_invalidate_f = lru_on_invalidate_f;
  
  return cache;
}

/* Given a configured cache, returns the tag portion of the given address.
 *
 * Example: a cache with 4 bits each in tag, index, offset
 * in binary -- get_cache_tag(0b111101010001) returns 0b1111
 * in decimal -- get_cache_tag(3921) returns 15 
 */
unsigned long get_cache_tag(cache_t *cache, unsigned long addr) {
  // FIX THIS CODE!
  unsigned tag = addr >> (32 - (cache->n_tag_bit)); //shift left by (32 - num of tag bits)

  return tag;
}

/* Given a configured cache, returns the index portion of the given address.
 *
 * Example: a cache with 4 bits each in tag, index, offset
 * in binary -- get_cache_index(0b111101010001) returns 0b0101
 * in decimal -- get_cache_index(3921) returns 5
 */
unsigned long get_cache_index(cache_t *cache, unsigned long addr) {
  // FIX THIS CODE!
  unsigned long index = ~(~0 << cache->n_index_bit) & (addr >> cache->n_offset_bit); // create bit mask for the index bits
  return index;
}

/* Given a configured cache, returns the given address with the offset bits zeroed out.
 *
 * Example: a cache with 4 bits each in tag, index, offset
 * in binary -- get_cache_block_addr(0b111101010001) returns 0b111101010000
 * in decimal -- get_cache_block_addr(3921) returns 3920
 */
unsigned long get_cache_block_addr(cache_t *cache, unsigned long addr) {
  // FIX THIS CODE!

  unsigned long block_addr = (addr >> (cache->n_offset_bit)) << (cache->n_offset_bit); //shift left then right
  return block_addr;
}

// update lru assume using 2_way set associative cache
void update_lru_way(cache_t *cache, int index, int way_accessed)
{
  if (cache->assoc == 1) {
    return;
  }

  else {
    cache->lru_way[index] = abs(way_accessed - 1);
  }

  
}

// helper function to update cache
void update_func(cache_t *cache, unsigned long index, unsigned long tag_received, int way_accessed, enum state_t current_state, bool hit_f, bool writeback_f, bool upgrade_miss_f, enum action_t action)
{

  if (action == STORE)
  {
    cache->lines[index][way_accessed].dirty_f = 1; // update dirty bit in store command
  }
  
  if (!hit_f){
    cache->lines[index][way_accessed].tag = tag_received; //update tag if miss

    if (action == LOAD) {
      cache->lines[index][way_accessed].dirty_f = 0; //reset dirty bit in load command when miss
    }
  }

  update_lru_way(cache, index, way_accessed);

  cache->lines[index][way_accessed].state = current_state;

  update_stats(cache->stats, hit_f, writeback_f, upgrade_miss_f, action);

  return;
}

/* this method takes a cache, an address, and an action
 * it proceses the cache access. functionality in no particular order: 
 *   - look up the address in the cache, determine if hit or miss
 *   - update the LRU_way, cacheTags, state, dirty flags if necessary
 *   - update the cache statistics (call update_stats)
 * return true if there was a hit, false if there was a miss
 * Use the "get" helper functions above. They make your life easier.
 */

bool access_cache(cache_t *cache, unsigned long addr, enum action_t action) {
  // FIX THIS CODE!

  unsigned long index = get_cache_index(cache, addr); //get the index

  unsigned long tag = get_cache_tag(cache, addr); //get the tag

  

  for (int i = 0; i < cache->assoc; i++){
    //check cache hit
    if (tag == cache->lines[index][i].tag){

      bool hit_f = true;
      bool writeback_f = false;

      update_func(cache, index, tag, i, VALID, hit_f, writeback_f, false, action);

      return hit_f;
    }
  }

  //cache miss:
  int curr_lru_way = cache->lru_way[index];

  bool writeback_f = cache->lines[index][cache->lru_way[index]].dirty_f == 1; // get writeback bool

  bool hit_f = false;

  update_func(cache, index, tag, curr_lru_way, VALID, hit_f, writeback_f, false, action);

  return hit_f;
}



