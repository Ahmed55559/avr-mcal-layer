// ========================================= TIMER0_program.c =========================================
// Author: Eng. Ahmed Ashraf
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

static u8 TIMER0_u8GetCurrentMode(void)
{
    u8 Local_u8Mode = 0;

    Local_u8Mode |= GET_BIT(TCCR0A, WGM00);
    Local_u8Mode |= (GET_BIT(TCCR0A, WGM01) << 1);
    Local_u8Mode |= (GET_BIT(TCCR0B, WGM02) << 2);

    return Local_u8Mode;
}

static u8 TIMER0_u8IsPWMMode(void)
{
    u8 Local_u8Mode = TIMER0_u8GetCurrentMode();

    if ((Local_u8Mode == 1) ||
        (Local_u8Mode == 3) ||
        (Local_u8Mode == 5) ||
        (Local_u8Mode == 7))
    {
        return 1;
    }

    return 0;
}

static u8 TIMER0_u8IsVariableTopMode(void)
{
    u8 Local_u8Mode = TIMER0_u8GetCurrentMode();

    if ((Local_u8Mode == 5) ||
        (Local_u8Mode == 7))
    {
        return 1;
    }

    return 0;
}

static u8 TIMER0_u8GetCOMMode(TIMER0_OutputPin Copy_enuPin)
{
    u8 Local_u8COMMode;

    if (Copy_enuPin == OC0A)
    {
        Local_u8COMMode =
            GET_BIT(TCCR0A, COM0A0);

        Local_u8COMMode |=
            (GET_BIT(TCCR0A, COM0A1) << 1);
    }
    else
    {
        Local_u8COMMode =
            GET_BIT(TCCR0A, COM0B0);

        Local_u8COMMode |=
            (GET_BIT(TCCR0A, COM0B1) << 1);
    }

    return Local_u8COMMode;
}

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

    TCCR0B =
        (TCCR0B & ~(0x07)) | INITIAL_TIMER0_CLK;

    TCNT0 = INITIAL_TIMER0_COUNTER_VALUE;

    OCR0A = INITIAL_TIMER0_OC0RA_VALUE;

    OCR0B = INITIAL_TIMER0_OC0RB_VALUE;
}

