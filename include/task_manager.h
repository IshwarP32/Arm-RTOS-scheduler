/**
 * @file task_manager.h
 * @brief Task Management Interface
 * @author Team Member 1 - Task Management
 * @date 2024
 * 
 * This module handles task creation and state management.
 */

#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include "rtos_config.h"

/* ============================================================================
 * TASK CONTROL BLOCK (TCB) STRUCTURE
 * ============================================================================ */
typedef struct task_control_block {
    uint8_t task_id;
    char task_name[MAX_TASK_NAME_LENGTH];
    void (*task_function)(void);
    task_state_t state;
    uint32_t* stack_pointer;
    uint32_t* stack_base;
    uint32_t stack_size;
    struct task_control_block* next;
    struct task_control_block* prev;
} tcb_t;

/* ============================================================================
 * FUNCTION PROTOTYPES
 * ============================================================================ */

/**
 * @brief Initialize the task manager
 */
rtos_result_t task_manager_init(void);

/**
 * @brief Create a new task
 */
uint8_t task_create(void (*task_function)(void), 
                   const char* task_name, 
                   uint32_t stack_size);

/**
 * @brief Get task control block by ID
 */
tcb_t* task_get_tcb(uint8_t task_id);

/**
 * @brief Get current running task
 */
tcb_t* task_get_current(void);

/**
 * @brief Set task state
 */
rtos_result_t task_set_state(uint8_t task_id, task_state_t new_state);

/**
 * @brief Get number of active tasks
 */
uint8_t task_get_count(void);

#endif /* TASK_MANAGER_H */
