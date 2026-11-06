/**
 * @file memory.h
 * @internal Part of IFJ-project-2026-compiler, Team 274855
 *
 * @brief Header file for dynamic memory management
 * @author Jakub Dufek (274855)
 * @date 17. 10. 2026
 */

#ifndef MEMORY_H
#define MEMORY_H

#define ARENA_SIZE (1024 * 1024) // 1 MB
#define STARTING_STAGING_CAPACITY 256

typedef struct arena arena_t;


/**
 * @struct arena_t
 * @brief struct representing 1 block of memory (1 MB)
 */
struct arena {
    char arena[ARENA_SIZE];
    unsigned offset;
    arena_t *next;
};



//  GENERAL ARENA FUNCTIONS

/**
 * @brief Allocates size number of bytes in arena
 * 
 * @param size size in bytes
 * 
 * @return ptr to allocated mem
 */
void *arena_alloc(unsigned size);

/**
 * @brief Initializes arena
 */
void arena_init();

/**
 * @brief Frees all memory in arena
 */
void arena_free_all();



//  SCANNER STRING BUILDER (SCRATCHPAD)

/**
 * @brief Appends char to string buffer
 */
void scratch_append(char character);

/**
 * @brief Clears string buffer
 */
void scratch_clear();

/**
 * @brief Outputs UNALLOCATED string at buffer
 * 
 * @return UNALLOCATED string sitting in buffer
 */
const char *scratch_get_string();

/**
 * @brief Allocates string at buffer
 * 
 * @return ALLOCATED string sitting in buffer
 */
char *scratch_save_to_arena();

/**
 * @brief Initializes string buffer
 */
void scratch_init();

/**
 * @brief Frees string buffer
 */
void scratch_free();

#endif
