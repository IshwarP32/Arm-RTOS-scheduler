#include "rtos_config.h"
#include "task_manager.h"
#include "scheduler.h"
#include "memory_manager.h"

// for demo
volatile int task1_counter = 0;
volatile int task2_counter = 0;
volatile int task3_counter = 0;
volatile int scheduler_iterations = 0;
volatile int current_task_id = 0;


void task1(void){
    task1_counter++;
    for (volatile int i = 0; i < 20000; i++);
}

void task2(void){
    task2_counter++;
    for (volatile int i = 0; i < 20000; i++);
}

void task3(void){
    task3_counter++;
    for (volatile int i = 0; i < 20000; i++);
}

int main(void){
    // initialisation
    memory_init();
    task_manager_init();
    scheduler_init();

    // schedule tasks
    scheduler_add_task_fn(task1, "Task1", DEFAULT_STACK_SIZE);
    scheduler_add_task_fn(task2, "Task2", DEFAULT_STACK_SIZE);
    scheduler_add_task_fn(task3, "Task3", DEFAULT_STACK_SIZE);
    
    scheduler_run();
    
    return 0;
}
