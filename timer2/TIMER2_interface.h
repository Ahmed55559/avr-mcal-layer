// ========================================= TIMER2_interface.h =========================================
// Author: Eng. Ahmed Ashraf
// SWC: TIMER2
// Layer: MCAL
// Description: This file contains the public interface definitions for the TIMER2 peripheral.
// =====================================================================================================

#ifndef TIMER2_INTERFACE_H
#define TIMER2_INTERFACE_H

#define MAX_VALUE 255
#define MIN_VALUE 0
typedef enum
{
    SYNC,
    ASYNC
} TIMER2_TimingMode;

typedef enum
{
    NORMAL,
    PWM_PC_FIXED,
    CTC,
    PWM_FAST_FIXED,
    PWM_PC_ADJ,
    PWM_FAST_ADJ,
} TIMER2_Mode;

typedef enum
{
    OVERFLOW = 0,
    COMPARE_MATCHA = 1,
    COMPARE_MATCHB = 2
} TIMER2_Interrupt;

typedef enum
{
    NO_CLK = 0,
    SYS_CLK,
    CLK_DIV_8,
    CLK_DIV_32,
    CLK_DIV_64,
    CLK_DIV_128,
    CLK_DIV_256,
    CLK_DIV_1024
} TIMER2_SysCLKPrescaler;

typedef enum oc_modes
{
    OC_DISCONNECTED,
    OC_TOGGLE,
    OC_CLEAR,
    OC_SET
} TIMER2_OCMode;

typedef enum
{
    PWM_NON_INVERTING,
    PWM_INVERTING
} TIMER2_PWMMode;

typedef enum
{
    OC2A,
    OC2B
} TIMER2_OutputPin;

typedef enum
{
    TIMER2_OK,

    INVALID_TIMER_MODE,
    INVALID_TIMING_MODE,
    INVALID_OUTPUT_MODE,
    INVALID_OUTPUT_PIN,
    TIMER2_CURRENT_MODE_INCOMPATIBLE,
    INVALID_INTERRUPT,
    INVALID_PRESCALER,
    OUT_OF_RANGE,
    NULL_POINTER,
    GIE_DISABLED,
    TIMER2_SHUTDOWN,
} TIMER2_ErrorStatus;

/**
 * @brief Initializes Timer2 using the settings defined in TIMER2_config.h.
 *
 * This function configures the timer operating mode, clock source,
 * interrupt settings, and any other static configuration options.
 */
void TIMER2_voidInit(void);

/**
 * @brief Configures the clock source of Timer2.
 *
 * @param Copy_enuPrescaler Desired system clock prescaler if in sync mode.
 *
 * @return
 * - TIMER2_OK on success.
 * - INVALID_PRESCALER if an invalid clock source is selected.
 */
TIMER2_ErrorStatus TIMER2_enuSetClkSource(TIMER2_SysCLKPrescaler Copy_enuPrescaler);

/**
 * @brief Configures the operating mode of Timer2.
 *
 * @param Copy_enuMode Desired Timer2 operating mode.
 *
 * @return
 * - TIMER2_OK on success.
 * - INVALID_TIMER_MODE if the selected mode is invalid.
 */
TIMER2_ErrorStatus TIMER2_enuSetTimerMode(TIMER2_Mode Copy_enuMode);

/**
 * @brief Configures the timing mode of Timer2.
 *
 * @param Copy_enuMode Desired Timer2 timing operating mode (sync,async).
 *
 * @return
 * - TIMER2_OK on success.
 * - INVALID_TIMING_MODE if the selected mode is invalid.
 */
TIMER2_ErrorStatus TIMER2_enuSetTimerTimingMode(TIMER2_TimingMode Copy_enuMode);

/**
 * @brief Configures the Output Compare (OC) pin behavior.
 *
 * Used in Normal and CTC modes to determine how the OC pin reacts
 * when a compare match occurs.
 *
 * @note This function is only applicable in Normal and CTC modes.
 *
 * @param Copy_enuMode Desired OC output mode.
 * @param Copy_enuPin Output compare channel (OC2A or OC2B).
 *
 * @return
 * - TIMER2_OK on success.
 * - INVALID_OUTPUT_MODE if the selected mode is invalid.
 * - INVALID_OUTPUT_PIN if the selected pin is invalid.
 */
TIMER2_ErrorStatus TIMER2_enuSetOCOutputMode(TIMER2_OCMode Copy_enuMode, TIMER2_OutputPin Copy_enuPin);

/**
 * @brief Configures the PWM output mode.
 *
 * Selects whether the PWM output operates in non-inverting
 * or inverting mode.
 *
 * @note This function is only applicable in PWM modes.
 *
 * @param Copy_enuMode Desired PWM output mode.
 * @param Copy_enuPin Output compare channel (OC2A or OC2B).
 *
 * @return
 * - TIMER2_OK on success.
 * - INVALID_OUTPUT_MODE if the selected mode is invalid.
 * - INVALID_OUTPUT_PIN if the selected pin is invalid.
 */
