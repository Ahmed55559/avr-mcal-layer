// ========================================= TIMER0_program.c =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: TIMER0
// Layer: MCAL
// Description: This file contains the implementation of TIMER0 functions
// =================================================================================================

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "TIMER0_interface.h"
#include "TIMER0_config.h"
#include "TIMER0_private.h"
static void (*TIMER0_Callback[3])(void) = {NULL};

void TIMER0_voidInit(void)
{
    switch (INITIAL_TIMER0_MODE)
    {
    case TIMER0_NORMAL:
        CLR_BIT(TCCR0A, WGM00);
        CLR_BIT(TCCR0A, WGM01);
        CLR_BIT(TCCR0B, WGM02);
        break;
    case TIMER0_PWM_PC_FIXED:
        SET_BIT(TCCR0A, WGM00);
        CLR_BIT(TCCR0A, WGM01);
        CLR_BIT(TCCR0B, WGM02);
        break;
    case TIMER0_CTC:
        CLR_BIT(TCCR0A, WGM00);
        SET_BIT(TCCR0A, WGM01);
        CLR_BIT(TCCR0B, WGM02);
        break;
    case TIMER0_PWM_FAST_FIXED:
        SET_BIT(TCCR0A, WGM00);
        SET_BIT(TCCR0A, WGM01);
        CLR_BIT(TCCR0B, WGM02);
        break;
    case TIMER0_PWM_PC_ADJ:
        SET_BIT(TCCR0A, WGM00);
        CLR_BIT(TCCR0A, WGM01);
        SET_BIT(TCCR0B, WGM02);
        break;
    case TIMER0_PWM_FAST_ADJ:
        SET_BIT(TCCR0A, WGM00);
        SET_BIT(TCCR0A, WGM01);
        SET_BIT(TCCR0B, WGM02);
        break;
    }

    TCCR0B = (TCCR0B & ~(0x07)) | INITIAL_TIMER0_CLK;

    TCNT0 = INITIAL_TIMER0_COUNTER_VALUE;
    OCR0A = INITIAL_TIMER0_OC0RA_VALUE;
    OCR0B = INITIAL_TIMER0_OC0RB_VALUE;
}

