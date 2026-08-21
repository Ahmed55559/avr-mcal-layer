// ========================================= TIMER0_interface.h =========================================
// Author: Eng. Ahmed Ashraf
// SWC: TIMER0
// Layer: MCAL
// Description: This file contains the public interface definitions for the TIMER0 peripheral.
// =====================================================================================================

#ifndef TIMER0_INTERFACE_H
#define TIMER0_INTERFACE_H

#define MAX_VALUE 255

typedef enum
{
    TIMER0_NORMAL,
    TIMER0_PWM_PC_FIXED,
    TIMER0_CTC,
    TIMER0_PWM_FAST_FIXED,
    TIMER0_PWM_PC_ADJ,
    TIMER0_PWM_FAST_ADJ,
} TIMER0_Mode;

typedef enum
{
    OVERFLOW = 0,
    COMPARE_MATCHA = 1,
    COMPARE_MATCHB = 2
} TIMER0_Interrupt;

typedef enum
{
    NO_CLK = 0,
    SYS_CLK,
    CLK_DIV_8,
    CLK_DIV_64,
    CLK_DIV_256,
    CLK_DIV_1024,
    EXT_CLK_FALLING,
    EXT_CLK_RISING = 7,
} TIMER0_ClkSource;

typedef enum
{
    OC_DISCONNECTED,
    OC_TOGGLE,
    OC_CLEAR,
    OC_SET
} TIMER0_OCMode;

typedef enum
{
    PWM_NON_INVERTING,
    PWM_INVERTING
} TIMER0_PWMMode;

typedef enum
{
    OC0A,
    OC0B
} TIMER0_OutputPin;

typedef enum
{
    TIMER0_OK,

    INVALID_TIMER_MODE,
    INVALID_OUTPUT_MODE,
    INVALID_OUTPUT_PIN,
    TIMER0_CURRENT_MODE_INCOMPATIBLE,
    INVALID_INTERRUPT,
    INVALID_PRESCALER,
    OUT_OF_RANGE,
    NULL_POINTER,
    GIE_DISABLED,
} TIMER0_ErrorStatus;

/**
 * @brief Initializes Timer0 using the settings defined in TIMER0_config.h.
 *
 * This function configures the timer operating mode, clock source,
 * interrupt settings, and any other static configuration options.
 */
void TIMER0_voidInit(void);

/**
 * @brief Configures the clock source of Timer0.
 *
 * @param Copy_enuSource Desired clock source or prescaler.
 *
 * @return
 * - TIMER0_OK on success.
 * - INVALID_PRESCALER if an invalid clock source is selected.
 */
TIMER0_ErrorStatus TIMER0_enuSetClkSource(TIMER0_ClkSource Copy_enuSource);

/**
 * @brief Configures the operating mode of Timer0.
 *
 * @param Copy_enuMode Desired Timer0 operating mode.
 *
 * @return
 * - TIMER0_OK on success.
 * - INVALID_TIMER_MODE if the selected mode is invalid.
 */
TIMER0_ErrorStatus TIMER0_enuSetTimerMode(TIMER0_Mode Copy_enuMode);

/**
 * @brief Configures the Output Compare (OC) pin behavior.
 *
 * Used in Normal and CTC modes to determine how the OC pin reacts
 * when a compare match occurs.
 *
 * @note This function is only applicable in Normal and CTC modes.
 *
 * @param Copy_enuMode Desired OC output mode.
 * @param Copy_enuPin Output compare channel (OC0A or OC0B).
 *
 * @return
 * - TIMER0_OK on success.
 * - INVALID_OUTPUT_MODE if the selected mode is invalid.
 * - INVALID_OUTPUT_PIN if the selected pin is invalid.
 */
TIMER0_ErrorStatus TIMER0_enuSetOCOutputMode(TIMER0_OCMode Copy_enuMode,
                                             TIMER0_OutputPin Copy_enuPin);

/**
 * @brief Configures the PWM output mode.
 *
 * Selects whether the PWM output operates in non-inverting
 * or inverting mode.
 *
 * @note This function is only applicable in PWM modes.
 *
 * @param Copy_enuMode Desired PWM output mode.
 * @param Copy_enuPin Output compare channel (OC0A or OC0B).
 *
 * @return
 * - TIMER0_OK on success.
 * - INVALID_OUTPUT_MODE if the selected mode is invalid.
 * - INVALID_OUTPUT_PIN if the selected pin is invalid.
 */
