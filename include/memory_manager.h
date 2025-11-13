#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "rtos_config.h"

/* ============================================================================
 * MEMORY CONFIGURATION
 * ============================================================================ */
#define MEMORY_ALIGNMENT            4
#define MIN_BLOCK_SIZE              16
#define MEMORY_MAGIC_FREE           0xDEAD
#define MEMORY_MAGIC_USED           0xBEEF

/* ============================================================================
 * MEMORY BLOCK STRUCTURE
 * ============================================================================ */
typedef struct memory_block {
    uint16_t magic;
    uint16_t size;
    struct memory_block* next;
    struct memory_block* prev;
} memory_block_t;

/* ============================================================================
 * FUNCTION PROTOTYPES
 * ============================================================================ */

/**
 * @brief Initialize the memory manager
 */
rtos_result_t memory_init(void);

/**
 * @brief Allocate memory block
 */
void* memory_alloc(uint32_t size);

/**
 * @brief Free previously allocated memory block
 */
rtos_result_t memory_free(void* ptr);

#endif /* MEMORY_MANAGER_H */