TIMER0_ErrorStatus TIMER0_enuSetClkSource(TIMER0_ClkSource Copy_enuSource)
{

    if (Copy_enuSource > EXT_CLK_RISING || Copy_enuSource < NO_CLK)
        return INVALID_PRESCALER;

    TCCR0B = (TCCR0B & ~(0x07)) | Copy_enuSource;
    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetTimerMode(TIMER0_Mode Copy_enuMode)
{

    if (Copy_enuMode > TIMER0_PWM_FAST_ADJ || Copy_enuMode < TIMER0_NORMAL)
        return INVALID_TIMER_MODE;

    switch (Copy_enuMode)
    {
    case TIMER0_NORMAL:
        CLR_BIT(TCCR0A, WGM00);
        CLR_BIT(TCCR0A, WGM01);
        CLR_BIT(TCCR0B, WGM02);
        break;
    case TIMER0_PWM_PC_FIXED:
        SET_BIT(TCCR0A, WGM00);
        CLR_BIT(TCCR0A, WGM01);
        CLR_BIT(TCCR0B, WGM02);
        break;
    case TIMER0_CTC:
        CLR_BIT(TCCR0A, WGM00);
        SET_BIT(TCCR0A, WGM01);
        CLR_BIT(TCCR0B, WGM02);
        break;
    case TIMER0_PWM_FAST_FIXED:
        SET_BIT(TCCR0A, WGM00);
        SET_BIT(TCCR0A, WGM01);
        CLR_BIT(TCCR0B, WGM02);
        break;
    case TIMER0_PWM_PC_ADJ:
        SET_BIT(TCCR0A, WGM00);
        CLR_BIT(TCCR0A, WGM01);
        SET_BIT(TCCR0B, WGM02);
        break;
    case TIMER0_PWM_FAST_ADJ:
        SET_BIT(TCCR0A, WGM00);
        SET_BIT(TCCR0A, WGM01);
        SET_BIT(TCCR0B, WGM02);
        break;
    }

    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetOCOutputMode(TIMER0_OCMode Copy_enuMode,
                                             TIMER0_OutputPin Copy_enuPin)
{

    if (Copy_enuMode > OC_SET || Copy_enuMode < OC_DISCONNECTED)
        return INVALID_OUTPUT_MODE;
    if (Copy_enuPin > OC0B || Copy_enuPin < OC0A)
        return INVALID_OUTPUT_PIN;

    switch (Copy_enuPin)
    {
    case OC0B:
        TCCR0B &= ~(0x03 << 4);
        TCCR0B |= (Copy_enuMode << 4);
        break;
    case OC0A:
        TCCR0A &= ~(0x03 << 6);
        TCCR0A |= (Copy_enuMode << 6);
        break;
    }
    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetPWMOutputMode(TIMER0_PWMMode Copy_enuMode,
                                              TIMER0_OutputPin Copy_enuPin)
{

    if (Copy_enuMode != PWM_INVERTING && Copy_enuMode != PWM_NON_INVERTING)
        return INVALID_OUTPUT_MODE;
    if (Copy_enuPin > OC0B || Copy_enuPin < OC0A)
        return INVALID_OUTPUT_PIN;
    if (!GET_BIT(TCCR0A, WGM00))
        return TIMER0_CURRENT_MODE_INCOMPATIBLE;

    switch (Copy_enuPin)
    {
    case OC0B:
        TCCR0B &= ~(0x03 << 4);
        TCCR0B |= (Copy_enuMode << 4);
        break;
    case OC0A:
        TCCR0A &= ~(0x03 << 6);
        TCCR0A |= (Copy_enuMode << 6);
        break;
    }
    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuEnableInterrupt(TIMER0_Interrupt Copy_enuInterrupt)
{

    if (Copy_enuInterrupt > COMPARE_MATCHB || Copy_enuInterrupt < OVERFLOW)
        return INVALID_INTERRUPT;

    SET_BIT(TIMSK0, Copy_enuInterrupt);
    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuDisableInterrupt(TIMER0_Interrupt Copy_enuInterrupt)
{

    if (Copy_enuInterrupt > COMPARE_MATCHB || Copy_enuInterrupt < OVERFLOW)
        return INVALID_INTERRUPT;

    CLR_BIT(TIMSK0, Copy_enuInterrupt);
    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetCallback(TIMER0_Interrupt Copy_enuInterrupt,
                                         void (*isr)(void))
{
    if (Copy_enuInterrupt < OVERFLOW || Copy_enuInterrupt > COMPARE_MATCHB)
        return INVALID_INTERRUPT;
    if (isr == NULL)
        return NULL_POINTER;

    TIMER0_Callback[Copy_enuInterrupt] = isr;

    return TIMER0_OK;
}
void __vector_17(void) __attribute__((signal));
void __vector_17(void)
{
    if (TIMER0_Callback[0] != NULL)
    {
        TIMER0_Callback[0]();
    }
}
void __vector_15(void) __attribute__((signal));
void __vector_15(void)
{
    if (TIMER0_Callback[1] != NULL)
    {
        TIMER0_Callback[1]();
    }
}
void __vector_16(void) __attribute__((signal));
void __vector_16(void)
{
    if (TIMER0_Callback[2] != NULL)
    {
        TIMER0_Callback[2]();
    }
}

TIMER0_ErrorStatus TIMER0_enuSetCounter(u8 Copy_u8Value)
{
    if (Copy_u8Value > MAX_VALUE)
        return OUT_OF_RANGE;

    TCNT0 = Copy_u8Value;
    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetCompareA(u8 Copy_u8Value)
{
    if (Copy_u8Value > MAX_VALUE)
        return OUT_OF_RANGE;

    OCR0A = Copy_u8Value;
    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetCompareB(u8 Copy_u8Value)
{
    if (Copy_u8Value > MAX_VALUE)
        return OUT_OF_RANGE;

    OCR0B = Copy_u8Value;
    return TIMER0_OK;
}
TIMER0_ErrorStatus TIMER0_enuGetCounter(u8 *Copy_pu8Value)
{
    if (Copy_pu8Value == NULL)
        return NULL_POINTER;

    *Copy_pu8Value = TCNT0;
    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuGetCompareA(u8 *Copy_pu8Value)
{
    if (Copy_pu8Value == NULL)
        return NULL_POINTER;

    *Copy_pu8Value = OCR0A;
    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuGetCompareB(u8 *Copy_pu8Value)
{
    if (Copy_pu8Value == NULL)
        return NULL_POINTER;

    *Copy_pu8Value = OCR0B;
    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetDutyCycle(TIMER0_OutputPin Copy_enuPin,
                                          u8 Copy_u8Value)
{

    if (Copy_u8Value > MAX_VALUE)
        return OUT_OF_RANGE;
    if (Copy_enuPin > OC0B || Copy_enuPin < OC0A)
        return INVALID_OUTPUT_PIN;
    if (!GET_BIT(TCCR0A, WGM00))
        return TIMER0_CURRENT_MODE_INCOMPATIBLE;

    u8 top = 0;
    if (GET_BIT(TCCR0B, WGM02))
    {
        //  Adjustable frequency mode
        TIMER0_enuGetCompareA(&top);

        if (Copy_u8Value > top)
            return OUT_OF_RANGE;
        if (Copy_enuPin == OC0A)
            return TIMER0_CURRENT_MODE_INCOMPATIBLE;

        OCR0B = GET_BIT(TCCR0A, COM0B0) ? (1 - (Copy_u8Value / 100)) * top : (Copy_u8Value / 100) * top;
    }
    else
    {
        // fixed frequency mode
        top = MAX_VALUE;
        if (Copy_enuPin == OC0A)
            OCR0A = GET_BIT(TCCR0A, COM0A0) ? (1 - (Copy_u8Value / 100)) * top : (Copy_u8Value / 100) * top;
        else
            OCR0B = GET_BIT(TCCR0A, COM0B0) ? (1 - (Copy_u8Value / 100)) * top : (Copy_u8Value / 100) * top;
    }

    return TIMER0_OK;
}