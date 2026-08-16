// ========================================= TWI_config.h =========================================
// Auther: Eng. Ahmed Ashraf
// SWC: TWI
// Layer: MCAL
// Description: This file contains the configuration information of TWI peripheral
// =================================================================================================

#ifndef TWI_CONFIG_H
#define TWI_CONFIG_H

#define DISABLE 0
#define ENABLE 1

#define SYS_FREQ 16000000

#define TWI_ROLE TWI_SLAVE
#define TWI_SCL_FREQ 100000
#define TWI_SLAVE_ADDRESS 0x11 // Don't use addresses of form (1111xxx)
#define TWI_SLAVE_GENERAL_CALL ENABLE
#define TWI_SYNC_MODE TWI_SYNC

#endif /* TWI_CONFIG_H */