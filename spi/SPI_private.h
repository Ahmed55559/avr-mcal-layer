// ========================================= SPI_private.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: SPI
// Layer: MCAL
// Description: This file contains the private definitions and macros for SPI peripheral
// =================================================================================================

#ifndef SPI_PRIVATE_H
#define SPI_PRIVATE_H

#define SREG *((volatile u8 *)0x5F) // we will use the 7th bit to read global interrupt status (Enabled/Disabled)
#define SPCR *((volatile u8 *)0x4C) // SPI control register
#define SPSR *((volatile u8 *)0x4D) // SPI status register
#define SPDR *((volatile u8 *)0x4E) // SPI data register

/* Bits defining */

/* SREG */
#define GIE 7 // globale interrupt enable

/* SPCR */
#define SPIE 7
#define SPE 6
#define DORD 5
#define MSTR 4
#define CPOL 3
#define CPHA 2
#define SPR1 1
#define SPR0 0

/* SPSR */
#define SPIF 7
#define WCOL 6
#define SPI2X 0

#endif /* SPI_PRIVATE_H */