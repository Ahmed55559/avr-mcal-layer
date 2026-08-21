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
        TCCR0A &= ~(0x03 << 4);
        TCCR0A |= (Copy_enuMode << 4);
        break;
    case OC0A:
        TCCR0A &= ~(0x03 << 6);
        TCCR0A |= (Copy_enuMode << 6);
        break;
    }
    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetPWMOutputMode(
    TIMER0_PWMMode Copy_enuMode,
    TIMER0_OutputPin Copy_enuPin)
{
    if (Copy_enuMode != PWM_INVERTING &&
        Copy_enuMode != PWM_NON_INVERTING)
    {
        return INVALID_OUTPUT_MODE;
    }

    if (Copy_enuPin > OC0B || Copy_enuPin < OC0A)
    {
        return INVALID_OUTPUT_PIN;
    }

    if (!GET_BIT(TCCR0A, WGM00))
    {
        return TIMER0_CURRENT_MODE_INCOMPATIBLE;
    }

    u8 Local_u8HardwareMode;

    if (Copy_enuMode == PWM_NON_INVERTING)
    {
        Local_u8HardwareMode = 2;
    }
    else
    {
        Local_u8HardwareMode = 3;
    }

    switch (Copy_enuPin)
    {
    case OC0A:

        TCCR0A &= ~(0x03 << COM0A0);
        TCCR0A |= (Local_u8HardwareMode << COM0A0);

        break;

    case OC0B:

        TCCR0A &= ~(0x03 << COM0B0);
        TCCR0A |= (Local_u8HardwareMode << COM0B0);

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
void __vector_16(void) __attribute__((signal));
void __vector_16(void)
{
    if (TIMER0_Callback[0] != NULL)
    {
        TIMER0_Callback[0]();
    }
}
void __vector_14(void) __attribute__((signal));
void __vector_14(void)
{
    if (TIMER0_Callback[1] != NULL)
    {
        TIMER0_Callback[1]();
    }
}
void __vector_15(void) __attribute__((signal));
void __vector_15(void)
{
    if (TIMER0_Callback[2] != NULL)
    {
        TIMER0_Callback[2]();
    }
}

TIMER0_ErrorStatus TIMER0_enuSetCounter(u8 Copy_u8Value)
{
    TCNT0 = Copy_u8Value;
    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetCompareA(u8 Copy_u8Value)
{

    OCR0A = Copy_u8Value;
    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetCompareB(u8 Copy_u8Value)
{

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

TIMER0_ErrorStatus TIMER0_enuSetDutyCycle(
    TIMER0_OutputPin Copy_enuPin,
    u8 Copy_u8Value)
{

    if (Copy_u8Value > 100)
        return OUT_OF_RANGE;

    if (Copy_enuPin > OC0B)
        return INVALID_OUTPUT_PIN;

    if (!GET_BIT(TCCR0A, WGM00))
        return TIMER0_CURRENT_MODE_INCOMPATIBLE;

    u8 top;
    u8 compareValue;
    u8 com0x0;

    if (GET_BIT(TCCR0B, WGM02))
    {
        /* Variable TOP modes */
        if (Copy_enuPin == OC0A)
            return TIMER0_CURRENT_MODE_INCOMPATIBLE;

        top = OCR0A;

        com0x0 = GET_BIT(TCCR0A, COM0B0);

        if (com0x0)
        {
            compareValue =
                top - (((u16)Copy_u8Value * top) / 100);
        }
        else
        {
            compareValue =
                ((u16)Copy_u8Value * top) / 100;
        }

        OCR0B = compareValue;
    }
    else
    {
        /* Fixed TOP = 255 */
        top = 255;

        if (Copy_enuPin == OC0A)
        {
            com0x0 = GET_BIT(TCCR0A, COM0A0);

            if (com0x0)
            {
                compareValue =
                    top - (((u16)Copy_u8Value * top) / 100);
            }
            else
            {
                compareValue =
                    ((u16)Copy_u8Value * top) / 100;
            }

            OCR0A = compareValue;
        }
        else
        {
            com0x0 = GET_BIT(TCCR0A, COM0B0);

            if (com0x0)
            {
                compareValue =
                    top - (((u16)Copy_u8Value * top) / 100);
            }
            else
            {
                compareValue =
                    ((u16)Copy_u8Value * top) / 100;
            }

            OCR0B = compareValue;
        }
    }

    return TIMER0_OK;
}