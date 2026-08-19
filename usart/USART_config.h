// ========================================= USART_config.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: USART
// Layer: MCAL
// Description: This file contains the configuration information of USART peripheral
// =================================================================================================

#ifndef USART_CONFIG_H
#define USART_CONFIG_H

#define DISABLE 0
#define ENABLE 1

#define SYSTEM_CLK_FREQUENCY 16000000

#define INITIAL_USART_MODE USART_ASYNC_NORMAL_SPEED
#define INITIAL_USART_BAUDRATE 9600

#define INITIAL_USART_TX_ENABLE ENABLE
#define INITIAL_USART_RX_ENABLE ENABLE

#define INITIAL_USART_DATA_BITS USART_DATA_BITS_8
#define INITIAL_USART_PARITY_MODE USART_PARITY_DISABLED
#define INITIAL_USART_STOP_BITS USART_STOP_BITS_2
#define INITIAL_USART_CLK_POL USART_CLK_TRANS_RISING_REC_FALLING

#endif /* USART_CONFIG_H */