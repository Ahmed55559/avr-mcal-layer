// ========================================= TIMER1_program.c =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: TIMER0
// Layer: MCAL
// Description: This file contains the implementation of TIMER0 functions
// =================================================================================================

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "TIMER1_interface.h"
#include "TIMER1_config.h"
#include "TIMER1_private.h"

static void (*TIMER1_Callback[4])(void) = {NULL};

void TIMER1_voidInit(void)
{
    switch (INITIAL_TIMER1_MODE)
    {
    case NORMAL:
        TCCR1A &= 0b11111100;
        TCCR1B &= 0b11100111;
        break;
    case PWM_PHASE_8BIT:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM10);
        TCCR1B &= 0b11100111;
        break;
    case PWM_PHASE_9BIT:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM11);
        TCCR1B &= 0b11100111;
        break;
    case PWM_PHASE_10BIT:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM10) | (1 << WGM11);
        TCCR1B &= 0b11100111;
        break;
    case CTC_OCR1A:
        TCCR1A &= 0b11111100;
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM12);
        break;
    case PWM_FAST_8BIT:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM10);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM12);
        break;
    case PWM_FAST_9BIT:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM11);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM12);
        break;
    case PWM_FAST_10BIT:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM10) | (1 << WGM11);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM12);
        break;
    case PWM_PHASE_FREQ_ICR1:
        TCCR1A &= 0b11111100;
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM13);
        break;
    case PWM_PHASE_FREQ_OCR1A:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM10);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM13);
        break;
    case PWM_PHASE_ICR1:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM11);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM13);

        break;
    case PWM_PHASE_OCR1A:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM10) | (1 << WGM11);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM13);
        break;
    case CTC_ICR1:
        TCCR1A &= 0b11111100;
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM13) | (1 << WGM12);

        break;
    case PWM_FAST_ICR1:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM11);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM13) | (1 << WGM12);
        break;
    case PWM_FAST_OCR1A:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM10) | (1 << WGM11);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM13) | (1 << WGM12);
        break;
    }

    TCCR1B = (TCCR1B & 0b11111000) | INITIAL_TIMER1_CLK;

    TCNT1L |= INITIAL_TIMER1_COUNTER_VALUE & 0b0000000011111111;
    TCNT1H |= INITIAL_TIMER1_COUNTER_VALUE & 0b1111111100000000;

    OCR1AL |= INITIAL_TIMER1_COUNTER_VALUE & 0b0000000011111111;
    OCR1AH |= INITIAL_TIMER1_COUNTER_VALUE & 0b1111111100000000;

    OCR1BL |= INITIAL_TIMER1_COUNTER_VALUE & 0b0000000011111111;
    OCR1BH |= INITIAL_TIMER1_COUNTER_VALUE & 0b1111111100000000;

    if (INITIAL_TIMER1_MODE != CTC_ICR1 && INITIAL_TIMER1_MODE != PWM_FAST_ICR1 && INITIAL_TIMER1_MODE != PWM_PHASE_FREQ_ICR1 && INITIAL_TIMER1_MODE != PWM_PHASE_ICR1)
    {
        ICR1L |= INITIAL_TIMER1_COUNTER_VALUE & 0b0000000011111111;
        ICR1H |= INITIAL_TIMER1_COUNTER_VALUE & 0b1111111100000000;
    }
}

TIMER1_ErrorStatus TIMER1_enuSetClkSource(TIMER1_SysCLKPrescaler Copy_enuPrescaler)
{
    if (!GET_BIT(PRR, PRTIM1))
        return TIMER1_SHUTDOWN;
    if (Copy_enuPrescaler > EXT_CLK_RISING || Copy_enuPrescaler < NO_CLK)
        return OUT_OF_RANGE;

    TCCR1B = (TCCR1B & (~0 < 3)) | Copy_enuPrescaler;
    return TIMER1_OK;
}

