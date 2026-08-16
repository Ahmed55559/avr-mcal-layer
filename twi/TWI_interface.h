// ========================================= TWI_interface.h =========================================
// Author: Eng. Ahmed Ashraf
// SWC: TWI
// Layer: MCAL
// Description: This file contains the public interface definitions for the TWI peripheral.
// =====================================================================================================

#ifndef TWI_INTERFACE_H
#define TWI_INTERFACE_H

typedef enum
{
    TWI_SYNC,
    TWI_ASYNC
} TWI_SyncMode;

typedef enum
{
    TWI_MASTER,
    TWI_SLAVE
} TWI_Role;

typedef enum
{
    TWI_WRITE = 0,
    TWI_READ = 1
} TWI_Direction;

typedef enum
{
    TWI_STAGE_START,
    TWI_STAGE_ADDRESS,
    TWI_STAGE_DATA,
    TWI_STAGE_STOP
} TWI_TransactionStage;

typedef enum
{
    TWI_BUS_IDLE,
    TWI_BUS_BUSY
} TWI_BusState;

typedef enum
{
    TWI_OK,
    TWI_INVALID_MODE,
    TWI_INVALID_STATUS,
    TWI_INVALID_DIR,
    TWI_OUT_OF_RANGE,
    TWI_NULL_POINTER,
    TWI_GIE_DISABLED,
    TWI_ERROR_BUS_BUSY,
    TWI_ARBITRATION_LOST,
    TWI_NACK
} TWI_ErrorStatus;

typedef enum
{
    // start
    M_Start = 0x08,
    M_RepeatedStart = 0x10,

    // Master Transmitter Mode
    MT_SLA_W_ACK = 0x18,
    MT_SLA_W_NACK = 0x20,
    MT_DATA_ACK = 0x28,
    MT_DATA_NACK = 0x30,
    M_ARBITRATION_LOST = 0x38,

    // Master Reciever Mode
    MR_SLA_R_ACK = 0x40,
    MR_SLA_R_NACK = 0x48,
    MR_DATA_ACK = 0x50,
    MR_DATA_NACK = 0x58,

    // Slave Reciever Mode
    SR_SLA_W_ACK = 0x60,
    SR_ARBITRATION_LOST_AS_MASTER_SLA_W_ACK = 0x68,
    SR_GEN_CALL_ACK = 0x70,
    SR_ARBITRATION_LOST_AS_MASTER_GEN_CALL_ACK = 0x78,
    SR_DATA_ACK = 0x80,
    SR_DATA_NACK = 0x88,
    SR_GEN_CALL_DATA_ACK = 0x90,
    SR_GEN_CALL_DATA_NACK = 0x98,
    SR_STOP_REPEATED_START = 0xA0,

    // Slave Transmitter Mode
    ST_SLA_R_ACK = 0xA8,
    ST_ARBITRATION_LOST_AS_MASTER_SLA_R_ACK = 0xB0,
    ST_DATA_ACK = 0xB8,
    ST_DATA_NACK = 0xC0,
    ST_LAST_DATA_ACK = 0xC8,

    // Miscellaneous States
    TWI_NO_STATE_AVAILABLE = 0xF8,
    TWI_ILLEGALE_START_OR_STOP = 0x00
} TWI_HWStatus;

typedef struct
{
    TWI_Role role;
    TWI_SyncMode syncMode;

    TWI_Direction directionOfData;
    TWI_HWStatus HWstatus;
    TWI_ErrorStatus errorStatus;
    TWI_BusState busState;

    u8 *TxBuffer;
    u8 *RxBuffer;
    u16 lengthOfBuffer;
    u16 indexOfNextByte;

    void (*NotificationCallBack)(void);

    u8 targetAddress;
} TWI_Context;

TWI_ErrorStatus TWI_voidInit();
TWI_ErrorStatus TWI_enuMasterTransmit(u8 Copy_u8SlaveAddress, u8 *TxBuffer, u16 length);
TWI_ErrorStatus TWI_enuMasterReceive(u8 Copy_u8SlaveAddress, u8 *RxBuffer, u16 length);
TWI_ErrorStatus TWI_enuSlaveTransmit(u8 *TxBuffer, u16 length);
TWI_ErrorStatus TWI_enuSlaveListen(u8 *RxBuffer, u16 length);
TWI_ErrorStatus TWI_enuSetCallBack(void (*Copy_pCallbackFunc)(void));
#endif /* TWI_INTERFACE_H */
