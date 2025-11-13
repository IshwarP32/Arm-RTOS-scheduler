#include "queue_manager.h"
#include <string.h>

/* ============================================================================
 * PRIVATE DATA STRUCTURES
 * ============================================================================ */
typedef struct {
    uint8_t  buffer[QUEUE_MAX_SIZE * sizeof(uint32_t)];
    uint32_t item_size;
    uint32_t head;
    uint32_t tail;
    uint32_t count;
    bool     initialized;
} queue_t;

static queue_t queues[QUEUE_MAX_COUNT];

/* ============================================================================
 * PUBLIC FUNCTIONS
 * ============================================================================ */

/**
 * @brief Initialize queue subsystem
 */
void queue_init(void)
{
    for (uint8_t i = 0; i < QUEUE_MAX_COUNT; i++) {
        queues[i].head = 0;
        queues[i].tail = 0;
        queues[i].count = 0;
        queues[i].item_size = 0;
        queues[i].initialized = false;
    }
}

/**
 * @brief Create a new message queue
 */
queue_result_t queue_create(uint8_t queue_id, uint32_t item_size)
{
    if (queue_id >= QUEUE_MAX_COUNT || item_size > sizeof(uint32_t)) {
        return QUEUE_ERROR;
    }
    
    queues[queue_id].item_size = item_size;
    queues[queue_id].head = 0;
    queues[queue_id].tail = 0;
    queues[queue_id].count = 0;
    queues[queue_id].initialized = true;
    
    return QUEUE_OK;
}

/**
 * @brief Send data to queue
 */
queue_result_t queue_send(uint8_t queue_id, const void* data)
{
    if (queue_id >= QUEUE_MAX_COUNT || !queues[queue_id].initialized || data == NULL) {
        return QUEUE_ERROR;
    }
    
    queue_t* q = &queues[queue_id];
    
    if (q->count >= QUEUE_MAX_SIZE) {
        return QUEUE_FULL;
    }
    
    /* Copy data into circular buffer */
    uint32_t offset = q->head * q->item_size;
    for (uint32_t i = 0; i < q->item_size; i++) {
        q->buffer[offset + i] = ((uint8_t*)data)[i];
    }
    
    q->head = (q->head + 1) % QUEUE_MAX_SIZE;
    q->count++;
    
    return QUEUE_OK;
}

/**
 * @brief Receive data from queue
 */
queue_result_t queue_receive(uint8_t queue_id, void* data)
{
    if (queue_id >= QUEUE_MAX_COUNT || !queues[queue_id].initialized || data == NULL) {
        return QUEUE_ERROR;
    }
    
    queue_t* q = &queues[queue_id];
    
    if (q->count == 0) {
        return QUEUE_EMPTY;
    }
    
    /* Copy data from circular buffer */
    uint32_t offset = q->tail * q->item_size;
    for (uint32_t i = 0; i < q->item_size; i++) {
        ((uint8_t*)data)[i] = q->buffer[offset + i];
    }
    
    q->tail = (q->tail + 1) % QUEUE_MAX_SIZE;
    q->count--;
    
    return QUEUE_OK;
}

/**
 * @brief Check if queue is empty
 */
bool queue_is_empty(uint8_t queue_id)
{
    if (queue_id >= QUEUE_MAX_COUNT) {
        return true;
    }
    return queues[queue_id].count == 0;
}

/**
 * @brief Check if queue is full
 */
bool queue_is_full(uint8_t queue_id)
{
    if (queue_id >= QUEUE_MAX_COUNT) {
        return false;
    }
    return queues[queue_id].count >= QUEUE_MAX_SIZE;
}
