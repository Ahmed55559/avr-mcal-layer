// ========================================= TWI_program.c =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: TWI
// Layer: MCAL
// Description: This file contains the implementation of TWI functions
// =================================================================================================

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "TWI_interface.h"
#include "TWI_config.h"
#include "TWI_private.h"

#define MAX_REG_VALUE 255
#define MIN_BITRATE_REG_VALUE 10
#define PRESCALER_BITS_CLR_MASK 0xF8
#define MIN_INVALID_ADDRESS 0b11110000
#define TWCR_REG_INIT_MASK 0xB8

TWI_Context localContext;

static TWI_ErrorStatus TWI_enuCalcBitRate(u16 *Copy_pu16BitRateRegValue, u8 *Copy_pu8PrescalerBitsValue)
{
    if (Copy_pu16BitRateRegValue == NULL || Copy_pu8PrescalerBitsValue == NULL)
        return TWI_NULL_POINTER;

    u16 local_u16TWBRValue = 0;
    u8 local_u8PrescalerValue = 1;
    u8 prescalerBitsCounter;
    for (prescalerBitsCounter = 0; prescalerBitsCounter < 4; prescalerBitsCounter++)
    {
        local_u16TWBRValue = ((SYS_FREQ / TWI_SCL_FREQ) - 16) / (2 * local_u8PrescalerValue); // Equation from the datasheet
        if (local_u16TWBRValue < MAX_REG_VALUE && local_u16TWBRValue >= MIN_BITRATE_REG_VALUE)
            break;
        else
            local_u8PrescalerValue *= 4;
    }

    *Copy_pu16BitRateRegValue = local_u16TWBRValue;
    *Copy_pu8PrescalerBitsValue = prescalerBitsCounter;
    return TWI_OK;
}
static TWI_ErrorStatus TWI_enuSetBitRate(u16 Copy_u16BitRateRegValue, u8 Copy_u8PrescalerBitsValue)
{
    if (Copy_u16BitRateRegValue > MAX_REG_VALUE || Copy_u16BitRateRegValue <= MIN_BITRATE_REG_VALUE)
    {
        return TWI_OUT_OF_RANGE;
    }
    TWBR = Copy_u16BitRateRegValue;
    TWSR = (TWSR & PRESCALER_BITS_CLR_MASK) | Copy_u8PrescalerBitsValue;
    return TWI_OK;
}

static void TWI_voidWait()
{
    while (!GET_BIT(TWCR, TWINT))
        ;
}

static void TWI_voidSendStart()
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
}

static void TWI_voidSendStop()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

static void TWI_voidGetStatus(TWI_HWStatus *Copy_penuStatus)
{
    *Copy_penuStatus = TWSR & PRESCALER_BITS_CLR_MASK;
}

static TWI_ErrorStatus TWI_enuLoadAddress(TWI_Direction Copy_enuDirection)
{
    if (Copy_enuDirection != TWI_WRITE && Copy_enuDirection != TWI_READ)
        return TWI_INVALID_DIR;
    if (localContext.targetAddress >= MIN_INVALID_ADDRESS)
        return TWI_OUT_OF_RANGE;

    TWDR = (localContext.targetAddress << 1) | Copy_enuDirection;
    TWCR = (1 << TWINT) | (1 << TWEN);

    return TWI_OK;
}

static void TWI_voidSendByte(u8 Copy_u8Byte)
{
    TWDR = Copy_u8Byte;
    TWCR = (1 << TWINT) | (1 << TWEN);
}
static void TWI_voidReadReceivedByte(u8 *Copy_pu8Byte)
{
    *Copy_pu8Byte = TWDR;
    TWCR = (1 << TWINT) | (1 << TWEN);
}

static void TWI_voidReset(void)
{
    localContext.busState = TWI_BUS_IDLE;
    localContext.targetAddress = 0;
    localContext.lengthOfBuffer = localContext.indexOfNextByte = 0;
    localContext.RxBuffer = NULL;
    localContext.TxBuffer = NULL;
    localContext.NotificationCallBack = NULL;
    TWCR |= 1 << TWEA;
}

