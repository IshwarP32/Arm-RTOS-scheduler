/**
 * @file queue_manager.h
 * @brief Queue and Semaphore Management Interface
 * @author Team Member 3 - Queue/Semaphore Management
 * @date 2024
 * 
 * This module implements simple message queues for inter-task communication.
 */

#ifndef QUEUE_MANAGER_H
#define QUEUE_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

/* Queue configuration */
#define QUEUE_MAX_SIZE    8
#define QUEUE_MAX_COUNT   2

/* Queue result codes */
typedef enum {
    QUEUE_OK = 0,
    QUEUE_EMPTY,
    QUEUE_FULL,
    QUEUE_ERROR
} queue_result_t;

/**
 * @brief Initialize queue subsystem
 */
void queue_init(void);

/**
 * @brief Create a new message queue
 * @param queue_id Queue identifier (0 to QUEUE_MAX_COUNT-1)
 * @param item_size Size of each queue item in bytes
 * @return queue_result_t Success or error code
 */
queue_result_t queue_create(uint8_t queue_id, uint32_t item_size);

/**
 * @brief Send data to queue
 * @param queue_id Queue identifier
 * @param data Pointer to data to send
 * @return queue_result_t Success or error code
 */
queue_result_t queue_send(uint8_t queue_id, const void* data);

/**
 * @brief Receive data from queue
 * @param queue_id Queue identifier
 * @param data Pointer to buffer for received data
 * @return queue_result_t Success or error code
 */
queue_result_t queue_receive(uint8_t queue_id, void* data);

/**
 * @brief Check if queue is empty
 * @param queue_id Queue identifier
 * @return bool True if queue is empty
 */
bool queue_is_empty(uint8_t queue_id);

/**
 * @brief Check if queue is full
 * @param queue_id Queue identifier
 * @return bool True if queue is full
 */
bool queue_is_full(uint8_t queue_id);

#endif /* QUEUE_MANAGER_H */