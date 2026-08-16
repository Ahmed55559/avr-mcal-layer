// ========================================= SPI_config.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: SPI
// Layer: MCAL
// Description: This file contains the configuration information of SPI peripheral
// =================================================================================================

#ifndef SPI_CONFIG_H
#define SPI_CONFIG_H
typedef enum
{
    DISABLE,
    ENABLE
} SPI_State;

#define INITIAL_SPI_STATE ENABLE
#define INITIAL_SPI_ROLE SPI_MASTER
#define INITIAL_SPI_INT_STATE ENABLE
#define INITIAL_SPI_CLK SPI_CLK_DIV_4
#define INITIAL_SPI_CLK_POL SPI_LEADING_RISING_TRAILING_FALLING
#define INITIAL_SPI_CLK_PHASE SPI_LEADING_SAMPLE_TRAILING_SETUP
#define INITIAL_SPI_DATA_ORDER SPI_LSB_FIRST

#endif /* SPI_CONFIG_H */