static void TWI_voidMasterTxStateMachine(TWI_HWStatus status)
{
    switch (status)
    {
    case M_Start:
    case M_RepeatedStart:
        if (TWI_enuLoadAddress(localContext.directionOfData) != TWI_OK)
        {
            localContext.errorStatus = TWI_INVALID_STATUS;
            TWI_voidReset();
            TWI_voidSendStop();
        }
        break;
    case MT_SLA_W_ACK:
        TWI_voidSendByte(localContext.TxBuffer[localContext.indexOfNextByte++]);
        break;
    case MT_SLA_W_NACK:
    case MT_DATA_NACK:
        localContext.errorStatus = TWI_NACK;
        TWI_voidReset();
        TWI_voidSendStop();
        break;
    case MT_DATA_ACK:
        if (localContext.lengthOfBuffer - localContext.indexOfNextByte != 1)
        {
            TWI_voidSendByte(localContext.TxBuffer[localContext.indexOfNextByte++]);
        }
        else
        {
            TWI_voidSendByte(localContext.TxBuffer[localContext.indexOfNextByte]);
        }
        break;
    case M_ARBITRATION_LOST:
        localContext.errorStatus = TWI_ARBITRATION_LOST;
        TWI_voidReset();
        TWI_voidSendStop();
        break;
    default:
        localContext.errorStatus = TWI_INVALID_STATUS;
        TWI_voidReset();
        TWI_voidSendStop();
        break;
    }
}
static void TWI_voidMasterRxStateMachine(TWI_HWStatus status)
{
    switch (status)
    {
    case M_Start:
    case M_RepeatedStart:
        if (TWI_enuLoadAddress(localContext.directionOfData) != TWI_OK)
        {
            localContext.errorStatus = TWI_INVALID_STATUS;
            TWI_voidReset();
            TWI_voidSendStop();
        }
        break;
    case MR_SLA_R_ACK:
        break;

    case MR_DATA_ACK:
        if (localContext.lengthOfBuffer - localContext.indexOfNextByte != 1)
        {
            CLR_BIT(TWCR, TWEA);
            TWI_voidReadReceivedByte(&localContext.RxBuffer[localContext.indexOfNextByte++]);
        }
        else
        {
            TWI_voidReadReceivedByte(&localContext.RxBuffer[localContext.indexOfNextByte++]);
        }
        break;

    case MR_DATA_NACK:
    case MR_SLA_R_NACK:
        localContext.errorStatus = TWI_NACK;
        TWI_voidReset();
        TWI_voidSendStop();
        break;
    case M_ARBITRATION_LOST:
        localContext.errorStatus = TWI_ARBITRATION_LOST;
        TWI_voidReset();
        TWI_voidSendStop();
        break;
    default:
        localContext.errorStatus = TWI_INVALID_STATUS;
        TWI_voidReset();
        TWI_voidSendStop();
        break;
    }
}
static void TWI_voidSlaveTxStateMachine(TWI_HWStatus status)
{
    switch (status)
    {
    case ST_SLA_R_ACK:
    case ST_ARBITRATION_LOST_AS_MASTER_SLA_R_ACK:
    case ST_DATA_ACK:
        if (localContext.lengthOfBuffer - localContext.indexOfNextByte != 1)
        {
            TWI_voidSendByte(localContext.TxBuffer[localContext.indexOfNextByte++]);
        }
        else
        {
            TWI_voidSendByte(localContext.TxBuffer[localContext.indexOfNextByte]);
        }
        break;
    case ST_DATA_NACK:
        localContext.errorStatus = TWI_OK;
        TWI_voidReset();
        break;
    case ST_LAST_DATA_ACK:
        localContext.errorStatus = TWI_OK;
        TWI_voidReset();
        break;
    default:
        localContext.errorStatus = TWI_INVALID_STATUS;
        TWI_voidReset();
        break;
    }
}
static void TWI_voidSlaveRxStateMachine(TWI_HWStatus status)
{
    switch (status)
    {
    case SR_SLA_W_ACK:
    case SR_ARBITRATION_LOST_AS_MASTER_SLA_W_ACK:
    case SR_GEN_CALL_ACK:
    case SR_ARBITRATION_LOST_AS_MASTER_GEN_CALL_ACK:
        break;
    case SR_DATA_ACK:
    case SR_GEN_CALL_DATA_ACK:
        if (localContext.lengthOfBuffer - localContext.indexOfNextByte != 1)
        {
            TWI_voidReadReceivedByte(&localContext.RxBuffer[localContext.indexOfNextByte++]);
        }
        else
        {
            TWI_voidReadReceivedByte(&localContext.RxBuffer[localContext.indexOfNextByte++]);
        }
        break;
    case SR_DATA_NACK:
    case SR_GEN_CALL_DATA_NACK:
    case SR_STOP_REPEATED_START:
        localContext.errorStatus = TWI_NACK;
        TWI_voidReset();
        break;
    default:
        localContext.errorStatus = TWI_INVALID_STATUS;
        TWI_voidReset();
        break;
    }
}

