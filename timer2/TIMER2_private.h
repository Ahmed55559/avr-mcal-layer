// ========================================= TIMER2_private.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: TIMER2
// Layer: MCAL
// Description: This file contains the private definitions and macros for TIMER2 peripheral
// =================================================================================================

#ifndef TIMER2_PRIVATE_H
#define TIMER2_PRIVATE_H

#define SREG *((volatile u8 *)0x5F) // we will use the 7th bit to read global interrupt status (Enabled/Disabled)
#define PRR *((volatile u8 *)0x64)  // Power Reduction Register, to shut down/ turn on the Timer

/* Timer Control Registers */
#define TCCR2A *((volatile u8 *)0xB0) // specify Waveform Generation (2 bits out of 3 bits) , Control OC2A,OC2B pins output mode
#define TCCR2B *((volatile u8 *)0xB1) // specify clock source, specify Waveform Generation (1 out of 3 bits) , force compare match A,B

/* Timer Values Registers */
#define TCNT2 *((volatile u8 *)0xB2) // hold counter value
#define OCR2A *((volatile u8 *)0xB3) // hold compare unit A value
#define OCR2B *((volatile u8 *)0xB4) // hold compare unit B value

/* Interrupts */
#define TIMSK2 *((volatile u8 *)0x70) // enable/disable Timer interrupts
#define TIFR2 *((volatile u8 *)0x37)  // hold Timer interrupts' flags status

/* Asynchronous */
#define ASSR *((volatile u8 *)0xB6) // Enable External clock input, Enable aysnc mode, update busy flags

/* Bits defining */

/* SREG */
#define GIE 7 // globale interrupt enable

/* PRR */
#define PRTIM2 6 // shutdown / turn on prephiral

/* TCCR2A */
#define WGM20 0  // Wave generation mode bit 0
#define WGM21 1  // Wave generation mode bit 1
#define COM2B0 4 // control output mode timer 0 pin OC2B
#define COM2B1 5 // control output mode timer 0 pin OC2B
#define COM2A0 6 // control output mode timer 0 pin OC2A
#define COM2A1 7 // control output mode timer 0 pin OC2A

/* TCCR2B */
#define CS20 0  // clock source control
#define CS21 1  // clock source control
#define CS22 2  // clock source control
#define WGM22 3 // Wave generation mode bit 2
#define FOC2B 6 // Force compare match B
#define FOC2A 7 // Force compare match A

/* TIMSK2 */
#define TOIE2 0  // Timer overflow interrupt enable
#define OCIE2A 1 // Timer compare match A interrupt enable
#define OCIE2B 2 // Timer compare match B interrupt enable

/* TIFR2 */
#define TOV2 0  // Timer overflow interrupt flag
#define OCF2A 1 // Timer compare match A interrupt flag
#define OCF2B 2 // Timer compare match B interrupt flag

/* ASSR */
#define EXCLK 6   // enable external clock input buffer
#define AS2 5     // change timing mode 1-> Async
#define TCN2UB 4  // counter update busy
#define OCR2AUB 3 // output compare A register update busy
#define OCR2BUB 2 // output compare B register update busy
#define TCR2AUB 1 // control register A update busy
#define TCR2BUB 0 // control register B update busy

/* Macros */
#define T2_WAIT() while (ASSR & ((1 << TCN2UB) | (1 << OCR2AUB) | (1 << OCR2BUB) | (1 << TCR2AUB) | (1 << TCR2BUB)))

/* helper function */
static void syncAsyncCompitability(void);
#endif /* TIMER2_PRIVATE_H */