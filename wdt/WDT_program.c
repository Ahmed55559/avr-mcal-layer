// ========================================= WDT_program.c =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: WDT
// Layer: MCAL
// Description: This file contains the implementation of WDT functions
// =================================================================================================

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "WDT_interface.h"
#include "WDT_config.h"
#include "WDT_private.h"

static void (*WDT_Callback)(void) = NULL;
static WDT_TimeOut local_enuTimeOut;
static WDT_Mode local_enuTimerMode;

static void WDT_voidApplyConfiguration(void)
{
    u8 Local_u8NewConfig = 0;

    switch (local_enuTimerMode)
    {
    case WDT_STOPPED:
        break;

    case WDT_INTERRUPT:
        Local_u8NewConfig |= (1 << WDIE);
        break;

    case WDT_SYS_RESET:
        Local_u8NewConfig |= (1 << WDE);
        break;

    case WDT_INTERRUPT_RESET:
        Local_u8NewConfig |= (1 << WDE);
        Local_u8NewConfig |= (1 << WDIE);
        break;
    }

    switch (local_enuTimeOut)
    {
    case WDT_16MS:
        break;

    case WDT_32MS:
        Local_u8NewConfig |= (1 << WDP0);
        break;

    case WDT_64MS:
        Local_u8NewConfig |= (1 << WDP1);
        break;

    case WDT_125MS:
        Local_u8NewConfig |= (1 << WDP1) |
                             (1 << WDP0);
        break;

    case WDT_250MS:
        Local_u8NewConfig |= (1 << WDP2);
        break;

    case WDT_500MS:
        Local_u8NewConfig |= (1 << WDP2) |
                             (1 << WDP0);
        break;

    case WDT_1S:
        Local_u8NewConfig |= (1 << WDP2) |
                             (1 << WDP1);
        break;

    case WDT_2S:
        Local_u8NewConfig |= (1 << WDP2) |
                             (1 << WDP1) |
                             (1 << WDP0);
        break;

    case WDT_4S:
        Local_u8NewConfig |= (1 << WDP3);
        break;

    case WDT_8S:
        Local_u8NewConfig |= (1 << WDP3) |
                             (1 << WDP0);
        break;
    }

    u8 Local_u8SREG = SREG;

    /* Disable global interrupts */
    CLR_BIT(SREG, GIE);

    /* Timed sequence */
    WDTCSR = (1 << WDCE) | (1 << WDE);

    WDTCSR = Local_u8NewConfig;

    /* Restore previous interrupt state */
    SREG = Local_u8SREG;
}

void WDT_voidInit(void)
{
    local_enuTimeOut = INITIAL_WDT_TIMEOUT;
    local_enuTimerMode = INITIAL_WDT_MODE;
    WDT_voidApplyConfiguration();
}

WDT_ErrorStatus WDT_enuSetTimeOut(WDT_TimeOut Copy_enuTimeOut)
{
    if (Copy_enuTimeOut > WDT_8S || Copy_enuTimeOut < WDT_16MS)
        return WDT_INVALID_TIMEOUT;

    local_enuTimeOut = Copy_enuTimeOut;
    WDT_voidApplyConfiguration();

    return WDT_OK;
}

WDT_ErrorStatus WDT_enuSetTimerMode(WDT_Mode Copy_enuMode)
{
    if (Copy_enuMode > WDT_INTERRUPT_RESET || Copy_enuMode < WDT_STOPPED)
        return WDT_INVALID_TIMER_MODE;
    if ((Copy_enuMode == WDT_INTERRUPT || Copy_enuMode == WDT_INTERRUPT_RESET) && !GET_BIT(SREG, GIE))
        return WDT_GIE_DISABLED;

    local_enuTimerMode = Copy_enuMode;
    WDT_voidApplyConfiguration();

    return WDT_OK;
}

WDT_ErrorStatus WDT_enuSetCallback(void (*Copy_pfCallback)(void))
{
    if (Copy_pfCallback == NULL)
        return WDT_NULL_POINTER;
    if (local_enuTimerMode != WDT_INTERRUPT && local_enuTimerMode != WDT_INTERRUPT_RESET)
        return WDT_INVALID_TIMER_MODE;

    WDT_Callback = Copy_pfCallback;

    return WDT_OK;
}
void __vector_6(void) __attribute__((signal));
void __vector_6(void)
{
    if (WDT_Callback != NULL)
    {
        WDT_Callback();
    }
}

void WDT_voidRefresh(void)
{
    __asm__ __volatile__("wdr");
}