TWI_ErrorStatus TWI_voidInit()
{
    TWI_ErrorStatus local_enuErrorStatus = TWI_OK;
    localContext.role = TWI_ROLE;
    localContext.syncMode = TWI_SYNC_MODE;
    localContext.errorStatus = TWI_OK;
    TWI_voidReset();
    u16 local_u16BitRateValue;
    u8 local_u8PrescalerBitsValue;

    local_enuErrorStatus = TWI_enuCalcBitRate(&local_u16BitRateValue, &local_u8PrescalerBitsValue);
    if (local_enuErrorStatus == TWI_OK)
        TWI_enuSetBitRate(local_u16BitRateValue, local_u8PrescalerBitsValue);
    else
        return local_enuErrorStatus;

    TWAR = (TWAR & 0) | (TWI_SLAVE_ADDRESS << 1) | (TWI_SLAVE_GENERAL_CALL << TWGCE);
    TWCR = (TWCR & TWCR_REG_INIT_MASK) | (TWI_SYNC << TWIE) | (1 << TWEA) | (1 << TWEN);
    return local_enuErrorStatus;
}
TWI_ErrorStatus TWI_enuMasterTransmit(u8 Copy_u8SlaveAddress, u8 *TxBuffer, u16 lengthOfBuffer)
{
    if (TxBuffer == NULL)
        return TWI_NULL_POINTER;
    if (localContext.busState == TWI_BUS_BUSY)
        return TWI_ERROR_BUS_BUSY;
    if (localContext.syncMode == TWI_ASYNC && !GET_BIT(SREG, GIE))
        return TWI_GIE_DISABLED;

    localContext.errorStatus = TWI_OK;
    localContext.busState = TWI_BUS_BUSY;
    localContext.targetAddress = Copy_u8SlaveAddress;
    localContext.TxBuffer = TxBuffer;
    localContext.lengthOfBuffer = lengthOfBuffer;
    localContext.indexOfNextByte = 0;

    if (TWI_SYNC_MODE == TWI_SYNC)
    {
        TWI_HWStatus status;
        TWI_voidSendStart();
        TWI_voidWait();

        TWI_voidGetStatus(&status);

        if (status == M_Start || status == M_RepeatedStart)
        {

            if (TWI_enuLoadAddress(TWI_WRITE) == TWI_OK)
            {
                TWI_voidWait();
                TWI_voidGetStatus(&status);

                if (status == MT_SLA_W_ACK)
                {
                    for (u16 byteCounter = 0; byteCounter < lengthOfBuffer; byteCounter++)
                    {
                        if (status == MT_DATA_NACK)
                        {
                            TWI_voidSendStop();
                            TWI_voidReset();
                            return TWI_NACK;
                        }

                        TWI_voidSendByte(TxBuffer[byteCounter]);
                        TWI_voidWait();
                        TWI_voidGetStatus(&status);
                    }
                    TWI_voidSendStop();
                    TWI_voidReset();
                }
                else if (status == MT_SLA_W_NACK)
                {
                    TWI_voidSendStop();
                    TWI_voidReset();
                    return TWI_NACK;
                }

                else
                {
                    TWI_voidSendStop();
                    TWI_voidReset();
                    return TWI_INVALID_STATUS;
                }
            }
        }
        else
        {
            TWI_voidSendStop();
            TWI_voidReset();
            return TWI_INVALID_STATUS;
        }
    }
    else
    {
        TWI_voidSendStart();
        localContext.busState = TWI_BUS_BUSY;
        localContext.directionOfData = TWI_WRITE;
    }
    return TWI_OK;
}

