// ========================================= EXTI_interface.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: EXTI
// Layer: MCAL
// Description: This file contains the interface definitions for interrupt peripheral (external & pin change)
// =================================================================================================

#ifndef EXTI_INTERFACE_H
#define EXTI_INTERFACE_H

typedef enum
{
    EXTI_line_0 = 0,
    EXTI_line_1 = 1
} EXTI_line;

typedef enum
{
    LOW_LEVEL,
    ANY_CHANGE,
    FALLING_EDGE,
    RISING_EDGE
} EXTI_sensing;

typedef enum
{
    PC_line0 = 0,
    PC_line1 = 1,
    PC_line2 = 2
} PC_line;

typedef enum
{
    PC_pin0,
    PC_pin1,
    PC_pin2,
    PC_pin3,
    PC_pin4,
    PC_pin5,
    PC_pin6,
    PC_pin7,
} PC_pin;

void GIE_voidEnable(void);
void GIE_voidDisable(void);

void EXTI_voidInit(void);
void EXTI_voidEnable(EXTI_line line);
void EXTI_voidDisable(EXTI_line line);
void EXTI_voidSetCallback(EXTI_line line, void (*isr)(void));
void EXTI_voidChangeSensingMode(EXTI_line line, EXTI_sensing mode);
void EXTI_voidClearFlag(EXTI_line line);

void PC_voidInit(void);
void PC_voidEnableInterruptGroup(PC_line line);
void PC_voidDisableInterruptGroup(PC_line line);
void PC_voidEnablePin(PC_line line, PC_pin pin);
void PC_voidSetCallback(PC_line line, void (*isr)(void));
void PC_voidClearFlag(PC_line line);

#endif /* EXTI_INTERFACE_H */