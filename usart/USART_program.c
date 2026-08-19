// ========================================= USART_program.c =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: USART
// Layer: MCAL
// Description: This file contains the implementation of USART functions
// =================================================================================================

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "USART_interface.h"
#include "USART_config.h"
#include "USART_private.h"

static USART_Mode local_enuMode;
static USART_DataBitsMode local_enuDataBitsMode;

static void (*USART_Callback[3])(void) = {NULL};

static u8 local_u8DataTransmit = 0;
static void (*local_pfNotificationTransmitCallback)(void) = NULL;
static u16 *local_u16DataReceive = 0;
static void (*local_pfNotificationReceiveCallback)(void) = NULL;

static u16 calcBaudRateReg(u64 Copy_u64BaudRate)
{
    u16 Local_u16NewUBRR0 = 0;
    switch (local_enuMode)
    {
    case USART_ASYNC_NORMAL_SPEED:
        Local_u16NewUBRR0 = (SYSTEM_CLK_FREQUENCY / (16 * Copy_u64BaudRate)) - 1;
        break;
    case USART_ASYNC_X2_SPEED:
        Local_u16NewUBRR0 = (SYSTEM_CLK_FREQUENCY / (8 * Copy_u64BaudRate)) - 1;
        break;
    case USART_SYNC_MASTER:
        Local_u16NewUBRR0 = (SYSTEM_CLK_FREQUENCY / (2 * Copy_u64BaudRate)) - 1;
        break;
    }
    return Local_u16NewUBRR0;
}

static void applyUSARTMode(USART_Mode Copy_enuMode)
{
    switch (Copy_enuMode)
    {
    case USART_ASYNC_NORMAL_SPEED:
        UCSR0C &= 0x3F;
        CLR_BIT(UCSR0A, U2X0);
        break;
    case USART_ASYNC_X2_SPEED:
        UCSR0C &= 0x3F;
        SET_BIT(UCSR0A, U2X0);
        break;
    case USART_SYNC_MASTER:
        UCSR0C &= 0x3F;
        CLR_BIT(UCSR0A, U2X0);
        SET_BIT(UCSR0C, UMSEL00);
        break;
    }
}

void USART_voidInit(void)
{
    local_enuMode = INITIAL_USART_MODE;
    local_enuDataBitsMode = INITIAL_USART_DATA_BITS;
    applyUSARTMode(local_enuMode);

    // Baud Rate
    u16 local_u16BuadRate = calcBaudRateReg(INITIAL_USART_BAUDRATE);
    UBRR0L = (local_u16BuadRate & 0x00FF);
    UBRR0H = (local_u16BuadRate & 0x0F00) >> 8;

    // TX,RX Enable
    UCSR0B = (UCSR0B & 0xE7) | (INITIAL_USART_RX_ENABLE << RXEN0) | (INITIAL_USART_TX_ENABLE << TXEN0);

    // Frame Format
    UCSR0C = (UCSR0C & 0xC0) |
             (INITIAL_USART_DATA_BITS << UCSZ00) |
             (INITIAL_USART_PARITY_MODE << UPM00) |
             (INITIAL_USART_STOP_BITS << USBS0) |
             (INITIAL_USART_CLK_POL << UCPOL0);
    if (local_enuDataBitsMode == USART_DATA_BITS_9)
        SET_BIT(UCSR0B, UCSZ02);
}

void USART_enuSetBaudRate(u32 Copy_u32BaudRate)
{
    u16 local_u16BuadRate = calcBaudRateReg(Copy_u32BaudRate);
    UBRR0L = (local_u16BuadRate & 0x00FF);
    UBRR0H = (local_u16BuadRate & 0x0F00) >> 8;
}

USART_ErrorStatus USART_enuSetMode(USART_Mode Copy_enuMode)
{
    if (Copy_enuMode > USART_SYNC_MASTER || Copy_enuMode < USART_ASYNC_NORMAL_SPEED)
        return USART_INVALID_MODE;

    local_enuMode = Copy_enuMode;
    applyUSARTMode(local_enuMode);
    return USART_OK;
}

