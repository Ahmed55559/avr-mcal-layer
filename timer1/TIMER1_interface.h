// ========================================= TIMER1_interface.h =========================================
// Author: Eng. Ahmed Ashraf
// SWC: TIMER2
// Layer: MCAL
// Description: This file contains the public interface definitions for the TIMER2 peripheral.
// =====================================================================================================

#ifndef TIMER1_INTERFACE_H
#define TIMER1_INTERFACE_H

// max value of 16 bit is 65536
#define MAX_VALUE 65536
#define MIN_VALUE 0

typedef enum
{
    NORMAL,

    PWM_PHASE_8BIT,
    PWM_PHASE_9BIT,
    PWM_PHASE_10BIT,

    CTC_OCR1A,

    PWM_FAST_8BIT,
    PWM_FAST_9BIT,
    PWM_FAST_10BIT,

    PWM_PHASE_FREQ_ICR1,
    PWM_PHASE_FREQ_OCR1A,

    PWM_PHASE_ICR1,
    PWM_PHASE_OCR1A,

    CTC_ICR1,

    PWM_FAST_ICR1 = 14,
    PWM_FAST_OCR1A
} TIMER1_Mode;

typedef enum
{
    OVERFLOW = 0,
    COMPARE_MATCHA = 1,
    COMPARE_MATCHB = 2,
    INPUT_CAPTURE_INT = 5
} TIMER1_Interrupt;

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
} TIMER1_SysCLKPrescaler;

typedef enum
{
    OC_DISCONNECTED,
    OC_TOGGLE,
    OC_CLEAR,
    OC_SET
} TIMER1_OCMode;

typedef enum
{
    PWM_NON_INVERTING,
    PWM_INVERTING
} TIMER1_PWMMode;

typedef enum
{
    OC1A,
    OC1B
} TIMER1_OutputPin;

typedef enum
{
    TIMER1_OK,

    INVALID_TIMER_MODE,
    INVALID_OUTPUT_MODE,
    INVALID_OUTPUT_PIN,
    TIMER1_CURRENT_MODE_INCOMPATIBLE,
    INVALID_INTERRUPT,
    OUT_OF_RANGE,
    NULL_POINTER,
    GIE_DISABLED,
    TIMER1_SHUTDOWN,
} TIMER1_ErrorStatus;

void TIMER1_voidInit(void);

TIMER1_ErrorStatus TIMER1_enuSetClkSource(TIMER1_SysCLKPrescaler Copy_enuPrescaler);

TIMER1_ErrorStatus TIMER1_enuSetTimerMode(TIMER1_Mode Copy_enuMode);

TIMER1_ErrorStatus TIMER1_enuSetOCOutputMode(TIMER1_OCMode Copy_enuMode, TIMER1_OutputPin Copy_enuPin);

TIMER1_ErrorStatus TIMER1_enuSetPWMOutputMode(TIMER1_PWMMode Copy_enuMode, TIMER1_OutputPin Copy_enuPin);

TIMER1_ErrorStatus TIMER1_enuEnableInterrupt(TIMER1_Interrupt Copy_enuInterrupt);

TIMER1_ErrorStatus TIMER1_enuDisableInterrupt(TIMER1_Interrupt Copy_enuInterrupt);

TIMER1_ErrorStatus TIMER1_enuSetCallback(TIMER1_Interrupt Copy_enuInterrupt, void (*isr)(void));

TIMER1_ErrorStatus TIMER1_enuSetCounter(u16 Copy_u16Value);

TIMER1_ErrorStatus TIMER1_enuSetICUReg(u16 Copy_u16Value);

TIMER1_ErrorStatus TIMER1_enuSetCompareA(u16 Copy_u16Value);

TIMER1_ErrorStatus TIMER1_enuSetCompareB(u16 Copy_u16Value);

TIMER1_ErrorStatus TIMER1_enuGetCounter(u16 *Copy_pu16Value);

TIMER1_ErrorStatus TIMER1_enuGetICU(u16 *Copy_pu16Value);

TIMER1_ErrorStatus TIMER1_enuGetCompareA(u16 *Copy_pu16Value);

TIMER1_ErrorStatus TIMER1_enuGetCompareB(u16 *Copy_pu16Value);

TIMER1_ErrorStatus TIMER1_enuSetDutyCycle(TIMER1_OutputPin Copy_enuPin, u16 Copy_u16Value);

#endif /* TIMER1_INTERFACE_H */