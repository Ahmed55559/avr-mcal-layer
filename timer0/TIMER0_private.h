// ========================================= TIMER0_private.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: TIMER0
// Layer: MCAL
// Description: This file contains the private definitions and macros for TIMER0 peripheral
// =================================================================================================

#ifndef TIMER0_PRIVATE_H
#define TIMER0_PRIVATE_H

#define SREG *((volatile u8 *)0x5F) // we will use the 7th bit to read global interrupt status (Enabled/Disabled)

/* Timer Control Registers */
#define TCCR0A *((volatile u8 *)0x44) // specify Waveform Generation (2 bits out of 3 bits) , Control OC0A,OC0B pins output mode
#define TCCR0B *((volatile u8 *)0x45) // specify clock source, specify Waveform Generation (1 out of 3 bits) , force compare match A,B

/* Timer Values Registers */
#define TCNT0 *((volatile u8 *)0x46) // hold counter value
#define OCR0A *((volatile u8 *)0x47) // hold compare unit A value
#define OCR0B *((volatile u8 *)0x48) // hold compare unit B value

/* Interrupts */
#define TIMSK0 *((volatile u8 *)0x6E) // enable/disable Timer interrupts
#define TIFR0 *((volatile u8 *)0x35)  // hold Timer interrupts' flags status

/* Bits defining */

/* SREG */
#define GIE 7 // globale interrupt enable

/* PRR */
#define PRTIM0 5 // shutdown / turn on prephiral

/* TCCR0A */
#define WGM00 0  // Wave generation mode bit 0
#define WGM01 1  // Wave generation mode bit 1
#define COM0B0 4 // control output mode timer 0 pin OC0B
#define COM0B1 5 // control output mode timer 0 pin OC0B
#define COM0A0 6 // control output mode timer 0 pin OC0A
#define COM0A1 7 // control output mode timer 0 pin OC0A

/* TCCR0B */
#define CS00 0  // clock source control
#define CS01 1  // clock source control
#define CS02 2  // clock source control
#define WGM02 3 // Wave generation mode bit 2
#define FOC0B 6 // Force compare match B
#define FOC0A 7 // Force compare match A

/* TIMSK0 */
#define TOIE00 0 // Timer overflow interrupt enable
#define OCIE0A 1 // Timer compare match A interrupt enable
#define OCIE0B 2 // Timer compare match B interrupt enable

/* TIFR0 */
#define TOV0 0  // Timer overflow interrupt flag
#define OCF0A 1 // Timer compare match A interrupt flag
#define OCF0B 2 // Timer compare match B interrupt flag

#endif /* TIMER0_PRIVATE_H */