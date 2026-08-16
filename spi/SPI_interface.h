// ========================================= SPI_interface.h =========================================
// Author: Eng. Ahmed Ashraf
// SWC: SPI
// Layer: MCAL
// Description: This file contains the public interface definitions for the SPI peripheral.
// =====================================================================================================

#ifndef SPI_INTERFACE_H
#define SPI_INTERFACE_H

typedef enum
{
    SPI_MASTER,
    SPI_SLAVE
} SPI_Role;

typedef enum
{
    SPI_CLK_DIV_4,
    SPI_CLK_DIV_16,
    SPI_CLK_DIV_64,
    SPI_CLK_DIV_128,
    SPI_CLK_DIV_2, // using one of the division factor below will activate the 2x speed
    SPI_CLK_DIV_8,
    SPI_CLK_DIV_32,
} SPI_Clk;

typedef enum
{
    SPI_LEADING_RISING_TRAILING_FALLING,
    SPI_LEADING_FALLING_TRAILING_RISING,
} SPI_ClkPol;

typedef enum
{
    SPI_LEADING_SAMPLE_TRAILING_SETUP,
    SPI_LEADING_SETUP_TRAILING_SAMPLE,
} SPI_ClkPhase;

typedef enum
{
    SPI_LSB_FIRST,
    SPI_MSB_FIRST
} SPI_DataOrder;

typedef enum
{
    SPI_OK,
    SPI_INVALID_ROLE,
    SPI_INVALID_CLK_PHASE,
    SPI_INVALID_CLK_POL,
    SPI_INVALID_DATA_ORDER,
    SPI_OUT_OF_RANGE,
    SPI_DISABLED,
    SPI_NULL_POINTER,
    SPI_GIE_DISABLED,
    SPI_WRITE_COLLISION,
    SPI_BUSY
} SPI_ErrorStatus;

void SPI_voidInit(void);
SPI_ErrorStatus SPI_enuEnable(void);
SPI_ErrorStatus SPI_enuDisable(void);
SPI_ErrorStatus SPI_enuChangeRole(SPI_Role Copy_enuRole);
SPI_ErrorStatus SPI_enuChangeClkRate(SPI_Clk Copy_enuClk);
SPI_ErrorStatus SPI_enuChangeClkPhase(SPI_ClkPhase Copy_enuClkPhase);
SPI_ErrorStatus SPI_enuChangeClkPol(SPI_ClkPol Copy_enuClkPol);
SPI_ErrorStatus SPI_enuChangeDataOrder(SPI_DataOrder Copy_enuOrder);
SPI_ErrorStatus SPI_enuEnableINT(void);
SPI_ErrorStatus SPI_enuDisableINT(void);

SPI_ErrorStatus SPI_enuExchangeByteSync(u8 Copy_u8TxData, u8 *Copy_pu8RxBuffer);
SPI_ErrorStatus SPI_enuExchangeByteAsync(u8 Copy_u8TxData, u8 *Copy_pu8RxBuffer, void (*Copy_pNotificationFunction)(void));

#endif /* SPI_INTERFACE_H */
