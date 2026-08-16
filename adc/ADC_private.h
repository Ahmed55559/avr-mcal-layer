// ========================================= ADC_private.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: ADC
// Layer: MCAL
// Description: This file contains the private definitions and macros for ADC peripheral
// =================================================================================================

#ifndef ADC_PRIVATE_H
#define ADC_PRIVATE_H

/* Eternal Interrupt Registers*/
#define SREG *((volatile u8 *)0x5F) // we will use the 7th bit to enable/disable global interrupt

#define ADMUX *((volatile u8 *)0x7C)  // Specify channel, voltage ref, and adjustment
#define ADCSRA *((volatile u8 *)0x7A) // enable/disable ADC, start conversion, autoTrigger, interrupt enable/flag , prescaler selection
#define ADCSRB *((volatile u8 *)0x7B) // specify trigger source

/* ADC digital output Registers */
#define ADCL *((volatile u8 *)0x78)
#define ADCH *((volatile u8 *)0x79)

/* Bits defining */

/* ADMUX */
#define REFS1 7 // Voltage Ref
#define REFS0 6 // Voltage Ref
#define ADLAR 5 // ADJ configuration 0->Right
// MUX0->3 will be calculated

/* ADCSRA */
#define ADEN 7  // ADC enable
#define ADSC 6  // start conversion
#define ADATE 5 // auto-trigger enable
#define ADIF 4  // ADC interrupt flag
#define ADIE 3  // ADC interrupt enable
#define ADPS2 2 // prescaler bit 3
#define ADPS1 1 // prescaler bit 2
#define ADPS0 0 // prescaler bit 1

/* ADCSRB */
#define ADTS2 2 // trigger source bit 3
#define ADTS1 1 // trigger source bit 2
#define ADTS0 0 // trigger source bit 1

#endif /* ADC_PRIVATE_H */