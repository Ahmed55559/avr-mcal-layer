// ========================================= ADC_interface.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: ADC
// Layer: MCAL
// Description: This file contains the interface definitions for the ADC peripheraL
// =================================================================================================

#ifndef ADC_INTERFACE_H
#define ADC_INTERFACE_H

typedef enum
{
    ADC0 = 0,
    ADC1,
    ADC2,
    ADC3,
    ADC4,
    ADC5,
    ADC6,
    ADC7,
    INTERNAL_TEMPRATURE_SENSOR
} ADC_channel;

typedef enum
{
    ADC_OK,
    ADC_DISABLED,
    INVALID_CHANNEL,
    INVALID_REF,
    INVALID_TRIGGER,
    INVALID_ADJ,
    NULL_POINTER,
    ADC_NOK,
    GLOBAL_INTERRUPT_DISABLED,
    ADC_BUSY,
    AUTO_TRIGGER_DISABLED
} ADC_ErrorStatus;

typedef enum
{
    AREF = 0,
    AVCC = 1,
    INTERNAL_REF = 3,
} ADC_VoltageRef;

typedef enum
{
    FREE_RUNNING = 0,
    ANALOG_COMP,
    EXTERNAL_INT0,
    TC0_COMPARE_MATCHA,
    TC0_OVERFLOW,
    TC1_COMPARE_MATCHB,
    TC1_OVERFLOW,
    TC1_CAPTURE_EVENT
} ADC_ConversionTrigger;

typedef enum
{
    RIGHT_ADJ = 0,
    LEFT_ADJ
} ADC_ReadAdjustment;

typedef enum
{
    ADC_IDLE_STATE,
    ADC_BUSY_STATE
} ADC_State;

void ADC_voidInit(void);
void ADC_voidEnable(void);
void ADC_voidDisable(void);
ADC_ErrorStatus ADC_enuReadSync(ADC_channel channel, u16 *result);
ADC_ErrorStatus ADC_enuReadAsync(ADC_channel channel, u16 *result, void (*notification_func)(void));
ADC_ErrorStatus ADC_enuChangeVoltageRef(ADC_VoltageRef newRef);
ADC_ErrorStatus ADC_enuChangeTrigger(ADC_ConversionTrigger newTrigger);
ADC_ErrorStatus ADC_enuChangeAdjustment(ADC_ReadAdjustment newAdj);

#endif /* ADC_INTERFACE_H */