TIMER0_ErrorStatus TIMER0_enuSetClkSource(
    TIMER0_ClkSource Copy_enuSource)
{
    if (Copy_enuSource > EXT_CLK_RISING ||
        Copy_enuSource < NO_CLK)
    {
        return INVALID_PRESCALER;
    }

    TCCR0B =
        (TCCR0B & ~(0x07)) | Copy_enuSource;

    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetTimerMode(
    TIMER0_Mode Copy_enuMode)
{
    if (Copy_enuMode > TIMER0_PWM_FAST_ADJ ||
        Copy_enuMode < TIMER0_NORMAL)
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

TIMER0_ErrorStatus TIMER0_enuSetOCOutputMode(
    TIMER0_OCMode Copy_enuMode,
    TIMER0_OutputPin Copy_enuPin)
{
    if (Copy_enuMode > OC_SET ||
        Copy_enuMode < OC_DISCONNECTED)
        return INVALID_OUTPUT_MODE;

    if (Copy_enuPin > OC0B ||
        Copy_enuPin < OC0A)
        return INVALID_OUTPUT_PIN;

    if (TIMER0_u8IsPWMMode())
        return TIMER0_CURRENT_MODE_INCOMPATIBLE;

    if (Copy_enuPin == OC0A)
    {
        TCCR0A &= ~(0x03 << COM0A0);

        TCCR0A |=
            ((u8)Copy_enuMode << COM0A0);
    }
    else
    {
        TCCR0A &= ~(0x03 << COM0B0);

        TCCR0A |=
            ((u8)Copy_enuMode << COM0B0);
    }

    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetPWMOutputMode(
    TIMER0_PWMMode Copy_enuMode,
    TIMER0_OutputPin Copy_enuPin)
{
    if (Copy_enuMode > PWM_INVERTING ||
        Copy_enuMode < PWM_NON_INVERTING)
    {
        return INVALID_OUTPUT_MODE;
    }

    if (Copy_enuPin > OC0B ||
        Copy_enuPin < OC0A)
    {
        return INVALID_OUTPUT_PIN;
    }

    if (!TIMER0_u8IsPWMMode())
    {
        return TIMER0_CURRENT_MODE_INCOMPATIBLE;
    }

    u8 Local_u8COMValue =
        (Copy_enuMode == PWM_NON_INVERTING)
            ? 2
            : 3;

    if (Copy_enuPin == OC0A)
    {
        TCCR0A &= ~(0x03 << COM0A0);

        TCCR0A |=
            (Local_u8COMValue << COM0A0);
    }
    else
    {
        TCCR0A &= ~(0x03 << COM0B0);

        TCCR0A |=
            (Local_u8COMValue << COM0B0);
    }

    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuEnableInterrupt(
    TIMER0_Interrupt Copy_enuInterrupt)
{
    if (Copy_enuInterrupt > COMPARE_MATCHB ||
        Copy_enuInterrupt < OVERFLOW)
    {
        return INVALID_INTERRUPT;
    }

    SET_BIT(TIMSK0, Copy_enuInterrupt);

    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuDisableInterrupt(
    TIMER0_Interrupt Copy_enuInterrupt)
{
    if (Copy_enuInterrupt > COMPARE_MATCHB ||
        Copy_enuInterrupt < OVERFLOW)
    {
        return INVALID_INTERRUPT;
    }

    CLR_BIT(TIMSK0, Copy_enuInterrupt);

    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetCallback(
    TIMER0_Interrupt Copy_enuInterrupt,
    void (*isr)(void))
{
    if (Copy_enuInterrupt < OVERFLOW ||
        Copy_enuInterrupt > COMPARE_MATCHB)
    {
        return INVALID_INTERRUPT;
    }

    if (isr == NULL)
    {
        return NULL_POINTER;
    }

    TIMER0_Callback[Copy_enuInterrupt] = isr;

    return TIMER0_OK;
}

/* TIMER0_OVF */
void __vector_16(void) __attribute__((signal));

void __vector_16(void)
{
    if (TIMER0_Callback[OVERFLOW] != NULL)
    {
        TIMER0_Callback[OVERFLOW]();
    }
}

/* TIMER0_COMPA */
void __vector_14(void) __attribute__((signal));

void __vector_14(void)
{
    if (TIMER0_Callback[COMPARE_MATCHA] != NULL)
    {
        TIMER0_Callback[COMPARE_MATCHA]();
    }
}

/* TIMER0_COMPB */
void __vector_15(void) __attribute__((signal));

void __vector_15(void)
{
    if (TIMER0_Callback[COMPARE_MATCHB] != NULL)
    {
        TIMER0_Callback[COMPARE_MATCHB]();
    }
}

TIMER0_ErrorStatus TIMER0_enuSetCounter(
    u8 Copy_u8Value)
{
    TCNT0 = Copy_u8Value;

    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuGetCounter(
    u8 *Copy_pu8Value)
{
    if (Copy_pu8Value == NULL)
    {
        return NULL_POINTER;
    }

    *Copy_pu8Value = TCNT0;

    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetCompareA(
    u8 Copy_u8Value)
{
    OCR0A = Copy_u8Value;

    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetCompareB(
    u8 Copy_u8Value)
{
    OCR0B = Copy_u8Value;

    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuGetCompareA(
    u8 *Copy_pu8Value)
{
    if (Copy_pu8Value == NULL)
    {
        return NULL_POINTER;
    }

    *Copy_pu8Value = OCR0A;

    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuGetCompareB(
    u8 *Copy_pu8Value)
{
    if (Copy_pu8Value == NULL)
    {
        return NULL_POINTER;
    }

    *Copy_pu8Value = OCR0B;

    return TIMER0_OK;
}

TIMER0_ErrorStatus TIMER0_enuSetDutyCycle(
    TIMER0_OutputPin Copy_enuPin,
    u8 Copy_u8Value)
{
    u8 Local_u8Top;
    u8 Local_u8COMMode;
    u8 Local_u8CompareValue;

    if (Copy_u8Value > 100)
        return OUT_OF_RANGE;

    if (Copy_enuPin > OC0B ||
        Copy_enuPin < OC0A)
        return INVALID_OUTPUT_PIN;

    if (!TIMER0_u8IsPWMMode())
        return TIMER0_CURRENT_MODE_INCOMPATIBLE;

    if (TIMER0_u8IsVariableTopMode())
    {
        if (Copy_enuPin == OC0A)
            return TIMER0_CURRENT_MODE_INCOMPATIBLE;

        Local_u8Top = OCR0A;
    }
    else
    {
        Local_u8Top = 255;
    }

    Local_u8COMMode =
        TIMER0_u8GetCOMMode(Copy_enuPin);

    if (Local_u8COMMode == 2)
    {
        Local_u8CompareValue =
            ((u16)Copy_u8Value *
             Local_u8Top) /
            100;
    }
    else if (Local_u8COMMode == 3)
    {
        Local_u8CompareValue =
            Local_u8Top -
            (((u16)Copy_u8Value *
              Local_u8Top) /
             100);
    }
    else
    {
        return TIMER0_CURRENT_MODE_INCOMPATIBLE;
    }

    if (Copy_enuPin == OC0A)
    {
        OCR0A = Local_u8CompareValue;
    }
    else
    {
        OCR0B = Local_u8CompareValue;
    }

    return TIMER0_OK;
}