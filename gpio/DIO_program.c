// ========================================= DIO_program.c =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: DIO/GPIO
// Layer: MCAL
// Description: This file contains the implementation of DIO peripheral functions
// =================================================================================================

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DIO_interface.h"
#include "DIO_config.h"
#include "DIO_private.h"
#include "PORT_private.h"

static volatile u8 *PORT_regs[3] = {&PORTB_REG, &PORTC_REG, &PORTD_REG};
static volatile u8 *PIN_regs[3] = {&PINB_REG, &PINC_REG, &PIND_REG};

DIO_ErrorState DIO_enumSetPinValue(u8 Copy_u8Port, u8 Copy_u8Pin, u8 Copy_u8Value)
{

    if (Copy_u8Port >= 3)
        return DIO_INVALID_PORT;
    if (Copy_u8Pin >= 8)
        return DIO_INVALID_PIN;

    if (Copy_u8Value == DIO_PIN_HIGH)
    {
        SET_BIT(*PORT_regs[Copy_u8Port], Copy_u8Pin);
    }
    else
    {
        CLR_BIT(*PORT_regs[Copy_u8Port], Copy_u8Pin);
    }
    return DIO_OK;
}
DIO_ErrorState DIO_enumGetPinValue(u8 Copy_u8Port, u8 Copy_u8Pin, u8 *Copy_pu8Value)
{

    if (Copy_u8Port >= 3)
        return DIO_INVALID_PORT;
    if (Copy_u8Pin >= 8)
        return DIO_INVALID_PIN;
    if (Copy_pu8Value == NULL)
        return DIO_NULL_POINTER;

    *Copy_pu8Value = GET_BIT(*PIN_regs[Copy_u8Port], Copy_u8Pin);
    return DIO_OK;
}
DIO_ErrorState DIO_enumTogglePinValue(u8 Copy_u8Port, u8 Copy_u8Pin)
{

    if (Copy_u8Port >= 3)
        return DIO_INVALID_PORT;
    if (Copy_u8Pin >= 8)
        return DIO_INVALID_PIN;

    TOG_BIT(*PORT_regs[Copy_u8Port], Copy_u8Pin);
    return DIO_OK;
}

DIO_ErrorState DIO_enumSetPortValue(u8 Copy_u8Port, u8 Copy_u8Value)
{
    if (Copy_u8Port >= 3)
        return DIO_INVALID_PORT;
    if (Copy_u8Value < DIO_PORT_LOW || Copy_u8Value > DIO_PORT_HIGH)
    {
        return DIO_NULL_POINTER;
    }

    PORT_regs[Copy_u8Port] = Copy_u8Value;

    return DIO_OK;
}