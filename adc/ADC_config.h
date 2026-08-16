// ========================================= ADC_config.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: ADC
// Layer: MCAL
// Description: This file contains the configuration information of ADC peripheral
// =================================================================================================

#ifndef ADC_CONFIG_H
#define ADC_CONFIG_H

typedef enum
{
    DISABLE = 0,
    ENABLE = 1
} INITIAL_VALUE;

typedef enum
{
    DIV_ON_2 = 1,
    DIV_ON_4,
    DIV_ON_8,
    DIV_ON_16,
    DIV_ON_32,
    DIV_ON_64,
    DIV_ON_128 = 7,
} PRESCALER;

/* ADC peripheral after initialization */
#define ADC_INITIAL_STATE ENABLE

/* Prescaler */
#define ADC_PRESCALER DIV_ON_32

/* Adjustment */
#define ADC_INITIAL_READING_ADJUSTMENT RIGHT_ADJ

/* Voltage Reference */
#define ADC_INITIAL_VOLTAGE_REF AVCC

#endif /* ADC_CONFIG_H */