TIMER1_ErrorStatus TIMER1_enuSetTimerMode(TIMER1_Mode Copy_enuMode)
{
    if (!GET_BIT(PRR, PRTIM1))
        return TIMER1_SHUTDOWN;
    if (Copy_enuMode > PWM_FAST_OCR1A || Copy_enuMode < NORMAL)
        return INVALID_TIMER_MODE;

    switch (Copy_enuMode)
    {
    case NORMAL:
        TCCR1A &= 0b11111100;
        TCCR1B &= 0b11100111;
        break;
    case PWM_PHASE_8BIT:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM10);
        TCCR1B &= 0b11100111;
        break;
    case PWM_PHASE_9BIT:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM11);
        TCCR1B &= 0b11100111;
        break;
    case PWM_PHASE_10BIT:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM10) | (1 << WGM11);
        TCCR1B &= 0b11100111;
        break;
    case CTC_OCR1A:
        TCCR1A &= 0b11111100;
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM12);
        break;
    case PWM_FAST_8BIT:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM10);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM12);
        break;
    case PWM_FAST_9BIT:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM11);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM12);
        break;
    case PWM_FAST_10BIT:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM10) | (1 << WGM11);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM12);
        break;
    case PWM_PHASE_FREQ_ICR1:
        TCCR1A &= 0b11111100;
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM13);
        break;
    case PWM_PHASE_FREQ_OCR1A:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM10);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM13);
        break;
    case PWM_PHASE_ICR1:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM11);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM13);

        break;
    case PWM_PHASE_OCR1A:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM10) | (1 << WGM11);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM13);
        break;
    case CTC_ICR1:
        TCCR1A &= 0b11111100;
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM13) | (1 << WGM12);

        break;
    case PWM_FAST_ICR1:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM11);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM13) | (1 << WGM12);
        break;
    case PWM_FAST_OCR1A:
        TCCR1A = (TCCR1A & 0b11111100) | (1 << WGM10) | (1 << WGM11);
        TCCR1B = (TCCR1B & 0b11100111) | (1 << WGM13) | (1 << WGM12);
        break;
    }
    return TIMER1_OK;
}

TIMER1_ErrorStatus TIMER1_enuSetOCOutputMode(TIMER1_OCMode Copy_enuMode, TIMER1_OutputPin Copy_enuPin)
{
    if (!GET_BIT(PRR, PRTIM1))
        return TIMER1_SHUTDOWN;
    if (Copy_enuMode > OC_SET || Copy_enuMode < OC_DISCONNECTED)
        return INVALID_OUTPUT_MODE;
    if (Copy_enuPin > OC1B || Copy_enuPin < OC1A)
        return INVALID_OUTPUT_PIN;

    switch (Copy_enuPin)
    {
    case OC1B:
        TCCR1B &= ~(0x03 << 4);
        TCCR1B |= (Copy_enuMode << 4);
        break;
    case OC1A:
        TCCR1A &= ~(0x03 << 6);
        TCCR1A |= (Copy_enuMode << 6);
        break;
    }
    return TIMER1_OK;
}

TIMER1_ErrorStatus TIMER1_enuSetPWMOutputMode(TIMER1_PWMMode Copy_enuMode, TIMER1_OutputPin Copy_enuPin)
{
    if (!GET_BIT(PRR, PRTIM1))
        return TIMER1_SHUTDOWN;
    if (Copy_enuMode != PWM_INVERTING && Copy_enuMode != PWM_NON_INVERTING)
        return INVALID_OUTPUT_MODE;
    if (Copy_enuPin > OC1B || Copy_enuPin < OC1A)
        return INVALID_OUTPUT_PIN;
    u8 local_u8CurrentTimerMode = GET_BIT(TCCR1B, WGM13) | GET_BIT(TCCR1B, WGM12) | GET_BIT(TCCR1A, WGM11) | GET_BIT(TCCR1A, WGM10);
    if (local_u8CurrentTimerMode == NORMAL || local_u8CurrentTimerMode == CTC_ICR1 || local_u8CurrentTimerMode == CTC_OCR1A)
        return TIMER1_CURRENT_MODE_INCOMPATIBLE;

    switch (Copy_enuPin)
    {
    case OC1B:
        TCCR1B &= ~(0x03 << 4);
        TCCR1B |= (Copy_enuMode << 4);
        break;
    case OC1A:
        TCCR1A &= ~(0x03 << 6);
        TCCR1A |= (Copy_enuMode << 6);
        break;
    }
    return TIMER1_OK;
}