TWI_ErrorStatus TWI_enuMasterReceive(u8 Copy_u8SlaveAddress, u8 *RxBuffer, u16 lengthOfBuffer)
{
    if (RxBuffer == NULL)
        return TWI_NULL_POINTER;
    if (localContext.busState == TWI_BUS_BUSY)
        return TWI_ERROR_BUS_BUSY;
    if (localContext.syncMode == TWI_ASYNC && !GET_BIT(SREG, GIE))
        return TWI_GIE_DISABLED;

    localContext.errorStatus = TWI_OK;
    localContext.busState = TWI_BUS_BUSY;
    localContext.targetAddress = Copy_u8SlaveAddress;
    localContext.RxBuffer = RxBuffer;
    localContext.lengthOfBuffer = lengthOfBuffer;
    localContext.indexOfNextByte = 0;

    if (TWI_SYNC_MODE == TWI_SYNC)
    {
        TWI_HWStatus status;
        TWI_voidSendStart();
        TWI_voidWait();

        TWI_voidGetStatus(&status);

        if (status == M_Start || status == M_RepeatedStart)
        {

            if (TWI_enuLoadAddress(TWI_READ) == TWI_OK)
            {
                TWI_voidWait();
                TWI_voidGetStatus(&status);
                if (status == MR_SLA_R_ACK)
                {
                    for (u16 byteCounter = 0; byteCounter < lengthOfBuffer; byteCounter++)
                    {
                        if (lengthOfBuffer - byteCounter == 1)
                            CLR_BIT(TWCR, TWEA);

                        TWI_voidWait();
                        TWI_voidReadReceivedByte(&RxBuffer[byteCounter]);
                    }
                    TWI_voidSendStop();
                    TWI_voidReset();
                }
                else if (status == MR_SLA_R_NACK)
                {
                    TWI_voidSendStop();
                    TWI_voidReset();
                    return TWI_NACK;
                }
                else
                {
                    TWI_voidSendStop();
                    TWI_voidReset();
                    return TWI_INVALID_STATUS;
                }
            }
        }
        else
        {
            TWI_voidSendStop();
            TWI_voidReset();
            return TWI_INVALID_STATUS;
        }
    }
    else
    {
        TWI_voidSendStart();
        localContext.busState = TWI_BUS_BUSY;
        localContext.directionOfData = TWI_READ;
    }
    return TWI_OK;
}

