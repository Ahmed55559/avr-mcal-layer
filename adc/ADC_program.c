// ========================================= ADC_program.c =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: ADC
// Layer: MCAL
// Description: This file contains the implementation of ADC functions
// =================================================================================================

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "ADC_interface.h"
#include "ADC_config.h"
#include "ADC_private.h"

static ADC_State ADC_CurrentState = ADC_IDLE_STATE;

void ADC_voidInit()
{
    /* Prescaler */
    CLR_BIT(ADCSRA, ADPS0);
    CLR_BIT(ADCSRA, ADPS1);
    CLR_BIT(ADCSRA, ADPS2);
    ADCSRA |= ADC_PRESCALER;

    // Adjustment
    CLR_BIT(ADMUX, ADLAR);
    ADMUX |= (ADC_INITIAL_READING_ADJUSTMENT << ADLAR);

    // Voltage reference
    CLR_BIT(ADMUX, REFS0);
    CLR_BIT(ADMUX, REFS1);
    ADMUX |= (ADC_INITIAL_VOLTAGE_REF << REFS0);

    // Enable ADC peripheral
    CLR_BIT(ADCSRA, ADEN);
    ADCSRA |= (ADC_INITIAL_STATE << ADEN);
}

void ADC_voidEnable(void)
{
    SET_BIT(ADCSRA, ADEN);
}
void ADC_voidDisable(void)
{
    CLR_BIT(ADCSRA, ADEN);
}

ADC_ErrorStatus ADC_enuReadSync(ADC_channel channel, u16 *result)
{
    if (!GET_BIT(ADCSRA, ADEN))
        return ADC_DISABLED;

    if (channel > INTERNAL_TEMPRATURE_SENSOR || channel < ADC0)
        return INVALID_CHANNEL;
    if (result == NULL)
        return NULL_POINTER;

    if (ADC_CurrentState == ADC_BUSY_STATE)
        return ADC_BUSY;

    ADC_CurrentState = ADC_BUSY_STATE;

    // change the channel
    ADMUX = (ADMUX & ~0x0F) | channel;

    // start a conversion
    SET_BIT(ADCSRA, ADSC);

    // wait until conversion is done (blocking | polling)
    while (GET_BIT(ADCSRA, ADSC))
        ;

    u8 low = ADCL;
    u8 high = ADCH;
    if (GET_BIT(ADMUX, ADLAR))
    {
        /* Left adjustent handling */
        *result = high;
    }
    else
    {
        /* Right adjustment handling */
        *result = (high << 8) | low;
    }
    ADC_CurrentState = ADC_IDLE_STATE;
    return ADC_OK;
}

static u16 *Global_u16AsyncResult = NULL;
static void (*Global_AsyncNotificationFunc)(void) = NULL;
ADC_ErrorStatus ADC_enuReadAsync(ADC_channel channel, u16 *result, void (*notification_func)(void))
{
    if (!GET_BIT(ADCSRA, ADEN))
        return ADC_DISABLED;
    if (channel > INTERNAL_TEMPRATURE_SENSOR || channel < ADC0)
        return INVALID_CHANNEL;
    if (result == NULL)
        return NULL_POINTER;
    if (!GET_BIT(SREG, 7))
        return GLOBAL_INTERRUPT_DISABLED;
    if (ADC_CurrentState == ADC_BUSY_STATE)
        return ADC_BUSY;

    ADC_CurrentState = ADC_BUSY_STATE;

    // setting the global values to be used by ADC ISR
    Global_u16AsyncResult = result;
    Global_AsyncNotificationFunc = notification_func;

    // Enable ADC interrupt
    SET_BIT(ADCSRA, ADIE);

    // change the channel
    ADMUX = (ADMUX & ~0x0F) | channel;

    // start a conversion
    SET_BIT(ADCSRA, ADSC);

    return ADC_OK;
}
void __vector_21(void) __attribute__((signal));
void __vector_21(void)
{
    u8 low = ADCL;
    u8 high = ADCH;
    if (GET_BIT(ADMUX, ADLAR))
    {
        /* Left adjustent handling */
        *Global_u16AsyncResult = ((u16)high << 2) | (low >> 6);
    }
    else
    {
        /* Right adjustment handling */
        *Global_u16AsyncResult = ((u16)high << 8) | low;
    }

    CLR_BIT(ADCSRA, ADIE);
    ADC_CurrentState = ADC_IDLE_STATE;

    if (Global_AsyncNotificationFunc != NULL)
        Global_AsyncNotificationFunc();
}

ADC_ErrorStatus ADC_enuChangeVoltageRef(ADC_VoltageRef newRef)
{
    if (newRef > INTERNAL_REF || newRef < AREF)
        return INVALID_REF;

    CLR_BIT(ADMUX, REFS0);
    CLR_BIT(ADMUX, REFS1);
    ADMUX |= (newRef << REFS0);

    return ADC_OK;
}

ADC_ErrorStatus ADC_enuChangeTrigger(ADC_ConversionTrigger newTrigger)
{
    if (newTrigger > TC1_CAPTURE_EVENT || newTrigger < FREE_RUNNING)
        return INVALID_TRIGGER;
    if (!GET_BIT(ADCSRA, ADATE))
        return AUTO_TRIGGER_DISABLED;

    // disabling ADEN so trigger changing not trigger a conversion
    u8 adcWasEnabled = GET_BIT(ADCSRA, ADEN);
    CLR_BIT(ADCSRA, ADEN);

    // change trigger
    CLR_BIT(ADCSRB, ADTS0);
    CLR_BIT(ADCSRB, ADTS1);
    CLR_BIT(ADCSRB, ADTS2);
    ADCSRB |= newTrigger;

    if (adcWasEnabled)
        SET_BIT(ADCSRA, ADEN);

    return ADC_OK;
}
ADC_ErrorStatus ADC_enuChangeAdjustment(ADC_ReadAdjustment newAdj)
{
    if (newAdj != RIGHT_ADJ && newAdj != LEFT_ADJ)
        return INVALID_ADJ;

    // change adj
    if (newAdj == LEFT_ADJ)
        SET_BIT(ADMUX, ADLAR);
    else
        CLR_BIT(ADMUX, ADLAR);

    return ADC_OK;
}