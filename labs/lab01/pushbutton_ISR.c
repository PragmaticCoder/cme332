#include "address_map_nios2.h"
#include "globals.h" // defines global values
#include "utils.h"

extern volatile int pattern, shift_enable;
int reset;
/*******************************************************************************
 * Pushbutton - Interrupt Service Routine
 *
 * This routine checks which KEY has been pressed and updates the global
 * variables as required.
 ******************************************************************************/
void pushbutton_ISR(void)
{
    volatile int *KEY_ptr = (int *)KEY_BASE;
    volatile int *slider_switch_ptr = (int *)SW_BASE;
    int press;

    press = *(KEY_ptr + 3); // read the pushbutton interrupt register
    *(KEY_ptr + 3) = press; // Clear the interrupt

    if (press & 0x1) // KEY0
        pattern = *slider_switch_ptr;

    if (press & 0x2) // KEY1
        pause ^= 1;

    if (press & 0x4) // KEY3
    {
        reset ^= 1;
    }

    if (reset)
        reset_system();

    return;
}
