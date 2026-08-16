// ========================================= PORT_private.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: PORT
// Layer: MCAL
// Description: This file contains the private definitions and macros for PORT peripheral
// =================================================================================================

#ifndef PORT_PRIVATE_H
#define PORT_PRIVATE_H

/* External Interrupt Registers*/
#define SREG *((volatile u8 *)0x5F)  // we will use the 7th bit to enable/disable global interrupt
#define EIMSK *((volatile u8 *)0x3D) // External interrupt mask register (responsibility: enable certain external interrupt line)
#define EICRA *((volatile u8 *)0x69) // External interrupt control regiser (responsibility: change sensing mode for ext interrupt)
#define EIFR *((volatile u8 *)0x3C)  // External interrupt flag register (responsibility: detects interrupt firing)

/* Pin change interrupt Registers */
#define PCICR *((volatile u8 *)0x68) // pin change interrupt control register (responsibility: enable certain pin change pin group)

/* pin change mask registers: enable certain pins of a group */
#define PCMSK2 *((volatile u8 *)0x6D)
#define PCMSK1 *((volatile u8 *)0x6C)
#define PCMSK0 *((volatile u8 *)0x6B)

#define PCIFR *((volatile u8 *)0x3B) // pin change interrupt flag register (responsibility: detects interrupt firing)

#endif /* PORT_PRIVATE_H */