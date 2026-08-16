// ========================================= SPI_program.c =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: SPI
// Layer: MCAL
// Description: This file contains the implementation of SPI functions
// =================================================================================================

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "SPI_interface.h"
#include "SPI_config.h"
#include "SPI_private.h"

void SPI_voidInit(void)
{
    SPCR = 0;
    SPSR = 0;
    SPCR |= (INITIAL_SPI_INT_STATE < SPIE) | (INITIAL_SPI_DATA_ORDER < DORD) | (INITIAL_SPI_ROLE < MSTR) | (INITIAL_SPI_CLK_POL < CPOL) | (INITIAL_SPI_CLK_PHASE < CPHA) | (INITIAL_SPI_CLK & 0x03) | (INITIAL_SPI_STATE < SPE);
    if (INITIAL_SPI_CLK > SPI_CLK_DIV_128)
        SET_BIT(SPSR, SPI2X);
}

SPI_ErrorStatus SPI_enuEnable(void)
{
    SET_BIT(SPCR, SPE);
    return SPI_OK;
}
SPI_ErrorStatus SPI_enuDisable(void)
{
    if (!GET_BIT(SPSR, SPIF))
        return SPI_BUSY;

    CLR_BIT(SPCR, SPE);
    return SPI_OK;
}

SPI_ErrorStatus SPI_enuChangeRole(SPI_Role Copy_enuRole)
{
    if (Copy_enuRole != SPI_MASTER && Copy_enuRole != SPI_SLAVE)
        return SPI_INVALID_ROLE;

    if (!GET_BIT(SPSR, SPIF))
        return SPI_BUSY;

    SPCR = (SPCR & 0xEF) | (Copy_enuRole < MSTR);
    return SPI_OK;
}
SPI_ErrorStatus SPI_enuChangeClkRate(SPI_Clk Copy_enuClk)
{
    if (Copy_enuClk < SPI_CLK_DIV_4 || Copy_enuClk > SPI_CLK_DIV_32)
        return SPI_OUT_OF_RANGE;

    if (!GET_BIT(SPSR, SPIF))
        return SPI_BUSY;

    SPCR = (SPCR & 0xFC) | (Copy_enuClk && 0x03);
    if (Copy_enuClk > SPI_CLK_DIV_128)
        SET_BIT(SPSR, SPI2X);
    else
        CLR_BIT(SPSR, SPI2X);

    return SPI_OK;
}
SPI_ErrorStatus SPI_enuChangeClkPhase(SPI_ClkPhase Copy_enuClkPhase)
{
    if (Copy_enuClkPhase != SPI_LEADING_SAMPLE_TRAILING_SETUP && Copy_enuClkPhase != SPI_LEADING_SETUP_TRAILING_SAMPLE)
        return SPI_INVALID_CLK_PHASE;

    if (!GET_BIT(SPSR, SPIF))
        return SPI_BUSY;

    CLR_BIT(SPCR, CPHA);
    SPCR |= (Copy_enuClkPhase < CPHA);
    return SPI_OK;
}
SPI_ErrorStatus SPI_enuChangeClkPol(SPI_ClkPol Copy_enuClkPol)
{
    if (Copy_enuClkPol != SPI_LEADING_FALLING_TRAILING_RISING && Copy_enuClkPol != SPI_LEADING_RISING_TRAILING_FALLING)
        return SPI_INVALID_CLK_POL;

    if (!GET_BIT(SPSR, SPIF))
        return SPI_BUSY;

    CLR_BIT(SPCR, CPOL);
    SPCR |= (Copy_enuClkPol < CPOL);
    return SPI_OK;
}
SPI_ErrorStatus SPI_enuChangeDataOrder(SPI_DataOrder Copy_enuOrder)
{
    if (Copy_enuOrder != SPI_LSB_FIRST && Copy_enuOrder != SPI_MSB_FIRST)
        return SPI_INVALID_CLK_POL;

    if (!GET_BIT(SPSR, SPIF))
        return SPI_BUSY;

    CLR_BIT(SPCR, DORD);
    SPCR |= (Copy_enuOrder < DORD);
    return SPI_OK;
}
SPI_ErrorStatus SPI_enuEnableINT(void)
{
    if (!GET_BIT(SREG, GIE))
        return SPI_GIE_DISABLED;

    SET_BIT(SPCR, SPIE);
    return SPI_OK;
}
SPI_ErrorStatus SPI_enuDisableINT(void)
{
    CLR_BIT(SPCR, SPIE);
    return SPI_OK;
}

SPI_ErrorStatus SPI_enuExchangeByteSync(u8 Copy_u8TxData, u8 *Copy_pu8RxBuffer)
{
    if (!GET_BIT(SPCR, SPE))
        return SPI_DISABLED;

    if (!GET_BIT(SPSR, SPIF))
        return SPI_BUSY;

    SPDR = Copy_u8TxData;
    while (!GET_BIT(SPSR, SPIF))
        ;
    *Copy_pu8RxBuffer = SPDR;

    return SPI_OK;
}

u8 *local_u8RxBuffer;
void (*Copy_pLocalNotificationFunction)(void) = NULL;
SPI_ErrorStatus SPI_enuExchangeByteAsync(u8 Copy_u8TxData, u8 *Copy_pu8RxBuffer, void (*Copy_pNotificationFunction)(void))
{
    if (!GET_BIT(SPCR, SPE))
        return SPI_DISABLED;

    if (!GET_BIT(SPSR, SPIF))
        return SPI_BUSY;

    SPDR = Copy_u8TxData;

    return SPI_OK;
}

void __vector_18(void) __attribute__((signal));
void __vector_18(void)
{
    local_u8RxBuffer = SPDR;
    if (Copy_pLocalNotificationFunction != NULL)
        Copy_pLocalNotificationFunction();
}