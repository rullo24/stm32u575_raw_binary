#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>

/// === GPIO REGISTERS REQUIRED (reference manual page 642) === ///
typedef struct {
    volatile uint32_t moder; // MODER - offset=0x00: Pin mode (input/output/alternate/analog)
    volatile uint32_t otyper; // OTYPER - offset=0x04: Output type (push-pull/open-drain)
    volatile uint32_t ospeedr; // OSPEEDR - offset=0x08: Output speed (low/medium/high/very high)
    volatile uint32_t pupdr; // PUPDR - offset=0x0C: Pull-up/pull-down resistor configuration
    volatile uint32_t idr; // IDR - offset=0x10: Input data register (not used)
    volatile uint32_t odr; // ODR - offset=0x14: Output data register (not used)
    volatile uint32_t bsrr; // BSRR - offset=0x18: Bit set/reset register (atomic pin control)
    volatile uint32_t lckr; // LCKR - offset=0x1C: Lock register (not used)
    volatile uint32_t afrl; // AFRL - offset=0x20: Alternate function low register (not used)
    volatile uint32_t afrh; // AFRL - offset=0x24: Alternate function high register (not used)
    volatile uint32_t brr; // BRR - offset=0x28: Bit reset register (not used)
    volatile uint32_t hslvcr; // HSLVCR - offset=0x2C: High speed level voltage change rate register (not used)
    volatile uint32_t seccfgr; // SECCFGR - offset=0x30: Security configuration register (not used)
} __attribute__((aligned(4))) GPIO_REGISTER_t;

#endif