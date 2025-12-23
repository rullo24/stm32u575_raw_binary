/* ================================================================
 * STM32U575 Bare-Metal Linker Script
 * ================================================================ */

/* ================================================================
* STEP 1: MEMORY REGIONS
* 
* Define the physical memory available on the chip.
* The linker uses this to know where it can place things.
* ================================================================ */

MEMORY
{
    FLASH (rx) : ORIGIN = 0x0800000, LENGTH = 0x200000
    RAM (xrw) : ORIGIN = 0x2000000, LENGTH = 0x0C0000
}

/* ================================================================
* STEP 2: INITIAL STACK POINTER
*
* _estack is a symbol that will hold the initial value of the
* stack pointer (SP). On reset, the CPU loads SP from the first
* word of the vector table. We will point that first word to this
* address, which is the TOP of RAM.
*
* Top of RAM = ORIGIN(RAM) + LENGTH(RAM)
*            = 0x20000000 + 0x0C0000 = 0x200C0000
* ================================================================ */

_estack = ORIGIN(RAM) + LENGTH(RAM);

/* ================================================================
* STEP 3: SECTIONS AND VECTOR TABLE
*
* SECTIONS tells the linker how to lay out the contents of the
* program in the memory regions we defined above.
*
* The first section we define is .isr_vector, which holds the
* interrupt vector table. This MUST be at the start of Flash,
* because on reset the CPU reads the first two words here:
*   - word 0: initial stack pointer value
*   - word 1: address of the reset handler function
* ================================================================ */

SECTIONS 
{
   .isr_vector : 
   {
        KEEP(*(.isr_vector)) /* linker should not discard this even if unused because CPU jumps here on reset */
   } >FLASH
}

/* ------------------------------------------------------------
* STEP 4: Program code (.text)
*
* This is all of your executable code: main(), other functions,
* and any code the compiler generates. We want this in FLASH,
* because it is read-only and must persist across resets.
* ------------------------------------------------------------ */

.text :
{
    *(.text) /* all sections named exactly ".text" */
    *(.text*) /* any sections that start with ".text" e.g. ".text.main" */
} >FLASH

/* ------------------------------------------------------------
* STEP 5: Read-only data (.rodata)
*
* This holds constants, such as string literals and 'const'
* variables. They never change, so we also keep them in FLASH
* to avoid wasting RAM.
* ------------------------------------------------------------ */

.rodata :
{
    *(.rodata) /* all read-only data sections */
    *(.rodata*) /* and any that start with .rodata */
} >FLASH

/* ------------------------------------------------------------
* STEP 6: Initialized variables (.data)
*
* Variables with initial values (like "int x = 42;") need special
* handling:
*   1. Initial values are stored in FLASH (permanent)
*   2. At startup, startup code COPIES them from FLASH to RAM
*   3. Your program uses the RAM copies (which can be modified)
*
* The "AT> FLASH" means: store the data in FLASH, but link
* addresses as if it's in RAM. This lets startup code know where
* to copy FROM (Flash) and TO (RAM).
*
* Symbols created:
*   - _sidata: Start address in FLASH (where to copy FROM)
*   - _sdata: Start address in RAM (where to copy TO)
*   - _edata: End address in RAM (know when to stop copying)
* ------------------------------------------------------------ */

_sidata = LOADADDR(.data) /* addr in flash where .data is stored */

.data :
{
    _sdata = .; /* . = current location, save as start of .data in RAM */
    *(.data)
    *(.data*)
    _edata = .; /* Save current location as end of .data in RAM */
} >RAM AT >FLASH /* Link as if in RAM, but store in Flash */

/* ------------------------------------------------------------
* STEP 7: Uninitialized variables (.bss)
*
* Variables without initial values (like "int x;" or "static int y;")
* go in the .bss section. These must be zeroed at startup.
*
* How it works:
*   1. Linker reserves space in RAM for these variables
*   2. At startup, startup code ZEROS this entire section
*   3. Your program then uses these zero-initialized variables
*
* Why zero them?
*   - C standard: Uninitialized static/global variables must be zero
*   - RAM contains random values on power-up (garbage)
*   - We must explicitly zero them
*
* Symbols created:
*   - _sbss: Start of BSS section (where to start zeroing)
*   - _ebss: End of BSS section (where to stop zeroing)
*
* Why *(COMMON)?
*   - Some compilers put uninitialized globals in COMMON section
*   - We collect those too
*
* Location: RAM only (nothing to copy from Flash, just zero it)
* ------------------------------------------------------------ */

.bss :
{
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(COMMON) /* collect common sections (uninit globals) */
    _ebss = .;
} >RAM

/* ------------------------------------------------------------
* STEP 8: Stack and Heap Space
*
* Reserve memory for stack and heap (even if we don't use heap).
*
* What is the stack?
*   - Memory used for function calls, local variables, return addresses
*   - Grows DOWNWARD from top of RAM (we already set _estack)
*   - Each function call "pushes" data onto stack
*   - When function returns, data is "popped" off stack
*
* What is the heap?
*   - Memory for dynamic allocation (malloc/free)
*   - We're not using it in bare-metal, but reserve space anyway
*   - Grows UPWARD from end of variables
*
* Memory layout in RAM:
*   0x20000000: Variables start (.data, .bss)
*        ↓
*        ↓  (Variables grow upward)
*        ↓
*   [Heap space] (if used, grows upward)
*        ↓
*   [Stack space] (grows downward)
*        ↓
*   0x200C0000: Stack starts (_estack)
*
* Why ALIGN(8)?
*   - ARM requires 8-byte alignment for stack pointer
*   - Aligns to 8-byte boundary for efficiency
*
* Symbols:
*   - end, _end: Used by some C libraries (we define them for compatibility)
* ------------------------------------------------------------ */

.user_heap_stack :
{
    . = ALIGN(8); /* align to 8-byte boundary (ARM requirement) */
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + 0x200; /* reserve 512 bytes for heap */
    . = . + 0x400; /* reserver 1KB for stack */
    . = ALIGN(8); /* align end to 8-byte boundary */
}