TIMER1_ErrorStatus TIMER1_enuEnableInterrupt(TIMER1_Interrupt Copy_enuInterrupt)
{
    if (!GET_BIT(PRR, PRTIM1))
        return TIMER1_SHUTDOWN;
    if (Copy_enuInterrupt > INPUT_CAPTURE_INT || Copy_enuInterrupt < OVERFLOW)
        return INVALID_INTERRUPT;
    if (!GET_BIT(SREG, GIE))
        return GIE_DISABLED;

    SET_BIT(TIMSK1, Copy_enuInterrupt);
    return TIMER1_OK;
}

TIMER1_ErrorStatus TIMER1_enuDisableInterrupt(TIMER1_Interrupt Copy_enuInterrupt)
{
    if (!GET_BIT(PRR, PRTIM1))
        return TIMER1_SHUTDOWN;
    if (Copy_enuInterrupt > INPUT_CAPTURE_INT || Copy_enuInterrupt < OVERFLOW)
        return INVALID_INTERRUPT;
    if (!GET_BIT(SREG, GIE))
        return GIE_DISABLED;

    CLR_BIT(TIMSK1, Copy_enuInterrupt);
    return TIMER1_OK;
}

TIMER1_ErrorStatus TIMER1_enuSetCallback(TIMER1_Interrupt Copy_enuInterrupt, void (*isr)(void))
{
    if (isr != NULL)
    {
        TIMER1_Callback[Copy_enuInterrupt] = isr;
    }
}
void __vector_14(void) __attribute__((signal));
void __vector_14(void)
{
    if (TIMER1_Callback[0] != NULL)
    {
        TIMER1_Callback[0]();
    }
}
void __vector_12(void) __attribute__((signal));
void __vector_12(void)
{
    if (TIMER1_Callback[1] != NULL)
    {
        TIMER1_Callback[1]();
    }
}
void __vector_13(void) __attribute__((signal));
void __vector_13(void)
{
    if (TIMER1_Callback[2] != NULL)
    {
        TIMER1_Callback[2]();
    }
}
void __vector_11(void) __attribute__((signal));
void __vector_11(void)
{
    if (TIMER1_Callback[3] != NULL)
    {
        TIMER1_Callback[3]();
    }
}

TIMER1_ErrorStatus TIMER1_enuSetCounter(u16 Copy_u16Value)
{
    if (Copy_u16Value > MAX_VALUE || Copy_u16Value < MIN_VALUE)
        return OUT_OF_RANGE;

    TCNT1L = Copy_u16Value && 0b0000000011111111;
    TCNT1H = Copy_u16Value && 0b1111111100000000;

    return TIMER1_OK;
}

TIMER1_ErrorStatus TIMER1_enuSetICUReg(u16 Copy_u16Value)
{
    if (Copy_u16Value > MAX_VALUE || Copy_u16Value < MIN_VALUE)
        return OUT_OF_RANGE;
    u8 mode = GET_BIT(TCCR1B, WGM13) | GET_BIT(TCCR1B, WGM12) | GET_BIT(TCCR1A, WGM11) | GET_BIT(TCCR1A, WGM10);
    if (mode == NORMAL || mode == CTC_ICR1 || mode == CTC_OCR1A)
        return TIMER1_CURRENT_MODE_INCOMPATIBLE;

    ICR1L = Copy_u16Value && 0b0000000011111111;
    ICR1H = Copy_u16Value && 0b1111111100000000;

    return TIMER1_OK;
}

TIMER1_ErrorStatus TIMER1_enuSetCompareA(u16 Copy_u16Value)
{
    if (Copy_u16Value > MAX_VALUE || Copy_u16Value < MIN_VALUE)
        return OUT_OF_RANGE;

    OCR1AL = Copy_u16Value && 0b0000000011111111;
    OCR1AH = Copy_u16Value && 0b1111111100000000;

    return TIMER1_OK;
}

TIMER1_ErrorStatus TIMER1_enuSetCompareB(u16 Copy_u16Value)
{
    if (Copy_u16Value > MAX_VALUE || Copy_u16Value < MIN_VALUE)
        return OUT_OF_RANGE;

    OCR1BL = Copy_u16Value && 0b0000000011111111;
    OCR1BH = Copy_u16Value && 0b1111111100000000;

    return TIMER1_OK;
}

