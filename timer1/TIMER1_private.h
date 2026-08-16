// ========================================= TIMER1_private.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: TIMER1
// Layer: MCAL
// Description: This file contains the private definitions and macros for TIMER1 peripheral
// =================================================================================================

#ifndef TIMER1_PRIVATE_H
#define TIMER1_PRIVATE_H

#define SREG *((volatile u8 *)0x5F) // we will use the 7th bit to read global interrupt status (Enabled/Disabled)
#define PRR *((volatile u8 *)0x64)  // Power Reduction Register, to shut down/ turn on the Timer

/* Timer Control Registers */
#define TCCR1A *((volatile u8 *)0x80) // specify Waveform Generation (2 bits out of 4 bits) , Control OC1A,OC1B pins output mode
#define TCCR1B *((volatile u8 *)0x81) // specify clock source, specify Waveform Generation (2 out of 4 bits) , ICU noise cancler enable, edge select
#define TCCR1C *((volatile u8 *)0x82) // force compare match

/* Timer Values Registers */
#define TCNT1L *((volatile u8 *)0x84) // hold counter value (Low bits)
#define TCNT1H *((volatile u8 *)0x85) // hold counter value (High bits)

#define OCR1AL *((volatile u8 *)0x88) // hold compare unit A value (Low bits)
#define OCR1AH *((volatile u8 *)0x89) // hold compare unit A value (High bits)

#define OCR1BL *((volatile u8 *)0x8A) // hold compare unit B value (Low bits)
#define OCR1BH *((volatile u8 *)0x8B) // hold compare unit B value (High bits)

#define ICR1L *((volatile u8 *)0x86) // hold Input capture unit value (Low bits)
#define ICR1H *((volatile u8 *)0x87) // hold Input capture unit value (High bits)

/* Interrupts */
#define TIMSK1 *((volatile u8 *)0x6F) // enable/disable Timer interrupts
#define TIFR1 *((volatile u8 *)0x36)  // hold Timer interrupts' flags status

/* Bits defining */

/* SREG */
#define GIE 7 // globale interrupt enable

/* PRR */
#define PRTIM1 3 // shutdown / turn on prephiral

/* TCCR1A */
#define WGM10 0  // Wave generation mode bit 0
#define WGM11 1  // Wave generation mode bit 1
#define COM1B0 4 // control output mode timer 0 pin OC1B
#define COM1B1 5 // control output mode timer 0 pin OC1B
#define COM1A0 6 // control output mode timer 0 pin OC1A
#define COM1A1 7 // control output mode timer 0 pin OC1A

/* TCCR1B */
#define CS10 0  // clock source control
#define CS11 1  // clock source control
#define CS12 2  // clock source control
#define WGM12 3 // Wave generation mode bit 2
#define WGM13 4 // Wave generation mode bit 3
#define ICES1 6 //  Input Capture Edge Select
#define ICNC1 7 //  Input Capture Noise Canceler

/* TCCR1C */
#define FOC1B 6 // Force compare match B
#define FOC1A 7 // Force compare match A

/* TIMSK1 */
#define TOIE1 0  // Timer overflow interrupt enable
#define OCIE1A 1 // Timer compare match A interrupt enable
#define OCIE1B 2 // Timer compare match B interrupt enable
#define ICIE1 5  // Input capture interrupt enable

/* TIFR1 */
#define TOV1 0  // Timer overflow interrupt flag
#define OCF1A 1 // Timer compare match A interrupt flag
#define OCF1B 2 // Timer compare match B interrupt flag
#define ICF1 5  // Input capture interrupt flag

#endif /* TIMER1_PRIVATE_H */