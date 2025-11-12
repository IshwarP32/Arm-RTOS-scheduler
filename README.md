# ARM RTOS Scheduler Project

## 📋 Project Overview

This project implements a **simple round-robin scheduler** with **time-sliced task execution** for ARM Cortex-M3. It demonstrates core RTOS concepts in a clear, educational manner suitable for learning and academic presentations.

### 🎯 Project Objectives

- Demonstrate round-robin scheduling with time slicing
- Show fair CPU time distribution among tasks
- Provide clear code organization
- Serve as educational platform for embedded systems

## 👥 Team

- [**Mayank Gangwar**](https://github.com/MayankGangwar1234)
- [**Ishwar Patil**](https://github.com/IshwarP32)
- [**Purab Agrawal**](https://github.com/purabagr2406)
- [**Priyansh Sharma**](https://github.com/priyansh2905)
- [**Shudhanshu Bhadana**](https://github.com/SHUDHANSHU-BHADANA)
- [**Aalok Moliya**](https://github.com/AalokMoliya)

## 🏗️ Project Structure

```
ARM_RTOS_Scheduler/
├── include/                    # Header files
│   ├── arm_cortex_m.h         # ARM Cortex-M3 hardware definitions
│   ├── memory_manager.h       # Memory allocation interface
│   ├── queue_manager.h        # Message queue interface
│   ├── rtos_config.h          # RTOS configuration settings
│   ├── scheduler.h            # Scheduler interface
│   ├── task_manager.h         # Task management interface
│   └── timer_manager.h        # Timer control interface
│
├── src/                       # Source files
│   ├── arm_cortex_m.c         # ARM hardware initialization
│   ├── main.c                 # Application entry point
│   ├── memory_manager.c       # Memory pool implementation
│   ├── queue_manager.c        # Circular queue implementation
│   ├── scheduler.c            # Round-robin scheduler
│   ├── task_manager.c         # Task control & state management
│   └── timer_manager.c        # SysTick timer control
│
├── Objects/                   # Build output (compiled objects)
├── Listings/                  # Assembly listings
├── startup_ARMCM3.s          # Startup code for Cortex-M3
├── system_ARMCM3.c           # System initialization
├── ARM_RTOS_Scheduler.uvprojx # Keil project file
└── README.md                 # This file
```

## 🚀 Getting Started

### Prerequisites

1. **Keil uVision IDE** (free version)
   - Download from: https://www.keil.com/download/product/
   - ARM Cortex-M3 support included

2. **Hardware Requirements**
   - None required - runs in simulator

### Setup Instructions

1. **Clone the Repository**
   ```bash
   git clone https://github.com/IshwarP32/Arm-RTOS-scheduler.git
   cd Arm-RTOS-scheduler
   ```

2. **Open in Keil uVision**
   - Launch Keil uVision IDE
   - Open project: `ARM_RTOS_Scheduler.uvprojx`
   - Select target: `ARM_RTOS_Simulator`

3. **Build the Project**
   - Press `F7` or use `Project → Build Target`
   - Ensure no compilation errors

4. **Run Simulation**
   - Press `Ctrl+F5` or use `Debug → Start/Stop Debug Session`
   - Add watch variables to observe execution

## 🔧 Configuration

### RTOS Configuration (rtos_config.h)

```c
#define MAX_TASKS               8       // Maximum number of tasks
#define TIME_SLICE_MS          10       // Time slice for round-robin (10ms)
#define HEAP_SIZE            4096       // Dynamic memory heap size
#define SYSTEM_CLOCK_HZ  48000000       // System clock frequency
#define DEFAULT_STACK_SIZE     256       // Default task stack size
```

## 📚 Core Components

### 1. Task Manager
**Responsibility:** Task creation and state management

**Key Functions:**
- `task_manager_init()` - Initialize task subsystem
- `task_create()` - Create task with stack allocation
- `task_set_state()` - Change task state

---

### 2. Scheduler (Member 2)
**Responsibility:** Round-robin scheduling with time slicing

**Key Functions:**
- `scheduler_init()` - Initialize scheduler
- `scheduler_run()` - Main scheduling loop
- `scheduler_add_task_fn()` - Register tasks

---

### 3. Queue Manager (Member 3)
**Responsibility:** Inter-task communication (available for expansion)

**Key Functions:**
- `queue_create()` - Create message queue
- `queue_send()` - Send message
- `queue_receive()` - Receive message

---

### 4. Memory Manager (Member 4)
**Responsibility:** Dynamic memory allocation

**Key Functions:**
- `memory_init()` - Initialize 4KB heap
- `memory_alloc()` - Allocate memory (first-fit)
- `memory_free()` - Free memory (with coalescing)

---

### 5. Timer Manager (Member 5)
**Responsibility:** Time-sliced scheduling control

**Key Functions:**
- `timer_calculate_slice_ticks()` - Calculate SysTick reload value
- `timer_start_slice()` - Start 10ms timer
- `timer_slice_expired()` - Check if slice completed

---

### 6. System Integration (Member 6)
**Responsibility:** System initialization and demo tasks

**Demo Tasks:**
- `task1_data_processing()` - Data processing simulation
- `task2_communication()` - Communication simulation
- `task3_monitoring()` - Monitoring simulation

---

## 💡 Demonstration

The main demo runs three tasks that increment counters:

```c
void task1_data_processing(void)
{
    task1_counter++;
    /* Simulate work */
    for (volatile int i = 0; i < 20000; i++);
}
```

### Watch Variables (in Keil Debugger)

Add these to watch window:
- `task1_counter` - Task 1 progress
- `task2_counter` - Task 2 progress  
- `task3_counter` - Task 3 progress
- `scheduler_iterations` - Total scheduler cycles
- `current_task_id` - Currently running task (0, 1, 2)

### Expected Behavior

✅ All three task counters increment continuously  
✅ All tasks progress at similar rates (fair scheduling)  
✅ Scheduler rotates every 10ms  
✅ No task starvation  

## 🔍 How It Works

### Scheduling Algorithm

1. **Initialize** all subsystems (memory, tasks, scheduler)
2. **Register** three demo tasks
3. **Enter** scheduler main loop:
   - Select next task (round-robin)
   - Start 10ms timer (SysTick)
   - Execute task function repeatedly until timer expires
   - Stop timer
   - Rotate to next task
   - Repeat forever

### Time Slicing

- Each task gets exactly 10ms of CPU time
- Timer uses SysTick hardware counter
- Polling mode checks COUNTFLAG bit
- Fair distribution ensures no starvation

## 🎯 Key Features

✅ Simple round-robin scheduling  
✅ Time-sliced execution (10ms)  
✅ Fair CPU distribution  
✅ Dynamic memory allocation  
✅ Clean code organization  
✅ Minimal dependencies  

## ⚠️ Limitations

This is an educational project, not a production RTOS:

- No task priorities (all tasks equal)
- No preemption (cooperative)
- No mutexes/semaphores (in demo)
- No interrupt-driven context switch
- Fixed time slice for all tasks
- Simple first-fit memory allocation

