// ========================================= EXTI_config.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: EXTI
// Layer: MCAL
// Description: This file contains the configuration information of EXTI peripheral in AVR
// =================================================================================================

#ifndef EXTI_CONFIG_H
#define EXTI_CONFIG_H

typedef enum
{
    DISABLE,
    ENABLE
} INITIAL_VALUE;

/* Global interrupt after initialization */
#define EXTI_GLOBAL_INTERRUPT_STATE ENABLE

/* Default sensing mode */
#define EXTI0_DEFAULT_SENSE FALLING_EDGE
#define EXTI1_DEFAULT_SENSE RISING_EDGE

/* Enable interrupts during initialization */
#define EXTI0_INITIAL_STATE ENABLE
#define EXTI1_INITIAL_STATE ENABLE

/* Enable Pin Change groups */
#define PCINT0_INITIAL_STATE DISABLE
#define PCINT1_INITIAL_STATE DISABLE
#define PCINT2_INITIAL_STATE DISABLE

#endif /* EXTI_CONFIG_H */