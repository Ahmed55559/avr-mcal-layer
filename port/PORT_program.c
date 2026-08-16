// ========================================= PORT_program.c =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: PORT
// Layer: MCAL
// Description: This file contains the implementation of PORT functions
// =================================================================================================

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "PORT_interface.h"
#include "PORT_config.h"
#include "PORT_private.h"

void PORT_voidInit()
{

    DDRB_REG = PORTB_DIR;
    DDRC_REG = PORTC_DIR;
    DDRD_REG = PORTD_DIR;

    PORTB_REG = PORTB_INIT_VAL;
    PORTC_REG = PORTC_INIT_VAL;
    PORTD_REG = PORTD_INIT_VAL;
}