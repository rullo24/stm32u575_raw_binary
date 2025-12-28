#ifndef RCC_H
#define RCC_H

#include <stdint.h>

// RCC base address (reference manual page 147) - non-secure boundary address
#define RCC_BASE_ADDR 0x46020C00UL

// RCC register offsets
#define RCC_AHB2ENR1_OFFSET 0x8C

// RCC register access macros
#define RCC_AHB2ENR1 (*(volatile uint32_t*)(RCC_BASE_ADDR + RCC_AHB2ENR1_OFFSET))

// RCC AHB2ENR1 bit positions for GPIO ports
#define RCC_AHB2ENR1_GPIOB_EN (1 << 1)

static inline void rccEnableGpioB(void) {
    RCC_AHB2ENR1 |= RCC_AHB2ENR1_GPIOB_EN;
}

#endif