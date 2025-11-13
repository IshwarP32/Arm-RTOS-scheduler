#include "memory_manager.h"
#include "rtos_config.h"
#include "arm_cortex_m.h"

/* ============================================================================
 * GLOBAL VARIABLES
 * ============================================================================ */
static uint8_t heap_memory[HEAP_SIZE];
static memory_block_t* free_block_list = NULL;
static bool is_memory_initialized = false;

/* ============================================================================
 * PRIVATE FUNCTION PROTOTYPES
 * ============================================================================ */
static memory_block_t* memory_find_free_block(uint32_t size);
static void memory_split_block(memory_block_t* block, uint32_t size);
static void memory_coalesce_blocks(void);
static void memory_insert_free_block(memory_block_t* block);
static void memory_remove_free_block(memory_block_t* block);
static uint32_t memory_align_size(uint32_t size);

/* ============================================================================
 * PUBLIC FUNCTIONS
 * ============================================================================ */

/**
 * @brief Initialize the memory manager
 */
rtos_result_t memory_init(void)
{
    if(is_memory_initialized)
    {
        return RTOS_SUCCESS;
    }
    
    memset(heap_memory, 0, HEAP_SIZE);
    
    /* Initialize first free block covering entire heap */
    free_block_list = (memory_block_t*)heap_memory;
    free_block_list->magic = MEMORY_MAGIC_FREE;
    free_block_list->size = HEAP_SIZE;
    free_block_list->next = NULL;
    free_block_list->prev = NULL;
    
    is_memory_initialized = true;
    
    return RTOS_SUCCESS;
}

/**
 * @brief Allocate memory block
 */
void* memory_alloc(uint32_t size)
{
    if(!is_memory_initialized || size == 0)
    {
        return NULL;
    }
    
    /* Align size and add header overhead */
    uint32_t aligned_size = memory_align_size(size + sizeof(memory_block_t));
    
    if(aligned_size < MIN_BLOCK_SIZE)
    {
        aligned_size = MIN_BLOCK_SIZE;
    }
    
    ENTER_CRITICAL();
    
    /* Find suitable free block */
    memory_block_t* block = memory_find_free_block(aligned_size);
    
    if(block == NULL)
    {
        EXIT_CRITICAL();
        return NULL;
    }
    
    /* Remove block from free list */
    memory_remove_free_block(block);
    
    /* Split block if too large */
    if(block->size >= aligned_size + MIN_BLOCK_SIZE)
    {
        memory_split_block(block, aligned_size);
    }
    
    /* Mark block as used */
    block->magic = MEMORY_MAGIC_USED;
    
    EXIT_CRITICAL();
    
    /* Return pointer to user data (skip header) */
    return (void*)((uint8_t*)block + sizeof(memory_block_t));
}

/**
 * @brief Free previously allocated memory block
 */
rtos_result_t memory_free(void* ptr)
{
    if(!is_memory_initialized || ptr == NULL)
    {
        return RTOS_INVALID_PARAM;
    }
    
    /* Get block header */
    memory_block_t* block = (memory_block_t*)((uint8_t*)ptr - sizeof(memory_block_t));
    
    /* Validate block */
    if(block->magic != MEMORY_MAGIC_USED)
    {
        return RTOS_ERROR;
    }
    
    /* Check bounds */
    if((uint8_t*)block < heap_memory || (uint8_t*)block >= heap_memory + HEAP_SIZE)
    {
        return RTOS_ERROR;
    }
    
    ENTER_CRITICAL();
    
    /* Mark block as free */
    block->magic = MEMORY_MAGIC_FREE;
    
    /* Add block to free list */
    memory_insert_free_block(block);
    
    /* Coalesce adjacent free blocks */
    memory_coalesce_blocks();
    
    EXIT_CRITICAL();
    
    return RTOS_SUCCESS;
}

/* ============================================================================
 * PRIVATE FUNCTIONS
 * ============================================================================ */

/**
 * @brief Align size to memory alignment boundary
 */
static uint32_t memory_align_size(uint32_t size)
{
    return (size + MEMORY_ALIGNMENT - 1) & ~(MEMORY_ALIGNMENT - 1);
}

/**
 * @brief Find free block with first-fit algorithm
 */
static memory_block_t* memory_find_free_block(uint32_t size)
{
    memory_block_t* current = free_block_list;
    
    while(current != NULL)
    {
        if(current->size >= size)
        {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

/**
 * @brief Split block into allocated and free parts
 */
static void memory_split_block(memory_block_t* block, uint32_t size)
{
    /* Create new free block from remainder */
    memory_block_t* new_block = (memory_block_t*)((uint8_t*)block + size);
    new_block->magic = MEMORY_MAGIC_FREE;
    new_block->size = block->size - size;
    new_block->next = NULL;
    new_block->prev = NULL;
    
    /* Update original block size */
    block->size = size;
    
    /* Add new block to free list */
    memory_insert_free_block(new_block);
}

/**
 * @brief Insert block into free list
 */
static void memory_insert_free_block(memory_block_t* block)
{
    if(free_block_list == NULL)
    {
        free_block_list = block;
        block->next = NULL;
        block->prev = NULL;
        return;
    }
    
    /* Insert at head for simplicity */
    block->next = free_block_list;
    block->prev = NULL;
    free_block_list->prev = block;
    free_block_list = block;
}

/**
 * @brief Remove block from free list
 */
static void memory_remove_free_block(memory_block_t* block)
{
    if(block->prev != NULL)
    {
        block->prev->next = block->next;
    }
    else
    {
        free_block_list = block->next;
    }
    
    if(block->next != NULL)
    {
        block->next->prev = block->prev;
    }
    
    block->next = NULL;
    block->prev = NULL;
}

/**
 * @brief Coalesce adjacent free blocks
 */
static void memory_coalesce_blocks(void)
{
    memory_block_t* current = free_block_list;
    
    while(current != NULL)
    {
        memory_block_t* next_addr = (memory_block_t*)((uint8_t*)current + current->size);
        
        /* Check if next physical block is also free */
        memory_block_t* scan = free_block_list;
        while(scan != NULL)
        {
            if(scan == next_addr && scan->magic == MEMORY_MAGIC_FREE)
            {
                /* Merge blocks */
                current->size += scan->size;
                memory_remove_free_block(scan);
                break;
            }
            scan = scan->next;
        }
        
        current = current->next;
    }
}

