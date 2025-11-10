
  //This module implements task creation and state management for the scheduler.
 
#include "task_manager.h"
#include "memory_manager.h"
#include "scheduler.h"

 // GLOBAL VARIABLES

static tcb_t task_table[MAX_TASKS];
static uint8_t task_count = 0;
static uint8_t current_task_id = 0xFF;


 //PRIVATE FUNCTION PROTOTYPES

static uint8_t task_get_free_id(void);
 // PUBLIC FUNCTIONS

rtos_result_t task_manager_init(void)
{
    memset(task_table, 0, sizeof(task_table));
    
    for(int i = 0; i < MAX_TASKS; i++)
    {
        task_table[i].task_id = 0xFF;
        task_table[i].state = TASK_STATE_DELETED;
    }
    
    task_count = 0;
    current_task_id = 0xFF;
    
    return RTOS_SUCCESS;
}

 // Create a new task
uint8_t task_create(void (*task_function)(void), 
                   const char* task_name, 
                   uint32_t stack_size)
{
    if(task_function == NULL || task_name == NULL)
    {
        return 0xFF;
    }
    if(stack_size < MIN_STACK_SIZE || task_count >= MAX_TASKS)
    {
        return 0xFF;
    }
    uint8_t task_id = task_get_free_id();
    if(task_id == 0xFF)
    {
        return 0xFF;
    }
    tcb_t* tcb = &task_table[task_id];
    
    /* Allocate stack memory */
    uint32_t* stack = (uint32_t*)memory_alloc(stack_size);
    if(stack == NULL)
    {
        return 0xFF;
    }
 
    /* Initialize TCB */
    tcb->task_id = task_id;
    strncpy(tcb->task_name, task_name, MAX_TASK_NAME_LENGTH - 1);
    tcb->task_name[MAX_TASK_NAME_LENGTH - 1] = '\0';
    tcb->task_function = task_function;
    tcb->state = TASK_STATE_READY;
    tcb->stack_base = stack;
    tcb->stack_size = stack_size;
    tcb->stack_pointer = stack + (stack_size / sizeof(uint32_t)) - 1;
    
    task_count++;
    
    /* Add task to scheduler ready list */
    scheduler_add_ready_task(tcb);
    
    return task_id;
}

 // Get task control block by ID
 
tcb_t* task_get_tcb(uint8_t task_id)
{
    if(task_id >= MAX_TASKS)
    {
        return NULL;
    }
    
    if(task_table[task_id].state == TASK_STATE_DELETED)
    {
        return NULL;
    }
    
    return &task_table[task_id];
}

  //Get current running task
 
tcb_t* task_get_current(void)
{
    if(current_task_id == 0xFF)
    {
        return NULL;
    }
    
    return &task_table[current_task_id];
}

// Set task state
 
rtos_result_t task_set_state(uint8_t task_id, task_state_t new_state)
{
    if(task_id >= MAX_TASKS)
    {
        return RTOS_INVALID_PARAM;
    }
    
    tcb_t* tcb = &task_table[task_id];
    
    if(tcb->state == TASK_STATE_DELETED)
    {
        return RTOS_ERROR;
    }
    
    tcb->state = new_state;
    
    if(new_state == TASK_STATE_RUNNING)
    {
        current_task_id = task_id;
    }
    
    return RTOS_SUCCESS;
}
 //Get number of active tasks
 
uint8_t task_get_count(void)
{
    return task_count;
}

 // PRIVATE FUNCTIONS
// Get next available task ID
 
static uint8_t task_get_free_id(void)
{
    for(uint8_t i = 0; i < MAX_TASKS; i++)
    {
        if(task_table[i].state == TASK_STATE_DELETED)
        {
            return i;
        }
    }
    
    return 0xFF;
}
