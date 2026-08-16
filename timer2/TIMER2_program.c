// ========================================= TIMER2_program.c =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: TIMER0
// Layer: MCAL
// Description: This file contains the implementation of TIMER0 functions
// =================================================================================================

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "TIMER2_interface.h"
#include "TIMER2_config.h"
#include "TIMER2_private.h"
static void (*TIMER2_Callback[3])(void) = {NULL};

void syncAsyncCompitability(void)
{
    if (GET_BIT(ASSR, AS2))
        T2_WAIT();
}

void TIMER2_voidInit(void)
{
    if (INITIAL_TIMER2_TIMING_MODE == SYNC)
        CLR_BIT(ASSR, AS2);
    else
    {
        SET_BIT(ASSR, AS2);
        SET_BIT(ASSR, EXCLK);
        T2_WAIT();
    }
    switch (INITIAL_TIMER2_MODE)
    {
    case NORMAL:
        CLR_BIT(TCCR2A, WGM20);
        CLR_BIT(TCCR2A, WGM21);
        CLR_BIT(TCCR2B, WGM22);
        break;
    case PWM_PC_FIXED:
        SET_BIT(TCCR2A, WGM20);
        CLR_BIT(TCCR2A, WGM21);
        CLR_BIT(TCCR2B, WGM22);
        break;
    case CTC:
        CLR_BIT(TCCR2A, WGM20);
        SET_BIT(TCCR2A, WGM21);
        CLR_BIT(TCCR2B, WGM22);
        break;
    case PWM_FAST_FIXED:
        SET_BIT(TCCR2A, WGM20);
        SET_BIT(TCCR2A, WGM21);
        CLR_BIT(TCCR2B, WGM22);
        break;
    case PWM_PC_ADJ:
        SET_BIT(TCCR2A, WGM20);
        CLR_BIT(TCCR2A, WGM21);
        SET_BIT(TCCR2B, WGM22);
        break;
    case PWM_FAST_ADJ:
        SET_BIT(TCCR2A, WGM20);
        SET_BIT(TCCR2A, WGM21);
        SET_BIT(TCCR2B, WGM22);
        break;
    }

    TCCR2B = (TCCR2B & (~0 << 3)) | INITIAL_TIMER2_CLK;

    TCNT2 = INITIAL_TIMER2_COUNTER_VALUE;
    OCR2A = INITIAL_TIMER2_OC2RA_VALUE;
    OCR2B = INITIAL_TIMER2_OC2RB_VALUE;
    if (INITIAL_TIMER2_TIMING_MODE == ASYNC)
        T2_WAIT();
}

TIMER2_ErrorStatus TIMER2_enuSetClkSource(TIMER2_SysCLKPrescaler Copy_enuSource)
{
    if (!GET_BIT(PRR, PRTIM2))
        return TIMER2_SHUTDOWN;
    if (Copy_enuSource > CLK_DIV_1024 || Copy_enuSource < NO_CLK)
        return INVALID_PRESCALER;
    if (GET_BIT(ASSR, AS2))
        return INVALID_TIMING_MODE;

    TCCR2B = (TCCR2B & (~0 < 3)) | Copy_enuSource;
    return TIMER2_OK;
}

TIMER2_ErrorStatus TIMER2_enuSetTimerMode(TIMER2_Mode Copy_enuMode)
{
    if (!GET_BIT(PRR, PRTIM2))
        return TIMER2_SHUTDOWN;
    if (Copy_enuMode > PWM_FAST_ADJ || Copy_enuMode < NORMAL)
        return INVALID_TIMER_MODE;

    switch (Copy_enuMode)
    {
    case NORMAL:
        CLR_BIT(TCCR2A, WGM20);
        CLR_BIT(TCCR2A, WGM21);
        CLR_BIT(TCCR2B, WGM22);
        break;
    case PWM_PC_FIXED:
        SET_BIT(TCCR2A, WGM20);
        CLR_BIT(TCCR2A, WGM21);
        CLR_BIT(TCCR2B, WGM22);
        break;
    case CTC:
        CLR_BIT(TCCR2A, WGM20);
        SET_BIT(TCCR2A, WGM21);
        CLR_BIT(TCCR2B, WGM22);
        break;
    case PWM_FAST_FIXED:
        SET_BIT(TCCR2A, WGM20);
        SET_BIT(TCCR2A, WGM21);
        CLR_BIT(TCCR2B, WGM22);
        break;
    case PWM_PC_ADJ:
        SET_BIT(TCCR2A, WGM20);
        CLR_BIT(TCCR2A, WGM21);
        SET_BIT(TCCR2B, WGM22);
        break;
    case PWM_FAST_ADJ:
        SET_BIT(TCCR2A, WGM20);
        SET_BIT(TCCR2A, WGM21);
        SET_BIT(TCCR2B, WGM22);
        break;
    }

    syncAsyncCompitability();

    return TIMER2_OK;
}

