/* ================================================================
* STM32U575 Startup Code
* 
* This file contains the interrupt vector table and initialisation
* code that runs before main() is called.
* ================================================================ */

// external symbols defined in linker.ld 
extern unsigned long _estack; // top of stack
extern unsigned long _sidata; // start of data in flash
extern unsigned long _sdata; // start of data in ram
extern unsigned long _edata; // end of data in ram
extern unsigned long _sbss; // start of bss in ram
extern unsigned long _ebss; // end of bss in ram

// forward declaration of main()
extern int main(void);

/* ================================================================
* STEP 2: Default Interrupt Handlers
* 
* These are "weak" symbols, meaning they can be overridden by
* your own interrupt handlers if you define them elsewhere.
* 
* If an interrupt occurs and you haven't defined a handler, these
* default handlers will be called (they do nothing, just loop).
* ================================================================ */
void Default_Handler(void) __attribute__((weak));
void Default_Handler(void) {
    while (1) {
        __asm__("nop"); // do nothing, just loop forever
    }
}

/* Individual interrupt handlers - all point to Default_Handler */
/* These will be used in the vector table */
void Reset_Handler(void); // real implementation is in startup.c -> no alias here
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

/* ================================================================
* STEP 3: Interrupt Vector Table
* 
* This table MUST be placed at the start of Flash (0x08000000).
* The linker script ensures this via the .isr_vector section.
* 
* Structure:
*   - First entry: Initial stack pointer value (top of RAM)
*   - Second entry: Reset handler address
*   - Remaining entries: Other interrupt handler addresses
* 
* The CPU automatically loads SP from the first word on reset.
* ================================================================ */

// function ptr type for interrupt handlers
typedef void (*InterruptHandler)(void);

// vector table - must be in .isr_vector section (handled by linker.ld)
__attribute__((section(".isr_vector")))
const InterruptHandler vector_table[] = {
    (InterruptHandler)(&_estack), /* Initial stack pointer value (top of RAM) */
    Reset_Handler, /* Reset handler address */
    NMI_Handler, /* NMI handler address */
    HardFault_Handler, /* Hard fault handler address */
    MemManage_Handler, /* Memory management fault handler address */
    BusFault_Handler, /* Bus fault handler address */
    UsageFault_Handler, /* Usage fault handler address */
    SVC_Handler, /* SVC handler address */
    DebugMon_Handler, /* Debug monitor handler address */
    PendSV_Handler, /* PendSV handler address */
    SysTick_Handler /* SysTick handler address */
};

/* ================================================================
* STEP 4: Reset Handler Implementation
* 
* This is the FIRST function that runs when the chip powers on or resets.
* It does three critical things:
*   1. Copy initialised variables from Flash to RAM (.data section)
*   2. Zero out uninitialised variables (.bss section)
*   3. Call main()
* 
* Note: We override the weak Reset_Handler alias from Step 2.
* ================================================================ */

void Reset_Handler(void) {
    unsigned long *src, *dest;

    /* ------------------------------------------------------------
    * 4.1: Copy .data section from Flash to RAM
    * 
    * Why? Initialized variables (like "int x = 42;") have their
    * initial values stored in Flash (permanent), but the actual
    * variables must live in RAM (so they can be modified).
    * 
    * We copy from _sidata (Flash) to _sdata (RAM), up to _edata.
    * ------------------------------------------------------------ */

    src = &_sidata; // start of .data in Flash
    dest = &_sdata; // start of .data in RAM

    // copy word by word until end of .data
    while (dest < &_edata) {
        *dest++ = *src++; // copy one word (4-bytes)
    }

    /* ------------------------------------------------------------
    * 4.2: Zero out .bss section
    * 
    * Why? Uninitialised static/global variables (like "int x;")
    * must start as zero according to the C standard. RAM contains
    * random garbage on power-up, so we must explicitly zero it.
    * 
    * We zero from _sbss to _ebss.
    * ------------------------------------------------------------ */

    dest = &_sbss; // start of .bss in RAM

    // zero word by word until end of .bss
    while (dest < &_ebss) {
        *dest++ = 0; // zero one word (4-bytes) and advance ptr
    }

    /* ------------------------------------------------------------
    * 4.3: Call main()
    * 
    * Now that RAM is initialised, we can safely call main().
    * Your program starts here!
    * ------------------------------------------------------------ */

    main();

    // If main() ever returns (it shouldn't in embedded systems), loop forever
    while (1) {
        __asm__("nop"); // do nothing, just loop forever
    }
}