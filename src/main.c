/* ================================================================
* bare‑metal main for STM32U575
* ================================================================ */

// See reference manual page 147: Memory map and peripheral register boundary addresses

#define GPIOB_START_ADDR 0x42020400 
#define GPIOB_END_ADDR 0x420207FF

/// === GPIO REGISTERS REQUIRED (reference manual page 642) === ///
// MODER - offset=0x00: Pin mode (input/output/alternate/analog)
// OTYPER - offset=0x04: Output type (push-pull/open-drain)
// OSPEEDR - offset=0x08: Output speed (low/medium/high/very high)
// PUPDR - offset=0x0C: Pull-up/pull-down resistor configuration
// BSRR - offset=0x18: Bit set/reset register (atomic pin control)

// see nucleo144 board pdf page 25: LEDs for registers used

int main(void) {
    
    while (1) {
        __asm__("nop"); // do nothing, just loop forever
    }

    return 0;
}