TIMER2_ErrorStatus TIMER2_enuSetOCOutputMode(TIMER2_OCMode Copy_enuMode, TIMER2_OutputPin Copy_enuPin)
{
    if (!GET_BIT(PRR, PRTIM2))
        return TIMER2_SHUTDOWN;
    if (Copy_enuMode > OC_SET || Copy_enuMode < OC_DISCONNECTED)
        return INVALID_OUTPUT_MODE;
    if (Copy_enuPin > OC2B || Copy_enuPin < OC2A)
        return INVALID_OUTPUT_PIN;

    switch (Copy_enuPin)
    {
    case OC2B:
        TCCR2B &= ~(0x03 << 4);
        syncAsyncCompitability();
        TCCR2B |= (Copy_enuMode << 4);
        syncAsyncCompitability();

        break;
    case OC2A:
        TCCR2A &= ~(0x03 << 6);
        syncAsyncCompitability();

        TCCR2A |= (Copy_enuMode << 6);
        syncAsyncCompitability();

        break;
    }
    return TIMER2_OK;
}

TIMER2_ErrorStatus TIMER2_enuSetPWMOutputMode(TIMER2_PWMMode Copy_enuMode,
                                              TIMER2_OutputPin Copy_enuPin)
{
    if (!GET_BIT(PRR, PRTIM2))
        return TIMER2_SHUTDOWN;
    if (Copy_enuMode != PWM_INVERTING && Copy_enuMode != PWM_NON_INVERTING)
        return INVALID_OUTPUT_MODE;
    if (Copy_enuPin > OC2B || Copy_enuPin < OC2A)
        return INVALID_OUTPUT_PIN;
    if (!GET_BIT(TCCR2A, WGM20))
        return TIMER2_CURRENT_MODE_INCOMPATIBLE;

    switch (Copy_enuPin)
    {
    case OC2B:
        TCCR2B &= ~(0x03 << 4);
        syncAsyncCompitability();

        TCCR2B |= (Copy_enuMode << 4);
        syncAsyncCompitability();

        break;
    case OC2A:
        TCCR2A &= ~(0x03 << 6);
        syncAsyncCompitability();

        TCCR2A |= (Copy_enuMode << 6);
        syncAsyncCompitability();

        break;
    }
    return TIMER2_OK;
}

TIMER2_ErrorStatus TIMER2_enuEnableInterrupt(TIMER2_Interrupt Copy_enuInterrupt)
{
    if (!GET_BIT(PRR, PRTIM2))
        return TIMER2_SHUTDOWN;
    if (Copy_enuInterrupt > COMPARE_MATCHB || Copy_enuInterrupt < OVERFLOW)
        return INVALID_INTERRUPT;
    if (!GET_BIT(SREG, GIE))
        return GIE_DISABLED;

    SET_BIT(TIMSK2, Copy_enuInterrupt);
    return TIMER2_OK;
}

TIMER2_ErrorStatus TIMER2_enuDisableInterrupt(TIMER2_Interrupt Copy_enuInterrupt)
{
    if (!GET_BIT(PRR, PRTIM2))
        return TIMER2_SHUTDOWN;
    if (Copy_enuInterrupt > COMPARE_MATCHB || Copy_enuInterrupt < OVERFLOW)
        return INVALID_INTERRUPT;
    if (!GET_BIT(SREG, GIE))
        return GIE_DISABLED;

    CLR_BIT(TIMSK2, Copy_enuInterrupt);
    return TIMER2_OK;
}

TIMER2_ErrorStatus TIMER2_enuSetCallback(TIMER2_Interrupt Copy_enuInterrupt,
                                         void (*isr)(void))
{
    if (isr != NULL)
    {
        TIMER2_Callback[Copy_enuInterrupt] = isr;
    }
}
void __vector_10(void) __attribute__((signal));
void __vector_10(void)
{
    if (TIMER2_Callback[0] != NULL)
    {
        TIMER2_Callback[0]();
    }
}
void __vector_8(void) __attribute__((signal));
void __vector_8(void)
{
    if (TIMER2_Callback[1] != NULL)
    {
        TIMER2_Callback[1]();
    }
}
void __vector_9(void) __attribute__((signal));
void __vector_9(void)
{
    if (TIMER2_Callback[2] != NULL)
    {
        TIMER2_Callback[2]();
    }
}

TIMER2_ErrorStatus TIMER2_enuSetCounter(u8 Copy_u8Value)
{
    if (Copy_u8Value > MAX_VALUE || Copy_u8Value < MIN_VALUE)
        return OUT_OF_RANGE;

    TCNT2 = Copy_u8Value;
    syncAsyncCompitability();
    return TIMER2_OK;
}

