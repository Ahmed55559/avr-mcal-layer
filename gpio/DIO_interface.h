// ========================================= DIO_interface.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: DIO/GPIO
// Layer: MCAL
// Description: This file contains the interface information of DIO peripheral like function declarations
// =================================================================================================

#include "STD_TYPES.h"
#ifndef DIO_INTERFACE_H_
#define DIO_INTERFACE_H_

#include "STD_TYPES.h"

/* ===========================
 * Ports
 * =========================== */
#define DIO_PORTB 0
#define DIO_PORTC 1
#define DIO_PORTD 2

/* ===========================
 * Pins
 * =========================== */
#define DIO_PIN0 0
#define DIO_PIN1 1
#define DIO_PIN2 2
#define DIO_PIN3 3
#define DIO_PIN4 4
#define DIO_PIN5 5
#define DIO_PIN6 6
#define DIO_PIN7 7

/* ===========================
 * Pin Values
 * =========================== */
#define DIO_PIN_LOW 0
#define DIO_PIN_HIGH 1

/* ===========================
 * Port Values
 * =========================== */
#define DIO_PORT_LOW 0x00
#define DIO_PORT_HIGH 0xFF

typedef enum
{
    DIO_OK,
    DIO_INVALID_PORT,
    DIO_INVALID_PIN,
    DIO_NULL_POINTER
} DIO_ErrorState;

/* Pin Functions */
DIO_ErrorState DIO_enumSetPinValue(u8 Copy_u8Port,
                                   u8 Copy_u8Pin,
                                   u8 Copy_u8Value);

DIO_ErrorState DIO_enumGetPinValue(u8 Copy_u8Port,
                                   u8 Copy_u8Pin,
                                   u8 *Copy_pu8Value);

DIO_ErrorState DIO_enumTogglePinValue(u8 Copy_u8Port,
                                      u8 Copy_u8Pin);

/* Port Function */
DIO_ErrorState DIO_enumSetPortValue(u8 Copy_u8Port,
                                    u8 Copy_u8Value);

#endif /*DIO_interface*/