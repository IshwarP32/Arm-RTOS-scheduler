/**
 * @author Team Member 5 - Timer/Interrupt Management
 */

#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include "rtos_config.h"

/* FUNCTION PROTOTYPES - TIMING UTILITIES*/

/**
 *For General RTOS
 */
uint32_t timer_calculate_slice_ticks(uint32_t time_slice_ms);

/**
 *Start SysTick timer with given reload value (polling mode)
 * @param reload_ticks Reload value for SysTick counter
 * This is for CortexM3
 */
void timer_start_slice(uint32_t reload_ticks);

/**N
 * Stop SysTick timer
 * This is for CortexM3
 */
void timer_stop_slice(void);

/**
 *Check if current time slice has expired
 * @return bool True if slice expired
 * This is for CortexM3
 */
bool timer_slice_expired(void);

#endif /* TIMER_MANAGER_H */

