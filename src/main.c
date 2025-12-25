/* ================================================================
* bare‑metal main for STM32U575
* ================================================================ */

#include "structs.h"

// See reference manual page 147: Memory map and peripheral register boundary addresses

#define GPIOB_START_ADDR 0x42020400 
#define GPIOB_END_ADDR 0x420207FF

// see nucleo144 board pdf page 25: LEDs for registers used


int main(void) {

    while (1) {
        __asm__("nop"); // do nothing, just loop forever
    }

    return 0;
}