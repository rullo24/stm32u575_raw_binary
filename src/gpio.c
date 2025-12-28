/* ================================================================
* GPIO peripheral driver for STM32U575
* ================================================================ */

#include "gpio.h"
#include "rcc.h"

// DESCRPTION
// Initialises the GPIO peripheral for the blue LED
void gpioInit(GPIO_REGISTER_t *p_port, uint8_t pin) {

    // enable GPIOB peripheral clock
    rccEnableGpioB();

    // Configure PB7 as general purpose output
    p_port->moder &= ~(GPIO_2BIT_MASK << (2 * pin));  // Clear bits 14:15
    p_port->moder |= GPIO_MODER_GENERAL_PURPOSE_OUTPUT << (2 * pin);
    
    // Configure as push-pull output
    p_port->otyper &= ~(GPIO_1BIT_MASK << pin);
    p_port->otyper |= GPIO_OTYPER_OUTPUT_PUSH_PULL << pin;
    
    // Configure low speed
    p_port->ospeedr &= ~(GPIO_2BIT_MASK << (2 * pin));
    p_port->ospeedr |= GPIO_OSPEEDR_LOW_SPEED << (2 * pin);
    
    // Configure pull-up
    p_port->pupdr &= ~(GPIO_2BIT_MASK << (2 * pin));
    p_port->pupdr |= GPIO_PUPDR_PULL_UP << (2 * pin);

}

void gpioLedOn(GPIO_REGISTER_t *p_port, uint8_t pin) {
    p_port->bsrr = (GPIO_BSRR_SET_BIT << pin);
}

void gpioLedOff(GPIO_REGISTER_t *p_port, uint8_t pin) {
    p_port->bsrr = (GPIO_BSRR_RESET_BIT << (pin + 16));
}