USART_ErrorStatus USART_enuSetParity(USART_ParityMode Copy_enuMode)
{
    if (Copy_enuMode > USART_PARITY_ODD || Copy_enuMode < USART_PARITY_DISABLED)
        return USART_INVALID_PARITY_MODE;

    UCSR0C = (UCSR0C & 0xCF) | (UPM00 << Copy_enuMode);
    return USART_OK;
}

USART_ErrorStatus USART_enuSetStopBits(USART_StopBitsMode Copy_enuMode)
{
    if (Copy_enuMode != USART_STOP_BITS_1 && Copy_enuMode != USART_STOP_BITS_2)
        return USART_INVALID_STOP_BITS;

    UCSR0C = (UCSR0C & 0xF7) | (USBS0 << Copy_enuMode);
    return USART_OK;
}
USART_ErrorStatus USART_enuSetDataBits(USART_DataBitsMode Copy_enuMode)
{
    if (Copy_enuMode > USART_DATA_BITS_9 ||
        Copy_enuMode < USART_DATA_BITS_5)
        return USART_OUT_OF_RANGE;

    local_enuDataBitsMode = Copy_enuMode;
    UCSR0C = (UCSR0C & 0xF9) | (Copy_enuMode << 1);
    if (Copy_enuMode == USART_DATA_BITS_9)
        SET_BIT(UCSR0B, UCSZ02);
    else
        CLR_BIT(UCSR0B, UCSZ02);

    return USART_OK;
}
USART_ErrorStatus USART_enuTransmitBlocking(u8 Copy_u8Data)
{
    if (!GET_BIT(UCSR0B, TXEN0))
        return USART_TX_DISABLED;

    while (!(UCSR0A & (1 << UDRE0)))
        ;

    UDR0 = Copy_u8Data;

    return USART_OK;
}
USART_ErrorStatus USART_enuReceiveBlocking(u16 *Copy_pu16Data)
{
    if (!GET_BIT(UCSR0B, RXEN0))
        return USART_RX_DISABLED;

    while (!(UCSR0A & (1 << RXC0)))
        ;

    if (local_enuDataBitsMode == USART_DATA_BITS_9)
    {
        *Copy_pu16Data = UDR0 | (GET_BIT(UCSR0B, RXB80) << 8);
    }
    else
    {
        *Copy_pu16Data = UDR0;
    }

    return USART_OK;
}

USART_ErrorStatus USART_enuEnableInterrupt(USART_Interrupt Copy_enuInterrupt)
{
    if (!GET_BIT(SREG, GIE))
        return USART_GIE_DISABLED;

    switch (Copy_enuInterrupt)
    {
    case RX_COMPLETE_INT:
        SET_BIT(UCSR0B, RXCIE0);
        break;

    case TX_COMPLETE_INT:
        SET_BIT(UCSR0B, TXCIE0);
        break;
    case DATA_REG_EMPTY:
        SET_BIT(UCSR0B, UDRIE0);
        break;
    }

    return USART_OK;
}
USART_ErrorStatus USART_enuDisableInterrupt(USART_Interrupt Copy_enuInterrupt)
{
    switch (Copy_enuInterrupt)
    {
    case RX_COMPLETE_INT:
        CLR_BIT(UCSR0B, RXCIE0);
        break;
    case TX_COMPLETE_INT:
        CLR_BIT(UCSR0B, TXCIE0);
        break;
    case DATA_REG_EMPTY:
        CLR_BIT(UCSR0B, UDRIE0);
        break;
    }

    return USART_OK;
}

USART_ErrorStatus USART_enuTransmitNonBlocking(u8 Copy_u8Data, void (*Copy_pfNotificationCallback)(void))
{
    if (!GET_BIT(SREG, GIE))
        return USART_GIE_DISABLED;

    if (!GET_BIT(UCSR0B, TXEN0))
        return USART_TX_DISABLED;

    local_u8DataTransmit = Copy_u8Data;
    local_pfNotificationTransmitCallback =
        Copy_pfNotificationCallback;

    SET_BIT(UCSR0B, UDRIE0);

    return USART_OK;
}

