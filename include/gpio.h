#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

// GPIO base addresses (reference manual page 147)
#define GPIOB_BASE_ADDR 0x42020400UL

// GPIO bit masks
#define GPIO_2BIT_MASK 0b11
#define GPIO_1BIT_MASK 0b01

// LED pin definitions (from stm32u5-nucleo144-board.pdf - search "LEDs")
#define BLUE_LED_PIN 7

// GPIO register struct (reference manual page 642)
typedef struct {
    volatile uint32_t moder;     // MODER - offset=0x00: Pin mode (input/output/alternate/analog)
    volatile uint32_t otyper;    // OTYPER - offset=0x04: Output type (push-pull/open-drain)
    volatile uint32_t ospeedr;   // OSPEEDR - offset=0x08: Output speed (low/medium/high/very high)
    volatile uint32_t pupdr;     // PUPDR - offset=0x0C: Pull-up/pull-down resistor configuration
    volatile uint32_t idr;       // IDR - offset=0x10: Input data register
    volatile uint32_t odr;       // ODR - offset=0x14: Output data register
    volatile uint32_t bsrr;      // BSRR - offset=0x18: Bit set/reset register (atomic pin control)
    volatile uint32_t lckr;      // LCKR - offset=0x1C: Lock register
    volatile uint32_t afrl;      // AFRL - offset=0x20: Alternate function low register
    volatile uint32_t afrh;      // AFRL - offset=0x24: Alternate function high register
    volatile uint32_t brr;       // BRR - offset=0x28: Bit reset register
    volatile uint32_t hslvcr;    // HSLVCR - offset=0x2C: High speed level voltage change rate register
    volatile uint32_t seccfgr;   // SECCFGR - offset=0x30: Security configuration register
} __attribute__((aligned(4))) GPIO_REGISTER_t;

// GPIO mode values (reference manual page 633)
#define GPIO_MODER_INPUT           0b00
#define GPIO_MODER_GENERAL_PURPOSE_OUTPUT 0b01
#define GPIO_MODER_ALTERNATE       0b10
#define GPIO_MODER_ANALOG          0b11

// GPIO output type values
#define GPIO_OTYPER_OUTPUT_PUSH_PULL   0b0
#define GPIO_OTYPER_OUTPUT_OPEN_DRAIN  0b1

// GPIO speed values
#define GPIO_OSPEEDR_LOW_SPEED         0b00
#define GPIO_OSPEEDR_MEDIUM_SPEED      0b01
#define GPIO_OSPEEDR_HIGH_SPEED        0b10
#define GPIO_OSPEEDR_VERY_HIGH_SPEED   0b11

// GPIO pull-up/pull-down values
#define GPIO_PUPDR_NO_PULL_UP_OR_DOWN  0b00
#define GPIO_PUPDR_PULL_UP             0b01
#define GPIO_PUPDR_PULL_DOWN           0b10

// GPIO BSRR bit positions (set = bit 0-15, reset = bit 16-31)
#define GPIO_BSRR_SET_BIT  0b01
#define GPIO_BSRR_RESET_BIT 0b01  // Note: reset uses bit 16-31, not bit 0-15

// Function declarations
void gpioInit(GPIO_REGISTER_t *p_port, uint8_t pin);
void gpioLedOn(GPIO_REGISTER_t *p_port, uint8_t pin);
void gpioLedOff(GPIO_REGISTER_t *p_port, uint8_t pin);

#endif