TIMER2_ErrorStatus TIMER2_enuSetPWMOutputMode(TIMER2_PWMMode Copy_enuMode, TIMER2_OutputPin Copy_enuPin);

/**
 * @brief Enables a Timer2 interrupt source.
 *
 * @param Copy_enuInterrupt Interrupt source to enable.
 *
 * @return
 * - TIMER2_OK on success.
 * - INVALID_INTERRUPT if the interrupt source is invalid.
 */
TIMER2_ErrorStatus TIMER2_enuEnableInterrupt(TIMER2_Interrupt Copy_enuInterrupt);

/**
 * @brief Disables a Timer2 interrupt source.
 *
 * @param Copy_enuInterrupt Interrupt source to disable.
 *
 * @return
 * - TIMER2_OK on success.
 * - INVALID_INTERRUPT if the interrupt source is invalid.
 */
TIMER2_ErrorStatus TIMER2_enuDisableInterrupt(TIMER2_Interrupt Copy_enuInterrupt);

/**
 * @brief Registers a callback function for a Timer2 interrupt.
 *
 * The callback will be executed whenever the selected interrupt
 * occurs and is enabled and its flag is raised.
 *
 * @param Copy_enuInterrupt Interrupt source.
 * @param isr Pointer to the callback function.
 *
 * @return
 * - TIMER2_OK on success.
 * - NULL_POINTER if the callback pointer is NULL.
 * - INVALID_INTERRUPT if the interrupt source is invalid.
 */
TIMER2_ErrorStatus TIMER2_enuSetCallback(TIMER2_Interrupt Copy_enuInterrupt, void (*isr)(void));

/**
 * @brief Writes a value to the Timer2 counter register.
 *
 * @param Copy_u8Value Counter value (0-255).
 *
 * @return
 * - TIMER2_OK on success.
 * - OUT_OF_RANGE if the value is invalid.
 */
TIMER2_ErrorStatus TIMER2_enuSetCounter(u8 Copy_u8Value);

/**
 * @brief Reads the current Timer2 counter value.
 *
 * @param Copy_pu8Value Pointer to store the counter value.
 *
 * @return
 * - TIMER2_OK on success.
 * - NULL_POINTER if value is NULL.
 */
TIMER2_ErrorStatus TIMER2_enuGetCounter(u8 *Copy_pu8Value);

/**
 * @brief Sets the compare value for channel A.
 *
 * @param Copy_u8Value Compare value (0-255).
 *
 * @return
 * - TIMER2_OK on success.
 */
TIMER2_ErrorStatus TIMER2_enuSetCompareA(u8 Copy_u8Value);

/**
 * @brief Sets the compare value for channel B.
 *
 * @param Copy_u8Value Compare value (0-255).
 *
 * @return
 * - TIMER2_OK on success.
 */
TIMER2_ErrorStatus TIMER2_enuSetCompareB(u8 Copy_u8Value);

/**
 * @brief Reads the compare value of channel A.
 *
 * @param Copy_pu8Value Pointer to store the compare value.
 *
 * @return
 * - TIMER2_OK on success.
 * - NULL_POINTER if value is NULL.
 */
TIMER2_ErrorStatus TIMER2_enuGetCompareA(u8 *Copy_pu8Value);

/**
 * @brief Reads the compare value of channel B.
 *
 * @param Copy_pu8Value Pointer to store the compare value.
 *
 * @return
 * - TIMER2_OK on success.
 * - NULL_POINTER if value is NULL.
 */
TIMER2_ErrorStatus TIMER2_enuGetCompareB(u8 *Copy_pu8Value);

/**
 * @brief Sets the PWM duty cycle.
 *
 * The duty cycle is specified as a percentage and is internally
 * converted to the appropriate compare value.
 *
 * @note This function is only applicable in PWM modes.
 *
 * @param Copy_enuPin PWM output channel (OC2A or OC2B).
 * @param Copy_u8Value Duty cycle percentage (0-100).
 *
 * @return
 * - TIMER2_OK on success.
 * - OUT_OF_RANGE if the percentage is greater than 100.
 * - INVALID_OUTPUT_PIN if the selected pin is invalid.
 * - TIMER2_CURRENT_MODE_INCOMPATIBLE if the current mode is not PWM or the selected pin is not compatible with the selected pwm mode
 */
TIMER2_ErrorStatus TIMER2_enuSetDutyCycle(TIMER2_OutputPin Copy_enuPin, u8 Copy_u8Value);

#endif /* TIMER2_INTERFACE_H */