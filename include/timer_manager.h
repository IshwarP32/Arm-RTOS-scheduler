/**
 * @file timer_manager.h
 * @brief Timer and Interrupt Management Interface
 * @author Team Member 5 - Timer/Interrupt Management
 * @date 2024
 * 
 * This module handles hardware timer (SysTick) configuration and timing utilities.
 */

#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include "rtos_config.h"

/* ============================================================================
 * FUNCTION PROTOTYPES - TIMING UTILITIES
 * ============================================================================ */

/**
 * @brief Calculate SysTick reload value for given time slice
 * @param time_slice_ms Time slice duration in milliseconds
 * @return uint32_t SysTick reload value
 */
uint32_t timer_calculate_slice_ticks(uint32_t time_slice_ms);

/**
 * @brief Start SysTick timer with given reload value (polling mode)
 * @param reload_ticks Reload value for SysTick counter
 */
void timer_start_slice(uint32_t reload_ticks);

/**
 * @brief Stop SysTick timer
 */
void timer_stop_slice(void);

/**
 * @brief Check if current time slice has expired
 * @return bool True if slice expired
 */
bool timer_slice_expired(void);

#endif /* TIMER_MANAGER_H */
