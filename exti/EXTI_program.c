// ========================================= EXTI_program.c =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: EXTI
// Layer: MCAL
// Description: This file contains the implementation of EXTI functions
// =================================================================================================

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "EXTI_interface.h"
#include "EXTI_config.h"
#include "EXTI_private.h"

static void (*EXTI_Callback[2])(void) = {NULL};
static void (*PC_Callback[3])(void) = {NULL};

void GIE_voidEnable(void)
{
    SET_BIT(SREG, 7);
}
void GIE_voidDisable(void)
{
    CLR_BIT(SREG, 7);
}
void EXTI_voidInit(void)
{

#if (EXTI0_INITIAL_STATE == ENABLE)
    SET_BIT(EIMSK, 0);
#elif (EXTI0_INITIAL_STATE == DISABLE)
    CLR_BIT(EIMSK, 0);
#endif

#if (EXTI1_INITIAL_STATE == ENABLE)
    SET_BIT(EIMSK, 1);
#elif (EXTI1_INITIAL_STATE == DISABLE)
    CLR_BIT(EIMSK, 1);
#endif

#if (EXTI0_DEFAULT_SENSE == LOW_LEVEL)
    CLR_BIT(EICRA, 0);
    CLR_BIT(EICRA, 1);
#elif (EXTI0_DEFAULT_SENSE == ANY_CHANGE)
    SET_BIT(EICRA, 0);
    CLR_BIT(EICRA, 1);
#elif (EXTI0_DEFAULT_SENSE == FALLING_EDGE)
    CLR_BIT(EICRA, 0);
    SET_BIT(EICRA, 1);
#elif (EXTI0_DEFAULT_SENSE == RISING_EDGE)
    SET_BIT(EICRA, 0);
    SET_BIT(EICRA, 1);
#endif

#if (EXTI1_DEFAULT_SENSE == LOW_LEVEL)
    CLR_BIT(EICRA, 2);
    CLR_BIT(EICRA, 3);
#elif (EXTI1_DEFAULT_SENSE == ANY_CHANGE)
    SET_BIT(EICRA, 2);
    CLR_BIT(EICRA, 3);
#elif (EXTI1_DEFAULT_SENSE == FALLING_EDGE)
    CLR_BIT(EICRA, 2);
    SET_BIT(EICRA, 3);
#elif (EXTI1_DEFAULT_SENSE == RISING_EDGE)
    SET_BIT(EICRA, 2);
    SET_BIT(EICRA, 3);
#endif
}

// this implementation depends on the fact that lines are line zero req bit zero and one req bit one
void EXTI_voidEnable(EXTI_line line)
{
    SET_BIT(EIMSK, line);
}
void EXTI_voidDisable(EXTI_line line)
{
    CLR_BIT(EIMSK, line);
}
void EXTI_voidSetCallback(EXTI_line line, void (*isr)(void))
{
    if (isr != NULL)
    {
        EXTI_Callback[line] = isr;
    }
}
void EXTI_voidChangeSensingMode(EXTI_line line, EXTI_sensing mode)
{
    switch (line)
    {
    case EXTI_line_0:

        switch (mode)
        {
        case LOW_LEVEL:
            CLR_BIT(EICRA, 0);
            CLR_BIT(EICRA, 1);
            break;
        case ANY_CHANGE:
            SET_BIT(EICRA, 0);
            CLR_BIT(EICRA, 1);
            break;
        case FALLING_EDGE:
            CLR_BIT(EICRA, 0);
            SET_BIT(EICRA, 1);
            break;
        case RISING_EDGE:
            SET_BIT(EICRA, 0);
            SET_BIT(EICRA, 1);
            break;
        }

        break;
    case EXTI_line_1:
        switch (mode)
        {
        case LOW_LEVEL:
            CLR_BIT(EICRA, 2);
            CLR_BIT(EICRA, 3);
            break;
        case ANY_CHANGE:
            SET_BIT(EICRA, 2);
            CLR_BIT(EICRA, 3);
            break;
        case FALLING_EDGE:
            CLR_BIT(EICRA, 2);
            SET_BIT(EICRA, 3);
            break;
        case RISING_EDGE:
            SET_BIT(EICRA, 2);
            SET_BIT(EICRA, 3);
            break;
        }
        break;
    }
}

void EXTI_voidClearFlag(EXTI_line line)
{
    SET_BIT(EIFR, line);
}

/* Pin change interrupts */
void PC_voidInit()
{

#if (PCINT0_INITIAL_STATE == ENABLE)
    SET_BIT(PCICR, 0);
#elif (PCINT0_INITIAL_STATE == DISABLE)
    CLR_BIT(PCICR, 0);
#endif

#if (PCINT1_INITIAL_STATE == ENABLE)
    SET_BIT(PCICR, 1);
#elif (PCINT1_INITIAL_STATE == DISABLE)
    CLR_BIT(PCICR, 1);
#endif

#if (PCINT2_INITIAL_STATE == ENABLE)
    SET_BIT(PCICR, 2);
#elif (PCINT2_INITIAL_STATE == DISABLE)
    CLR_BIT(PCICR, 2);
#endif
}

void PC_voidEnableInterruptGroup(PC_line line)
{
    SET_BIT(PCICR, line);
}
void PC_voidDisableInterruptGroup(PC_line line)
{
    CLR_BIT(PCICR, line);
}
void PC_voidEnablePin(PC_line line, PC_pin pin)
{
    switch (line)
    {
    case PC_line0:
        SET_BIT(PCMSK0, pin);
        break;
    case PC_line1:
        SET_BIT(PCMSK1, pin);
        break;
    case PC_line2:
        SET_BIT(PCMSK2, pin);
        break;
    }
}
void PC_voidSetCallback(PC_line line, void (*isr)(void))
{
    if (isr != NULL)
    {
        PC_Callback[line] = isr;
    }
}
void PC_voidClearFlag(PC_line line)
{
    SET_BIT(PCIFR, line);
}

void __vector_1(void) __attribute__((signal));
void __vector_1(void)
{
    if (EXTI_Callback[0] != NULL)
    {
        EXTI_Callback[0]();
    }
}

void __vector_2(void) __attribute__((signal));
void __vector_2(void)
{
    if (EXTI_Callback[1] != NULL)
    {
        EXTI_Callback[1]();
    }
}

void __vector_3(void) __attribute__((signal));
void __vector_3(void)
{
    if (PC_Callback[0] != NULL)
    {
        PC_Callback[0]();
    }
}

void __vector_4(void) __attribute__((signal));
void __vector_4(void)
{
    if (PC_Callback[1] != NULL)
    {
        PC_Callback[1]();
    }
}

void __vector_5(void) __attribute__((signal));
void __vector_5(void)
{
    if (PC_Callback[2] != NULL)
    {
        PC_Callback[2]();
    }
}