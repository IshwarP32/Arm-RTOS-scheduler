/**
 * @file rtos_config.h
 * @brief ARM RTOS Configuration Header
 * @author All Team Members
 * @date 2024
 * 
 * This file contains configuration parameters for the ARM RTOS scheduler.
 */

#ifndef RTOS_CONFIG_H
#define RTOS_CONFIG_H

/* Standard includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ============================================================================
 * RTOS CONFIGURATION PARAMETERS
 * ============================================================================ */

/* Maximum number of tasks */
#define MAX_TASKS                   8

/* Maximum task name length */
#define MAX_TASK_NAME_LENGTH        16

/* Task stack sizes */
#define MIN_STACK_SIZE              128
#define DEFAULT_STACK_SIZE          256

/* Time slice for round-robin scheduling (in ms) */
#define TIME_SLICE_MS               10

/* System clock frequency (Hz) */
#define SYSTEM_CLOCK_HZ             48000000

/* Memory configuration */
#define HEAP_SIZE                   4096

/* ============================================================================
 * TASK STATES
 * ============================================================================ */
typedef enum {
    TASK_STATE_READY,
    TASK_STATE_RUNNING,
    TASK_STATE_BLOCKED,
    TASK_STATE_SUSPENDED,
    TASK_STATE_DELETED
} task_state_t;

/* ============================================================================
 * RETURN CODES
 * ============================================================================ */
typedef enum {
    RTOS_SUCCESS = 0,
    RTOS_ERROR = -1,
    RTOS_TIMEOUT = -2,
    RTOS_NO_MEMORY = -3,
    RTOS_INVALID_PARAM = -4
} rtos_result_t;

/* ============================================================================
 * CRITICAL SECTION MACROS
 * ============================================================================ */
#define ENTER_CRITICAL()            __disable_irq()
#define EXIT_CRITICAL()             __enable_irq()

/* ============================================================================
 * DEBUG MACROS
 * ============================================================================ */
#define DEBUG_PRINT(...)            do { } while(0)

#endif /* RTOS_CONFIG_H */
