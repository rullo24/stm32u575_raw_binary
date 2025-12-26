/* ================================================================
* bare‑metal main for STM32U575
* ================================================================ */

#include "structs.h"

// See reference manual page 147: Memory map and peripheral register boundary addresses

#define GPIOB_START_ADDR 0x42020400UL
#define GPIOB_END_ADDR 0x420207FFUL

#define RCC_BASE_ADDR 0x46020C00UL
#define RCC_AHB2ENR1 (*(volatile uint32_t*)(RCC_BASE_ADDR + 0x8C))

// see nucleo144 board pdf page 25: LEDs for registers used

// See reference manual page 633: GPIO registers
#define GPIO_MODER_GENERAL_PURPOSE_OUTPUT 0b01

#define GPIO_OTYPER_OUTPUT_PUSH_PULL 0b0 

#define GPIO_OSPEEDR_LOW_SPEED 0b00
#define GPIO_OSPEEDR_MEDIUM_SPEED 0b01
#define GPIO_OSPEEDR_HIGH_SPEED 0b10
#define GPIO_OSPEEDR_VERY_HIGH_SPEED 0b11

#define GPIO_PUPDR_NO_PULL_UP_OR_DOWN 0b00
#define GPIO_PUPDR_PULL_UP 0b01
#define GPIO_PUPDR_PULL_DOWN 0b10

#define GPIO_BSRR_RESET_BIT 0b1
#define GPIO_BSRR_SET_BIT 0b1

int main(void) {

    // enable the GPIOB peripheral clock by setting the appropriate bit in RCC_AHB2ENR
    RCC_AHB2ENR1 |= (1 << 1);  // Enable GPIOB clock

    // PB7 == Blue LED
    volatile GPIO_REGISTER_t *p_gpio_b = (GPIO_REGISTER_t*)GPIOB_START_ADDR;
    p_gpio_b->moder &= ~(0x3 << (2*7));  // Clear bits 14:15
    p_gpio_b->moder |= GPIO_MODER_GENERAL_PURPOSE_OUTPUT << (2*7); // double-bit width

    p_gpio_b->otyper &= ~(1 << 7);  // Clear bit 7 (push-pull = 0)
    p_gpio_b->otyper |= GPIO_OTYPER_OUTPUT_PUSH_PULL << 7; // single-bit width

    p_gpio_b->ospeedr &= ~(0x3 << (2*7));  // Clear bits 14:15
    p_gpio_b->ospeedr |= GPIO_OSPEEDR_LOW_SPEED << (2*7); // double-bit width

    p_gpio_b->pupdr &= ~(0x3 << (2*7));  // Clear bits 14:15
    p_gpio_b->pupdr |= GPIO_PUPDR_PULL_UP << (2*7); // double-bit width

    // setting LED on
    p_gpio_b->bsrr |= GPIO_BSRR_SET_BIT << 7; // no reset in here for now

    while (1) {
        __asm__("nop");
    }

    return 0;
}