TIMER0_ErrorStatus TIMER0_enuSetPWMOutputMode(TIMER0_PWMMode Copy_enuMode,
                                              TIMER0_OutputPin Copy_enuPin);

/**
 * @brief Enables a Timer0 interrupt source.
 *
 * @param Copy_enuInterrupt Interrupt source to enable.
 *
 * @return
 * - TIMER0_OK on success.
 * - INVALID_INTERRUPT if the interrupt source is invalid.
 */
TIMER0_ErrorStatus TIMER0_enuEnableInterrupt(TIMER0_Interrupt Copy_enuInterrupt);

/**
 * @brief Disables a Timer0 interrupt source.
 *
 * @param Copy_enuInterrupt Interrupt source to disable.
 *
 * @return
 * - TIMER0_OK on success.
 * - INVALID_INTERRUPT if the interrupt source is invalid.
 */
TIMER0_ErrorStatus TIMER0_enuDisableInterrupt(TIMER0_Interrupt Copy_enuInterrupt);

/**
 * @brief Registers a callback function for a Timer0 interrupt.
 *
 * The callback will be executed whenever the selected interrupt
 * occurs and is enabled and its flag is raised.
 *
 * @param Copy_enuInterrupt Interrupt source.
 * @param isr Pointer to the callback function.
 *
 * @return
 * - TIMER0_OK on success.
 * - NULL_POINTER if the callback pointer is NULL.
 * - INVALID_INTERRUPT if the interrupt source is invalid.
 */
TIMER0_ErrorStatus TIMER0_enuSetCallback(TIMER0_Interrupt Copy_enuInterrupt,
                                         void (*isr)(void));

/**
 * @brief Writes a value to the Timer0 counter register.
 *
 * @param Copy_u8Value Counter value (0-255).
 *
 * @return
 * - TIMER0_OK on success.
 * - OUT_OF_RANGE if the value is invalid.
 */
TIMER0_ErrorStatus TIMER0_enuSetCounter(u8 Copy_u8Value);

/**
 * @brief Reads the current Timer0 counter value.
 *
 * @param Copy_pu8Value Pointer to store the counter value.
 *
 * @return
 * - TIMER0_OK on success.
 * - NULL_POINTER if value is NULL.
 */
TIMER0_ErrorStatus TIMER0_enuGetCounter(u8 *Copy_pu8Value);

/**
 * @brief Sets the compare value for channel A.
 *
 * @param Copy_u8Value Compare value (0-255).
 *
 * @return
 * - TIMER0_OK on success.
 */
TIMER0_ErrorStatus TIMER0_enuSetCompareA(u8 Copy_u8Value);

/**
 * @brief Sets the compare value for channel B.
 *
 * @param Copy_u8Value Compare value (0-255).
 *
 * @return
 * - TIMER0_OK on success.
 */
TIMER0_ErrorStatus TIMER0_enuSetCompareB(u8 Copy_u8Value);

/**
 * @brief Reads the compare value of channel A.
 *
 * @param Copy_pu8Value Pointer to store the compare value.
 *
 * @return
 * - TIMER0_OK on success.
 * - NULL_POINTER if value is NULL.
 */
TIMER0_ErrorStatus TIMER0_enuGetCompareA(u8 *Copy_pu8Value);

/**
 * @brief Reads the compare value of channel B.
 *
 * @param Copy_pu8Value Pointer to store the compare value.
 *
 * @return
 * - TIMER0_OK on success.
 * - NULL_POINTER if value is NULL.
 */
TIMER0_ErrorStatus TIMER0_enuGetCompareB(u8 *Copy_pu8Value);

/**
 * @brief Sets the PWM duty cycle.
 *
 * The duty cycle is specified as a percentage and is internally
 * converted to the appropriate compare value.
 *
 * @note This function is only applicable in PWM modes.
 *
 * @param Copy_enuPin PWM output channel (OC0A or OC0B).
 * @param Copy_u8Value Duty cycle percentage (0-100).
 *
 * @return
 * - TIMER0_OK on success.
 * - OUT_OF_RANGE if the percentage is greater than 100.
 * - INVALID_OUTPUT_PIN if the selected pin is invalid.
 * - TIMER0_CURRENT_MODE_INCOMPATIBLE if the current mode is not PWM or the selected pin is not compatible with the selected pwm mode
 */
TIMER0_ErrorStatus TIMER0_enuSetDutyCycle(TIMER0_OutputPin Copy_enuPin,
                                          u8 Copy_u8Value);

#endif /* TIMER0_INTERFACE_H */