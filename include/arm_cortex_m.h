#ifndef ARM_CORTEX_M_H 
#define ARM_CORTEX_M_H

#include "rtos_config.h"
#ifdef __ARMCC_VERSION
#include <arm_compat.h>
#endif

// Interrupt control and state register 
#define NVIC_INT_CTRL_REG       (*((volatile uint32_t*)0xE000ED04))
#define NVIC_PENDSVSET          0x10000000

// System control block registers
#define NVIC_SYSPRI2_REG        (*((volatile uint32_t*)0xE000ED20))
#define NVIC_SYSPRI3_REG        (*((volatile uint32_t*)0xE000ED24))
#define NVIC_PENDSV_PRI         (0xFF << 16)
#define NVIC_SYSTICK_PRI        (0xFF << 24)

// SysTick timer registers
#define SYSTICK_CTRL_REG        (*((volatile uint32_t*)0xE000E010))
#define SYSTICK_LOAD_REG        (*((volatile uint32_t*)0xE000E014))
#define SYSTICK_VAL_REG         (*((volatile uint32_t*)0xE000E018))
#define SYSTICK_CALIB_REG       (*((volatile uint32_t*)0xE000E01C))

// SysTick control bits 
#define SYSTICK_ENABLE          (1 << 0)
#define SYSTICK_TICKINT         (1 << 1)
#define SYSTICK_CLKSOURCE       (1 << 2)
#define SYSTICK_COUNTFLAG       (1 << 16)

#endif /* ARM_CORTEX_M_H */