#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "rtos_config.h"
#include "task_manager.h"

/* ============================================================================
 * TYPE DEFINITIONS
 * ============================================================================ */
typedef void (*scheduler_task_fn_t)(void);

/* ============================================================================
 * FUNCTION PROTOTYPES
 * ============================================================================ */

/**
 * @brief Initialize the scheduler and create an idle task
 */
rtos_result_t scheduler_init(void);

/**
 * @brief Add task to ready queue (used by task_manager)
 */
rtos_result_t scheduler_add_ready_task(tcb_t* tcb);

/**
 * @brief Check if scheduler is running
 */
bool scheduler_is_running(void);

/**
 * @brief Idle task
 */
void scheduler_idle_task(void);

/**
 * @brief Register a task function with the scheduler
 */
uint8_t scheduler_add_task_fn(scheduler_task_fn_t fn, const char* name, uint32_t stack_size);

/**
 * @brief Run cooperative round-robin with fixed time slice per task
 */
void scheduler_run(void);

#endif /* SCHEDULER_H */