USART_ErrorStatus USART_enuReceiveNonBlocking(u16 *Copy_pu16Data, void (*Copy_pfNotificationCallback)(void))
{
    if (!GET_BIT(SREG, GIE))
        return USART_GIE_DISABLED;
    if (!GET_BIT(UCSR0B, RXCIE0))
        return USART_RXIE_DISABLED;
    if (!GET_BIT(UCSR0B, RXEN0))
        return USART_RX_DISABLED;

    local_u16DataReceive = Copy_pu16Data;
    local_pfNotificationReceiveCallback = Copy_pfNotificationCallback;

    return USART_OK;
}

USART_ErrorStatus USART_enuSetCallback(USART_Interrupt Copy_enuInterrupt, void (*Copy_pfCallback)(void))
{
    if (Copy_pfCallback == NULL)
        return USART_NULL_POINTER;

    USART_Callback[Copy_enuInterrupt] = Copy_pfCallback;

    return USART_OK;
}

void __vector_18(void) __attribute__((signal));
void __vector_18(void)
{
    if (local_enuDataBitsMode == USART_DATA_BITS_9)
    {
        *local_u16DataReceive = UDR0 | (GET_BIT(UCSR0B, RXB80) << 8);
    }
    else
    {
        *local_u16DataReceive = UDR0;
    }
    if (local_pfNotificationReceiveCallback != NULL)
        local_pfNotificationReceiveCallback();
    if (USART_Callback[0] != NULL)
        USART_Callback[0]();
}

void __vector_19(void) __attribute__((signal));
void __vector_19(void)
{
    UDR0 = local_u8DataTransmit;

    CLR_BIT(UCSR0B, UDRIE0);

    if (local_pfNotificationTransmitCallback != NULL)
        local_pfNotificationTransmitCallback();

    if (USART_Callback[DATA_REG_EMPTY] != NULL)
        USART_Callback[DATA_REG_EMPTY]();
}

void __vector_20(void) __attribute__((signal));
void __vector_20(void)
{
    if (USART_Callback[TX_COMPLETE_INT] != NULL)
        USART_Callback[TX_COMPLETE_INT]();
}

USART_ErrorStatus USART_enuEnableMultiProcessorMode(void)
{
    if (local_enuMode != USART_SYNC_MASTER)
        return USART_INVALID_MODE;

    SET_BIT(UCSR0A, MPCM0);

    return USART_OK;
}
USART_ErrorStatus USART_enuDisableMultiProcessorMode(void)
{
    CLR_BIT(UCSR0A, MPCM0);
    return USART_OK;
}
USART_ErrorStatus USART_enuTransmitAddress(u8 Copy_u8Address)
{
    if (!GET_BIT(UCSR0B, TXEN0))
        return USART_TX_DISABLED;

    UCSR0C = (UCSR0C & 0xF9) | (USART_DATA_BITS_9 << 1);
    SET_BIT(UCSR0B, UCSZ02);

    while (!(UCSR0A & (1 << TXC0)))
        ;

    SET_BIT(UCSR0B, TXB80);
    UDR0 = Copy_u8Address;

    UCSR0C = (UCSR0C & 0xF9) | (local_enuDataBitsMode << 1);
    if (local_enuDataBitsMode == USART_DATA_BITS_9)
        SET_BIT(UCSR0B, UCSZ02);

    return USART_OK;
}

USART_ErrorStatus USART_enuSetClkPol(USART_ClkPol Copy_enuMode)
{
    if (Copy_enuMode != USART_CLK_TRANS_FALLING_REC_RISING && Copy_enuMode != USART_CLK_TRANS_RISING_REC_FALLING)
        return USART_INVALID_CLK_POL;
    if (local_enuMode != USART_SYNC_MASTER)
        return USART_INVALID_MODE;

    UCSR0C = (UCSR0C & 0xFE) | Copy_enuMode;
    return USART_OK;
}