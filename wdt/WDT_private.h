// ========================================= WDT_private.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: WDT
// Layer: MCAL
// Description: This file contains the private definitions and macros for WDT peripheral
// =================================================================================================

#ifndef WDT_PRIVATE_H
#define WDT_PRIVATE_H

#define SREG *((volatile u8 *)0x5F)   // we will use the 7th bit to read global interrupt status (Enabled/Disabled)
#define MCUSR *((volatile u8 *)0x55)  // MCU Status Register
#define WDTCSR *((volatile u8 *)0x60) // Watchdog Timer Control Register

/* Bits defining */

/* SREG */
#define GIE 7 // globale interrupt enable

/* MCUSR */
#define PORF 0  // Power-on Reset Flag
#define EXTRF 1 //  External Reset Flag
#define BORF 2  // Brown-out Reset Flag
#define WDRF 3  // Watchdog System Reset Flag

/* WDTCSR */
#define WDP0 0 // clock prescaler
#define WDP1 1 // clock prescaler
#define WDP2 2 // clock prescaler
#define WDE 3  // Watchdog System Reset Enable
#define WDCE 4 // Watchdog Change Enable
#define WDP3 5 // clock prescaler
#define WDIE 6 // Watchdog Interrupt Enable
#define WDIF 7 // Watchdog Interrupt Flag

/* helper */
static void WDT_voidApplyConfiguration(void);
#endif /* WDT_PRIVATE_H */