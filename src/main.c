/* ================================================================
* bare‑metal main for STM32U575
* ================================================================ */

#include "gpio.h"
#include "rcc.h"
#include "uart.h"
#include "systick.h"

static GPIO_REGISTER_t *const p_GPIOB = (GPIO_REGISTER_t*)GPIOB_BASE_ADDR;

int main(void) {

    // configure system clock first (required before peripheral tick)
    // sysClkConfig();

    // init peripherals
    // uartInit();
    // sysTickInit();
    gpioInit(p_GPIOB, BLUE_LED_PIN);

    // turn on blue LED
    gpioLedOn(p_GPIOB, BLUE_LED_PIN);

    while (1) {
        __asm__("nop");
    }

    return 0;
}