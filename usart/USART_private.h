// ========================================= USART_private.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: USART
// Layer: MCAL
// Description: This file contains the private definitions and macros for USART peripheral
// =================================================================================================

#ifndef USART_PRIVATE_H
#define USART_PRIVATE_H

#define SREG *((volatile u8 *)0x5F) // we will use the 7th bit to read global interrupt status (Enabled/Disabled)

#define UDR0 *((volatile u8 *)0xC6) // USART I/O Data Register

#define UCSR0A *((volatile u8 *)0xC0) // USART Control and Status Register A

#define UCSR0B *((volatile u8 *)0xC1) // USART Control and Status Register B

#define UCSR0C *((volatile u8 *)0xC2) // USART Control and Status Register C

#define UBRR0L *((volatile u8 *)0xC4) // USART Baud Rate Registers
#define UBRR0H *((volatile u8 *)0xC5)

/* Bits defining */

/* ------- SREG ------- */
#define GIE 7 // globale interrupt enable

/* ------- UCSR0A ------- */
#define RXC0 7  // USART Receive Complete flag
#define TXC0 6  // USART Transmit Complete flag
#define UDRE0 5 // USART Data Register Empty flag
#define FE0 4   // Frame Error flag
#define DOR0 3  // Data OverRun flag
#define UPE0 2  // USART Parity Error flag

#define U2X0 1  // Double the USART Transmission Speed
#define MPCM0 0 // Multi-processor Communication Mode

/* ------- UCSR0B ------- */
#define RXCIE0 7 // RX Complete Interrupt Enable
#define TXCIE0 6 // TX Complete Interrupt Enable
#define UDRIE0 5 // USART Data Register Empty Interrupt Enable
#define RXEN0 4  // Receiver Enable
#define TXEN0 3  // Transmitter Enable

#define UCSZ02 2 // Character Size Bit 2

#define RXB80 1 // Receive Data Bit 8
#define TXB80 0 // Transmit Data Bit 8

/* ------- UCSR0C ------- */
#define UMSEL01 7 // USART Mode Select
#define UMSEL00 6 // USART Mode Select

#define UPM01 5 // Parity Mode
#define UPM00 4 // Parity Mode

#define USBS0 3  // Stop Bit Select
#define UCSZ01 2 // Character Size Bit 1
#define UCSZ00 1 // Character Size Bit 0
#define UCPOL0 0 // Clock Polarity

/* Helper Functions */
static u16 calcBaudRateReg(u64 Copy_u64BaudRate);
static void applyUSARTMode(USART_Mode Copy_enuMode);
#endif /* USART_PRIVATE_H */