TIMER2_ErrorStatus TIMER2_enuSetCompareA(u8 Copy_u8Value)
{
    if (Copy_u8Value > MAX_VALUE || Copy_u8Value < MIN_VALUE)
        return OUT_OF_RANGE;

    OCR2A = Copy_u8Value;
    syncAsyncCompitability();

    return TIMER2_OK;
}

TIMER2_ErrorStatus TIMER2_enuSetCompareB(u8 Copy_u8Value)
{
    if (Copy_u8Value > MAX_VALUE || Copy_u8Value < MIN_VALUE)
        return OUT_OF_RANGE;

    OCR2B = Copy_u8Value;
    syncAsyncCompitability();

    return TIMER2_OK;
}
TIMER2_ErrorStatus TIMER2_enuGetCounter(u8 *Copy_pu8Value)
{
    if (Copy_pu8Value == NULL)
        return NULL_POINTER;

    *Copy_pu8Value = TCNT2;
    syncAsyncCompitability();

    return TIMER2_OK;
}

TIMER2_ErrorStatus TIMER2_enuGetCompareA(u8 *Copy_pu8Value)
{
    if (Copy_pu8Value == NULL)
        return NULL_POINTER;

    *Copy_pu8Value = OCR2A;
    syncAsyncCompitability();

    return TIMER2_OK;
}

TIMER2_ErrorStatus TIMER2_enuGetCompareB(u8 *Copy_pu8Value)
{
    if (Copy_pu8Value == NULL)
        return NULL_POINTER;

    *Copy_pu8Value = OCR2B;
    syncAsyncCompitability();

    return TIMER2_OK;
}

TIMER2_ErrorStatus TIMER2_enuSetDutyCycle(TIMER2_OutputPin Copy_enuPin, u8 Copy_u8Value)
{
    if (!GET_BIT(PRR, PRTIM2))
        return TIMER2_SHUTDOWN;
    if (Copy_u8Value > MAX_VALUE || Copy_u8Value < MIN_VALUE)
        return OUT_OF_RANGE;
    if (Copy_enuPin > OC2B || Copy_enuPin < OC2A)
        return INVALID_OUTPUT_PIN;
    if (!GET_BIT(TCCR2A, WGM20))
        return TIMER2_CURRENT_MODE_INCOMPATIBLE;

    u8 top = 0;
    if (GET_BIT(TCCR2B, WGM22))
    {
        //  Adjustable frequency mode
        TIMER2_enuGetCompareA(&top);

        if (Copy_u8Value > top)
            return OUT_OF_RANGE;
        if (Copy_enuPin == OC2A)
            return TIMER2_CURRENT_MODE_INCOMPATIBLE;
        OCR2B = GET_BIT(TCCR2A, COM2B0) ? (1 - (Copy_u8Value / 100)) * top : (Copy_u8Value / 100) * top;
        syncAsyncCompitability();
    }
    else
    {
        // fixed frequency mode
        top = MAX_VALUE;
        if (Copy_enuPin == OC2A)
        {
            OCR2A = GET_BIT(TCCR2A, COM2A0) ? (1 - (Copy_u8Value / 100)) * top : (Copy_u8Value / 100) * top;
            syncAsyncCompitability();
        }

        else
        {
            OCR2B = GET_BIT(TCCR2A, COM2B0) ? (1 - (Copy_u8Value / 100)) * top : (Copy_u8Value / 100) * top;
            syncAsyncCompitability();
        }
    }

    return TIMER2_OK;
}

TIMER2_ErrorStatus TIMER2_enuSetTimerTimingMode(TIMER2_TimingMode Copy_enuMode)
{
    u8 lastCounterValue, lastComAvalue, lastComBvalue;
    TIMER2_enuGetCounter(&lastCounterValue);
    TIMER2_enuGetCompareA(&lastComAvalue);
    TIMER2_enuGetCompareB(&lastComBvalue);

    if (Copy_enuMode != ASYNC && Copy_enuMode != SYNC)
        return INVALID_TIMING_MODE;

    switch (Copy_enuMode)
    {
    case ASYNC:
        TIMSK2 = 0;
        SET_BIT(ASSR, EXCLK);
        SET_BIT(ASSR, AS2);
        TCNT2 = lastCounterValue;
        OCR2A = lastComAvalue;
        OCR2B = lastComBvalue;
        T2_WAIT();
        TIFR2 = 0;
        TIMSK2 = 1;
        break;
    case SYNC:
        TIMSK2 = 0;
        CLR_BIT(ASSR, EXCLK);
        CLR_BIT(ASSR, AS2);
        TCNT2 = lastCounterValue;
        OCR2A = lastComAvalue;
        OCR2B = lastComBvalue;
        TIFR2 = 0;
        TIMSK2 = 1;
        break;
    }
    return TIMER2_OK;
}