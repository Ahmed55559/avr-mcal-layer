// ========================================= WDT_interface.h =========================================
// Author: Eng. Ahmed Ashraf
// SWC: WDT
// Layer: MCAL
// Description: This file contains the public interface definitions for the WDT peripheral.
// =====================================================================================================

#ifndef WDT_INTERFACE_H
#define WDT_INTERFACE_H

typedef enum
{
    WDT_STOPPED,
    WDT_INTERRUPT,
    WDT_SYS_RESET,
    WDT_INTERRUPT_RESET
} WDT_Mode;

typedef enum
{
    WDT_16MS,
    WDT_32MS,
    WDT_64MS,
    WDT_125MS,
    WDT_250MS,
    WDT_500MS,
    WDT_1S,
    WDT_2S,
    WDT_4S,
    WDT_8S,
} WDT_TimeOut;

typedef enum
{
    WDT_OK,
    WDT_INVALID_TIMER_MODE,
    WDT_INVALID_TIMEOUT,
    WDT_NULL_POINTER,
    WDT_GIE_DISABLED,
} WDT_ErrorStatus;

void WDT_voidInit(void);

WDT_ErrorStatus WDT_enuSetTimeOut(WDT_TimeOut Copy_enuTimeOut);

WDT_ErrorStatus WDT_enuSetTimerMode(WDT_Mode Copy_enuMode);

WDT_ErrorStatus WDT_enuSetCallback(void (*Copy_pfCallback)(void));

void WDT_voidRefresh(void);
#endif /* WDT_INTERFACE_H */
