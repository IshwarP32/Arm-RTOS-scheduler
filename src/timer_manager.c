/**
 * @file timer_manager.c
 * @brief Timer and Interrupt Management Implementation
 * @author Team Member 5 - Timer/Interrupt Management
 * @date 2024
 * 
 * This module handles hardware timer (SysTick) configuration and timing utilities
 * for the RTOS scheduler.
 */

#include "timer_manager.h"
#include "arm_cortex_m.h"

/* ============================================================================
 * PUBLIC FUNCTIONS - TIMING UTILITIES
 * ============================================================================ */

/**
 * @brief Calculate SysTick reload value for given time slice in milliseconds
 * @param time_slice_ms Time slice duration in milliseconds
 * @return uint32_t SysTick reload value (clamped to 24-bit range)
 */
uint32_t timer_calculate_slice_ticks(uint32_t time_slice_ms)
{
    const uint32_t ticks_per_ms = (SYSTEM_CLOCK_HZ / 1000U);
    uint32_t slice_ticks = ticks_per_ms * time_slice_ms;
    
    if (slice_ticks == 0U) {
        slice_ticks = 1U; /* Minimum 1 tick */
    }
    if (slice_ticks > 0x00FFFFFFU) {
        slice_ticks = 0x00FFFFFFU; /* SysTick is 24-bit */
    }
    
    return slice_ticks;
}

/**
 * @brief Start SysTick timer with given reload value (polling mode)
 * @param reload_ticks Reload value for SysTick counter
 */
void timer_start_slice(uint32_t reload_ticks)
{
    SYSTICK_CTRL_REG = 0;                       /* Stop */
    SYSTICK_LOAD_REG = reload_ticks - 1U;       /* Set reload */
    SYSTICK_VAL_REG  = 0;                       /* Clear current value */
    SYSTICK_CTRL_REG = SYSTICK_ENABLE | SYSTICK_CLKSOURCE; /* Start with CPU clock */
}

/**
 * @brief Stop SysTick timer
 */
void timer_stop_slice(void)
{
    SYSTICK_CTRL_REG = 0;
}

/**
 * @brief Check if current time slice has expired
 * @return bool True if slice expired (COUNTFLAG set)
 */
bool timer_slice_expired(void)
{
    return ((SYSTICK_CTRL_REG & SYSTICK_COUNTFLAG) != 0);
}
