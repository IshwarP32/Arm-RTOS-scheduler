#include "scheduler.h"
#include "timer_manager.h"


static tcb_t* taskList = NULL;
static bool schedulerRunning = false;
static uint8_t idleTaskId = 0xFF;


extern volatile int scheduler_iterations;
extern volatile int current_task_id;

static void moveToNextTask(void);
static tcb_t* scheduler_get_next_task(void);


rtos_result_t scheduler_init(void)
{
    taskList = NULL;
    schedulerRunning = false;
    
    /* Create idle task */
    idleTaskId = task_create(scheduler_idle_task, "IDLE", MIN_STACK_SIZE);
    
    return RTOS_SUCCESS;
}


void scheduler_start(void)
{
    schedulerRunning = true;
    
    tcb_t* firstTask = scheduler_get_next_task();
    
    if(firstTask != NULL)
    {
        task_set_state(firstTask->task_id, TASK_STATE_RUNNING);
    }
}


rtos_result_t scheduler_add_ready_task(tcb_t* tcb)
{
    if(tcb == NULL)
    {
        return RTOS_INVALID_PARAM;
    }
    
    if(taskList == NULL)
    {
        
        taskList = tcb;
        tcb->next = tcb;
        tcb->prev = tcb;
    }
    else
    {

        tcb_t* tail = taskList->prev;
        tcb->next = taskList;
        tcb->prev = tail;
        tail->next = tcb;
        taskList->prev = tcb;
    }
    
    return RTOS_SUCCESS;
}


bool scheduler_is_running(void)
{
    return schedulerRunning;
}


void scheduler_idle_task(void)
{
    /* Idle processing - could include power management */
}


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


void scheduler_run(void)
{
    scheduler_start();

    uint32_t slice_ticks = timer_calculate_slice_ticks(TIME_SLICE_MS);

    int rr_index = 0;

    for(;;) {
        scheduler_iterations++;

        tcb_t* nextTask = scheduler_get_next_task();
        if (nextTask == NULL) {
            scheduler_idle_task();
            continue;
        }

        if (nextTask->state == TASK_STATE_BLOCKED || nextTask->state == TASK_STATE_SUSPENDED) {
            moveToNextTask();
            continue;
        }

        
        timer_start_slice(slice_ticks);

        task_set_state(nextTask->task_id, TASK_STATE_RUNNING);

        current_task_id = rr_index;

        
        while (!timer_slice_expired()) {
            if (nextTask->task_function) {
                nextTask->task_function();
            } else {
                break;
            }
        }

        timer_stop_slice();

    
        if (nextTask->state == TASK_STATE_RUNNING) {
            nextTask->state = TASK_STATE_READY;
        }

        
        moveToNextTask();
        rr_index = (rr_index + 1) % (task_get_count() ? task_get_count() : 1);

    
        if (scheduler_iterations >= 1000) {
            scheduler_iterations = 0;
        }
    }
}


static tcb_t* scheduler_get_next_task(void)
{
    if(taskList == NULL)
    {
        return task_get_tcb(idleTaskId);
    }
    
    return taskList;
}

static void moveToNextTask(void)
{
    if(taskList == NULL)
    {
        return;
    }
    
    taskList = taskList->next;
}
