#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "rtos_config.h"
#include "task_manager.h"


typedef void (*scheduler_task_fn_t)(void);

rtos_result_t scheduler_init(void);

rtos_result_t scheduler_add_ready_task(tcb_t* tcb);

bool scheduler_is_running(void);

void scheduler_idle_task(void);

uint8_t scheduler_add_task_fn(scheduler_task_fn_t fn, const char* name, uint32_t stack_size);

void scheduler_run(void);

#endif 
