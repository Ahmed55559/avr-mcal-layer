// ========================================= USART_interface.h =========================================
// Author: Eng. Ahmed Ashraf
// SWC: USART
// Layer: MCAL
// Description: This file contains the public interface definitions for the USART peripheral.
// =====================================================================================================

#ifndef USART_INTERFACE_H
#define USART_INTERFACE_H

typedef enum
{
    USART_ASYNC_NORMAL_SPEED,
    USART_ASYNC_X2_SPEED,
    USART_SYNC_MASTER,
    //  USART_SPI_MASTER
} USART_Mode;

typedef enum
{
    RX_COMPLETE_INT,
    DATA_REG_EMPTY,
    TX_COMPLETE_INT
} USART_Interrupt;

typedef enum
{
    USART_PARITY_DISABLED,
    USART_PARITY_EVEN = 2,
    USART_PARITY_ODD = 3
} USART_ParityMode;

typedef enum
{
    USART_STOP_BITS_1,
    USART_STOP_BITS_2
} USART_StopBitsMode;

typedef enum
{
    USART_DATA_BITS_5,
    USART_DATA_BITS_6,
    USART_DATA_BITS_7,
    USART_DATA_BITS_8,
    USART_DATA_BITS_9 = 3 // and UCSZ02 bit is 1
} USART_DataBitsMode;

typedef enum
{
    USART_CLK_TRANS_RISING_REC_FALLING,
    USART_CLK_TRANS_FALLING_REC_RISING
} USART_ClkPol;

typedef enum
{
    USART_OK,
    USART_INVALID_MODE,
    USART_INVALID_STOP_BITS,
    USART_INVALID_CLK_POL,
    USART_INVALID_PARITY_MODE,
    USART_INVALID_INTERRUPT,
    USART_OUT_OF_RANGE,
    USART_NULL_POINTER,
    USART_GIE_DISABLED,
    USART_TXIE_DISABLED,
    USART_RXIE_DISABLED,
    USART_TX_DISABLED,
    USART_RX_DISABLED
} USART_ErrorStatus;

void USART_voidInit(void);

void USART_enuSetBaudRate(u32 Copy_u32BaudRate);

// sync, async
USART_ErrorStatus USART_enuSetMode(USART_Mode Copy_enuMode);

// frame format
USART_ErrorStatus USART_enuSetParity(USART_ParityMode Copy_enuMode);
USART_ErrorStatus USART_enuSetStopBits(USART_StopBitsMode Copy_enuMode);
USART_ErrorStatus USART_enuSetDataBits(USART_DataBitsMode Copy_enuMode);

// polling send/receive
USART_ErrorStatus USART_enuTransmitBlocking(u8 Copy_u8Data);
USART_ErrorStatus USART_enuReceiveBlocking(u16 *Copy_pu16Data);

// int send/receive
USART_ErrorStatus USART_enuTransmitNonBlocking(u8 Copy_u8Data, void (*Copy_pfNotificationCallback)(void));
USART_ErrorStatus USART_enuReceiveNonBlocking(u16 *Copy_pu16Data, void (*Copy_pfNotificationCallback)(void));

USART_ErrorStatus USART_enuEnableInterrupt(USART_Interrupt Copy_enuInterrupt);
USART_ErrorStatus USART_enuDisableInterrupt(USART_Interrupt Copy_enuInterrupt);

USART_ErrorStatus USART_enuSetCallback(USART_Interrupt Copy_enuInterrupt, void (*Copy_pfCallback)(void));

// Multi-Processors communication mode
USART_ErrorStatus USART_enuEnableMultiProcessorMode(void);
USART_ErrorStatus USART_enuDisableMultiProcessorMode(void);
USART_ErrorStatus USART_enuTransmitAddress(u8 Copy_u8Address);

USART_ErrorStatus USART_enuSetClkPol(USART_ClkPol Copy_enuMode);

#endif /* USART_INTERFACE_H */