TIMER1_ErrorStatus TIMER1_enuGetCounter(u16 *Copy_pu16Value)
{
    if (Copy_pu16Value == NULL)
        return NULL_POINTER;
    u8 low = TCNT1L;
    u8 high = TCNT1H;
    *Copy_pu16Value = (high << 8) | low;
    return TIMER1_OK;
}
TIMER1_ErrorStatus TIMER1_enuGetICU(u16 *Copy_pu16Value)
{
    if (Copy_pu16Value == NULL)
        return NULL_POINTER;
    u8 low = ICR1L;
    u8 high = ICR1H;
    *Copy_pu16Value = (high << 8) | low;
    return TIMER1_OK;
}

TIMER1_ErrorStatus TIMER1_enuGetCompareA(u16 *Copy_pu16Value)
{
    if (Copy_pu16Value == NULL)
        return NULL_POINTER;
    u8 low = OCR1AL;
    u8 high = OCR1AH;
    *Copy_pu16Value = (high << 8) | low;
    return TIMER1_OK;
}

TIMER1_ErrorStatus TIMER1_enuGetCompareB(u16 *Copy_pu16Value)
{
    if (Copy_pu16Value == NULL)
        return NULL_POINTER;
    u8 low = OCR1BL;
    u8 high = OCR1BH;
    *Copy_pu16Value = (high << 8) | low;
    return TIMER1_OK;
}

TIMER1_ErrorStatus TIMER1_enuSetDutyCycle(TIMER1_OutputPin Copy_enuPin, u16 Copy_u16Value)
{
    if (!GET_BIT(PRR, PRTIM1))
        return TIMER1_SHUTDOWN;
    if (Copy_u16Value > MAX_VALUE || Copy_u16Value < MIN_VALUE)
        return OUT_OF_RANGE;
    if (Copy_enuPin > OC1B || Copy_enuPin < OC1A)
        return INVALID_OUTPUT_PIN;

    u8 local_u8CurrentTimerMode = (GET_BIT(TCCR1B, WGM13)) | (GET_BIT(TCCR1B, WGM12)) | (GET_BIT(TCCR1A, WGM11)) | (GET_BIT(TCCR1A, WGM10));
    if (local_u8CurrentTimerMode == NORMAL || local_u8CurrentTimerMode == CTC_ICR1 || local_u8CurrentTimerMode == CTC_OCR1A)
        return TIMER1_CURRENT_MODE_INCOMPATIBLE;

    u16 top;
    switch (local_u8CurrentTimerMode)
    {
    case PWM_PHASE_8BIT:
        top = 0x00FF;
        break;
    case PWM_PHASE_9BIT:
        top = 0x01FF;
        break;
    case PWM_PHASE_10BIT:
        top = 0x03FF;
        break;
    case PWM_FAST_8BIT:
        top = 0x00FF;

        break;
    case PWM_FAST_9BIT:
        top = 0x01FF;
        break;
    case PWM_FAST_10BIT:
        top = 0x03FF;
        break;
    case PWM_PHASE_FREQ_ICR1:
        TIMER1_enuGetICU(&top);
        break;
    case PWM_PHASE_FREQ_OCR1A:
        TIMER1_enuGetCompareA(&top);
        break;
    case PWM_PHASE_ICR1:
        TIMER1_enuGetICU(&top);
        break;
    case PWM_PHASE_OCR1A:
        TIMER1_enuGetCompareA(&top);
        break;
    case PWM_FAST_ICR1:
        TIMER1_enuGetICU(&top);
        break;
    case PWM_FAST_OCR1A:
        TIMER1_enuGetCompareA(&top);
        break;
    }

    if ((local_u8CurrentTimerMode == PWM_FAST_OCR1A || local_u8CurrentTimerMode == PWM_PHASE_OCR1A || local_u8CurrentTimerMode == PWM_PHASE_FREQ_OCR1A) && Copy_enuPin == OC1A)
        return TIMER1_CURRENT_MODE_INCOMPATIBLE;

    u16 result = GET_BIT(TCCR1A, COM1A0) ? (1 - (Copy_u16Value / 100)) * top : (Copy_u16Value / 100) * top;
    if (Copy_enuPin == OC1A)
    {
        OCR1AL = (result & 0x00FF);
        OCR1AH = (result & 0xFF00) >> 8;
    }
    else
    {
        OCR1BL = (result & 0x00FF);
        OCR1BH = (result & 0xFF00) >> 8;
    }
    return TIMER1_OK;
}
