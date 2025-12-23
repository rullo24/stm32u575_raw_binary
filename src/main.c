/* ================================================================
* bare‑metal main for STM32U575
* ================================================================ */

int main(void) {
    
    while (1) {
        __asm__("nop"); // do nothing, just loop forever
    }

    return 0;
}