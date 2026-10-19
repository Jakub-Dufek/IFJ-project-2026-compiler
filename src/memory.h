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

#define ARENA_SIZE 1024 * 1024 // 1 MB

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


//  AST ARENA FUNCTIONS

/**
 * @brief Initializes arena for AST
 */
void ast_init_arena();

/**
 * @brief Frees all allocated nodes in AST at once
 */
void ast_free_arena(void);

/**
 * @brief Allocates size number of bytes in ast_arena
 * 
 * @param size size in bytes
 * 
 * @return ptr to allocated mem
 */
void *ast_alloc(unsigned size);


//  STRING ARENA FUNCTIONS

// TODO: string arena functions

#endif
