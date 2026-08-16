# 2. `avr-mcal-drivers/README.md`

This one should be the **strongest driver README**, because this is where most of your low-level work lives.

```markdown
# AVR MCAL Drivers

Register-level peripheral drivers for AVR 8-bit microcontrollers.

This repository contains the MCAL (Microcontroller Abstraction Layer) developed while building my embedded-systems software stack.

The drivers interface directly with MCU peripheral registers and expose higher-level APIs to upper software layers.

## Drivers

| Driver | Peripheral |
|---|---|
| GPIO | Digital I/O |
| PORT | Port configuration |
| ADC | Analog-to-Digital Converter |
| EXTI | External Interrupts |
| SPI | Serial Peripheral Interface |
| TWI | Two-Wire Interface / I²C |
| USART | Universal Synchronous/Asynchronous Receiver Transmitter |
| TIMER0 | 8-bit Timer/Counter 0 |
| TIMER1 | 16-bit Timer/Counter 1 |
| TIMER2 | 8-bit Timer/Counter 2 |
| WDT | Watchdog Timer |

## Architecture

```text
Application
     │
     ▼
HAL / Higher-Level Drivers
     │
     ▼
MCAL Drivers
     │
     ├── GPIO
     ├── ADC
     ├── EXTI
     ├── SPI
     ├── TWI
     ├── USART
     ├── TIMER0
     ├── TIMER1
     ├── TIMER2
     └── WDT
     │
     ▼
AVR Peripheral Registers
     │
     ▼
Hardware
````

## Driver Structure

Each driver follows a layered structure where applicable:

```text
<DRIVER>_config.h
<DRIVER>_interface.h
<DRIVER>_private.h
<DRIVER>_program.c
```

### Configuration

`*_config.h`

Contains compile-time configuration options.

### Interface

`*_interface.h`

Contains the public API exposed to other software layers.

### Private

`*_private.h`

Contains driver-internal definitions and implementation details.

### Implementation

`*_program.c`

Contains the driver implementation and direct peripheral manipulation.

## Design Approach

The drivers are implemented around the AVR hardware rather than relying on Arduino abstractions.

The goal is to understand and control the MCU at the register level while maintaining a reusable software interface.

The project emphasizes:

* Register-level programming
* Hardware abstraction
* Modular driver design
* Compile-time configuration
* Separation of interface and implementation
* Reusable APIs
* Interrupt-driven peripherals where applicable

## Target

Primary target:

* ATmega328P
* AVR 8-bit architecture
* C

The drivers are intended to be portable to closely related AVR devices with appropriate register definitions and configuration changes.

## Project Status

**Complete / maintained as a foundation for further embedded development.**

The driver layer was developed before moving into RTOS and kernel development.

## Next Layer

These MCAL drivers form the foundation for the higher-level HAL drivers in [`avr-hal-drivers`](../avr-hal-drivers).

They also provide the hardware-level foundation for my current project, **ARTOS**, a from-scratch RTOS for AVR.

````

---
