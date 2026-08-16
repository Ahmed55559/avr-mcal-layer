// ========================================= PORT_private.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: PORT
// Layer: MCAL
// Description: This file contains the private definitions and macros for PORT peripheral
// =================================================================================================

#ifndef PORT_PRIVATE_H
#define PORT_PRIVATE_H

/* PORT Registers */
#define PORTD_REG *((volatile u8 *)0x2B)
#define DDRD_REG *((volatile u8 *)0x2A)
#define PIND_REG *((volatile u8 *)0x29)
#define PORTC_REG *((volatile u8 *)0x28)
#define DDRC_REG *((volatile u8 *)0x27)
#define PINC_REG *((volatile u8 *)0x26)
#define PORTB_REG *((volatile u8 *)0x25)
#define DDRB_REG *((volatile u8 *)0x24)
#define PINB_REG *((volatile u8 *)0x23)

#define concatenate(b7, b6, b5, b4, b3, b2, b1, b0) concatenate_helper(b7, b6, b5, b4, b3, b2, b1, b0)
#define concatenate_helper(b7, b6, b5, b4, b3, b2, b1, b0) 0b##b7##b6##b5##b4##b3##b2##b1##b0

#define PORTB_DIR concatenate(PORTB_PIN7_DIR, PORTB_PIN6_DIR, PORTB_PIN5_DIR, PORTB_PIN4_DIR, PORTB_PIN3_DIR, PORTB_PIN2_DIR, PORTB_PIN1_DIR, PORTB_PIN0_DIR)
#define PORTC_DIR concatenate(PORTC_PIN7_DIR, PORTC_PIN6_DIR, PORTC_PIN5_DIR, PORTC_PIN4_DIR, PORTC_PIN3_DIR, PORTC_PIN2_DIR, PORTC_PIN1_DIR, PORTC_PIN0_DIR)
#define PORTD_DIR concatenate(PORTD_PIN7_DIR, PORTD_PIN6_DIR, PORTD_PIN5_DIR, PORTD_PIN4_DIR, PORTD_PIN3_DIR, PORTD_PIN2_DIR, PORTD_PIN1_DIR, PORTD_PIN0_DIR)

#define PORTB_INIT_VAL concatenate(PORTB_PIN7_INIT_VAL, PORTB_PIN6_INIT_VAL, PORTB_PIN5_INIT_VAL, PORTB_PIN4_INIT_VAL, PORTB_PIN3_INIT_VAL, PORTB_PIN2_INIT_VAL, PORTB_PIN1_INIT_VAL, PORTB_PIN0_INIT_VAL)
#define PORTC_INIT_VAL concatenate(PORTC_PIN7_INIT_VAL, PORTC_PIN6_INIT_VAL, PORTC_PIN5_INIT_VAL, PORTC_PIN4_INIT_VAL, PORTC_PIN3_INIT_VAL, PORTC_PIN2_INIT_VAL, PORTC_PIN1_INIT_VAL, PORTC_PIN0_INIT_VAL)
#define PORTD_INIT_VAL concatenate(PORTD_PIN7_INIT_VAL, PORTD_PIN6_INIT_VAL, PORTD_PIN5_INIT_VAL, PORTD_PIN4_INIT_VAL, PORTD_PIN3_INIT_VAL, PORTD_PIN2_INIT_VAL, PORTD_PIN1_INIT_VAL, PORTD_PIN0_INIT_VAL)

#endif /* PORT_PRIVATE_H */