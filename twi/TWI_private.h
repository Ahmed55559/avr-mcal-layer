// ========================================= TWI_private.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: TWI
// Layer: MCAL
// Description: This file contains the private definitions and macros for TWI peripheral
// =================================================================================================

#ifndef TWI_PRIVATE_H
#define TWI_PRIVATE_H

#define SREG *((volatile u8 *)0x5F) // we will use the 7th bit to read global interrupt status (Enabled/Disabled)

#define TWBR *((volatile u8 *)0xB8)  // BitRate Register
#define TWCR *((volatile u8 *)0xBC)  // TWI Control Register
#define TWSR *((volatile u8 *)0xB9)  // TWI Status Register
#define TWDR *((volatile u8 *)0xBB)  // TWI Data Register
#define TWAR *((volatile u8 *)0xBA)  // TWI Slave Address Register
#define TWAMR *((volatile u8 *)0xBD) // TWI Slave Address Mask Register

/* Bits defining */

/* SREG */
#define GIE 7 // globale interrupt enable

/* TWCR */
#define TWINT 7 // TWI interrupt flag
#define TWEA 6  // Enable Ack
#define TWSTA 5 // Start condition
#define TWSTO 4 // Stop condition
#define TWWC 3  // Write Collision flag
#define TWEN 2  // TWI Enable
#define TWIE 0  // TWI INT Enable

/* TWSR */
#define TWS3 3  // first Status bit
#define TWPS1 1 // Prescaler bit 1
#define TWPS0 0 // Prescaler bit 0

/* TWDR */
#define TWD0 0 // Data bit 0 (R/W)

/* TWAR */
#define TWGCE 0 // General call enable

/* helper */
static TWI_ErrorStatus TWI_enuCalcBitRate(u16 *Copy_pu16BitRateRegValue, u8 *Copy_pu8PrescalerBitsValue);
static TWI_ErrorStatus TWI_enuSetBitRate(u16 Copy_u16BitRateRegValue, u8 Copy_u8PrescalerBitsValue);
static TWI_ErrorStatus TWI_enuLoadAddress(TWI_Direction Copy_enuDirection);

static void TWI_voidGetStatus(TWI_HWStatus *Copy_penuStatus);
static void TWI_voidSendStart(void);
static void TWI_voidWait(void);
static void TWI_voidSendStop(void);
static void TWI_voidReset(void);

static void TWI_voidSendByte(u8 Copy_u8Byte);
static void TWI_voidReadReceivedByte(u8 *Copy_pu8Byte);

static void TWI_voidMasterTxStateMachine(TWI_HWStatus status);
static void TWI_voidMasterRxStateMachine(TWI_HWStatus status);
static void TWI_voidSlaveTxStateMachine(TWI_HWStatus status);
static void TWI_voidSlaveRxStateMachine(TWI_HWStatus status);

#endif /* TWI_PRIVATE_H */