TWI_ErrorStatus TWI_enuSlaveTransmit(u8 *TxBuffer, u16 lengthOfBuffer)
{
    if (TxBuffer == NULL)
        return TWI_NULL_POINTER;
    if (localContext.busState == TWI_BUS_BUSY)
        return TWI_ERROR_BUS_BUSY;
    if (localContext.syncMode == TWI_ASYNC && !GET_BIT(SREG, GIE))
        return TWI_GIE_DISABLED;

    localContext.errorStatus = TWI_OK;
    localContext.busState = TWI_BUS_BUSY;
    localContext.TxBuffer = TxBuffer;
    localContext.lengthOfBuffer = lengthOfBuffer;
    localContext.indexOfNextByte = 0;

    TWI_HWStatus status;
    if (TWI_SYNC_MODE == TWI_SYNC)
    {
        // Sync
        TWI_voidGetStatus(&status);
        if (status == ST_SLA_R_ACK || status == ST_ARBITRATION_LOST_AS_MASTER_SLA_R_ACK)
        {
            for (u16 byteCounter = 0; byteCounter < lengthOfBuffer; byteCounter++)
            {
                if (status == ST_DATA_NACK)
                {
                    TWI_voidReset();
                    return TWI_OK;
                }

                TWI_voidSendByte(TxBuffer[byteCounter]);
                TWI_voidWait();
                TWI_voidGetStatus(&status);
            }
            TWI_voidReset();
        }
        else
        {
            TWI_voidReset();
            return TWI_INVALID_STATUS;
        }
    }
    else
    {
        // Async
        TWI_voidGetStatus(&status);
        if (status == ST_SLA_R_ACK || status == ST_ARBITRATION_LOST_AS_MASTER_SLA_R_ACK)
        {
            localContext.busState = TWI_BUS_BUSY;
            localContext.directionOfData = TWI_WRITE;
        }
        else
        {
            TWI_voidReset();
            return TWI_INVALID_STATUS;
        }
    }

    return TWI_OK;
}
TWI_ErrorStatus TWI_enuSlaveListen(u8 *RxBuffer, u16 lengthOfBuffer)
{
    if (RxBuffer == NULL)
        return TWI_NULL_POINTER;
    if (localContext.busState == TWI_BUS_BUSY)
        return TWI_ERROR_BUS_BUSY;
    if (localContext.syncMode == TWI_ASYNC && !GET_BIT(SREG, GIE))
        return TWI_GIE_DISABLED;

    localContext.errorStatus = TWI_OK;
    localContext.busState = TWI_BUS_BUSY;
    localContext.RxBuffer = RxBuffer;
    localContext.lengthOfBuffer = lengthOfBuffer;
    localContext.indexOfNextByte = 0;

    TWI_HWStatus status;
    if (TWI_SYNC_MODE == TWI_SYNC)
    {
        // Sync
        TWI_voidGetStatus(&status);
        if (status == SR_SLA_W_ACK || status == SR_ARBITRATION_LOST_AS_MASTER_SLA_W_ACK || status == SR_GEN_CALL_ACK || status == SR_ARBITRATION_LOST_AS_MASTER_GEN_CALL_ACK)
        {
            for (u16 byteCounter = 0; byteCounter < lengthOfBuffer; byteCounter++)
            {
                if (status == SR_DATA_NACK || status == SR_GEN_CALL_DATA_NACK || status == SR_STOP_REPEATED_START)
                {
                    TWI_voidReset();
                    return TWI_OK;
                }

                TWI_voidWait();
                TWI_voidReadReceivedByte(&RxBuffer[byteCounter]);
                TWI_voidGetStatus(&status);
            }
            TWI_voidReset();
        }
        else
        {
            TWI_voidReset();
            return TWI_INVALID_STATUS;
        }
    }
    else
    {
        // Async
        TWI_voidGetStatus(&status);
        if (status == SR_SLA_W_ACK || status == SR_ARBITRATION_LOST_AS_MASTER_SLA_W_ACK || status == SR_GEN_CALL_ACK || status == SR_ARBITRATION_LOST_AS_MASTER_GEN_CALL_ACK)
        {
            localContext.busState = TWI_BUS_BUSY;
            localContext.directionOfData = TWI_READ;
        }
        else
        {
            TWI_voidReset();
            return TWI_INVALID_STATUS;
        }
    }
    return TWI_OK;
}

// ----------------------------------------------
TWI_ErrorStatus TWI_enuSetCallBack(void (*Copy_pCallbackFunc)(void))
{
    if (Copy_pCallbackFunc == NULL)
        return TWI_NULL_POINTER;

    localContext.NotificationCallBack = Copy_pCallbackFunc;

    return TWI_OK;
}

void __vector_25(void) __attribute__((signal));
void __vector_25(void)
{
    if (TWI_SYNC_MODE != TWI_ASYNC)
        return;

    TWI_voidGetStatus(&localContext.HWstatus);
    switch (localContext.role)
    {
    case TWI_MASTER:

        switch (localContext.directionOfData)
        {
        case TWI_WRITE:
            TWI_voidMasterTxStateMachine(localContext.HWstatus);
            break;

        case TWI_READ:
            TWI_voidMasterRxStateMachine(localContext.HWstatus);
            break;
        }

        break;

    case TWI_SLAVE:

        switch (localContext.directionOfData)
        {
        case TWI_WRITE:
            TWI_voidSlaveTxStateMachine(localContext.HWstatus);
            break;

        case TWI_READ:
            TWI_voidSlaveRxStateMachine(localContext.HWstatus);
            break;
        }
        break;
    }
}
