#include "scheduler.h"
#include "timer_manager.h"

/* ============================================================================
 * GLOBAL VARIABLES
 * ============================================================================ */
static tcb_t* taskList = NULL;
static bool schedulerRunning = false;
static uint8_t idleTaskId = 0xFF;

/* External demo variables from main.c */
extern volatile int scheduler_iterations;
extern volatile int current_task_id;

/* ============================================================================
 * PRIVATE FUNCTION PROTOTYPES
 * ============================================================================ */
static void moveToNextTask(void);
static tcb_t* scheduler_get_next_task(void);

/* ============================================================================
 * PUBLIC FUNCTIONS
 * ============================================================================ */

/**
 * @brief Initialize the scheduler
 */
rtos_result_t scheduler_init(void)
{
    taskList = NULL;
    schedulerRunning = false;
    
    /* Create idle task */
    idleTaskId = task_create(scheduler_idle_task, "IDLE", MIN_STACK_SIZE);
    
    return RTOS_SUCCESS;
}

/**
 * @brief Start the scheduler
 */
void scheduler_start(void)
{
    schedulerRunning = true;
    
    tcb_t* firstTask = scheduler_get_next_task();
    
    if(firstTask != NULL)
    {
        task_set_state(firstTask->task_id, TASK_STATE_RUNNING);
    }
}

/**
 * @brief Add task to ready list
 */
rtos_result_t scheduler_add_ready_task(tcb_t* tcb)
{
    if(tcb == NULL)
    {
        return RTOS_INVALID_PARAM;
    }
    
    if(taskList == NULL)
    {
        /* First task in list */
        taskList = tcb;
        tcb->next = tcb;
        tcb->prev = tcb;
    }
    else
    {
        /* Add to end of circular list */
        tcb_t* tail = taskList->prev;
        tcb->next = taskList;
        tcb->prev = tail;
        tail->next = tcb;
        taskList->prev = tcb;
    }
    
    return RTOS_SUCCESS;
}

/**
 * @brief Check if scheduler is running
 */
bool scheduler_is_running(void)
{
    return schedulerRunning;
}

/**
 * @brief Idle task function
 */
void scheduler_idle_task(void)
{
    /* Idle processing - could include power management */
}

/**
 * @brief Register a task function with the scheduler
 */
uint8_t scheduler_add_task_fn(scheduler_task_fn_t fn, const char* name, uint32_t stack_size)
{
    if (fn == NULL) {
        return 0xFF;
    }
    if (stack_size == 0) {
        stack_size = DEFAULT_STACK_SIZE;
    }
    return task_create(fn, (name ? name : "Task"), stack_size);
}

/**
 * @brief Run cooperative round-robin with fixed time slice per task
 */
void scheduler_run(void)
{
    scheduler_start();

    /* Calculate SysTick slice using timer_manager */
    uint32_t slice_ticks = timer_calculate_slice_ticks(TIME_SLICE_MS);

    int rr_index = 0;

    for(;;) {
        scheduler_iterations++;

        tcb_t* nextTask = scheduler_get_next_task();
        if (nextTask == NULL) {
            scheduler_idle_task();
            continue;
        }

        /* Skip blocked/suspended tasks */
        if (nextTask->state == TASK_STATE_BLOCKED || nextTask->state == TASK_STATE_SUSPENDED) {
            moveToNextTask();
            continue;
        }

        /* Start time slice using timer_manager */
        timer_start_slice(slice_ticks);

        task_set_state(nextTask->task_id, TASK_STATE_RUNNING);

        current_task_id = rr_index;

        /* Run task function repeatedly until the slice expires */
        while (!timer_slice_expired()) {
            if (nextTask->task_function) {
                nextTask->task_function();
            } else {
                break;
            }
        }

        /* Stop timer after slice */
        timer_stop_slice();

        /* Set back to READY unless it blocked itself */
        if (nextTask->state == TASK_STATE_RUNNING) {
            nextTask->state = TASK_STATE_READY;
        }

        /* Advance to next task */
        moveToNextTask();
        rr_index = (rr_index + 1) % (task_get_count() ? task_get_count() : 1);

        /* Reset iteration counter periodically */
        if (scheduler_iterations >= 1000) {
            scheduler_iterations = 0;
        }
    }
}

/* ============================================================================
 * PRIVATE FUNCTIONS
 * ============================================================================ */

/**
 * @brief Get next task to run (round-robin)
 */
static tcb_t* scheduler_get_next_task(void)
{
    if(taskList == NULL)
    {
        return task_get_tcb(idleTaskId);
    }
    
    return taskList;
}

/**
 * @brief Move to next task in round-robin
 */
static void moveToNextTask(void)
{
    if(taskList == NULL)
    {
        return;
    }
    
    taskList = taskList->next;
}
