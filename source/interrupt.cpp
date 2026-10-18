#include "interrupt.h"
#include "libraries/libugba/include/ugba.h"

void interrupt_init(void) { IRQ_Init(); }

void interrupt_set_handler(interrupt_index index, interrupt_vector function)
{
    IRQ_SetHandler((irq_index)index, function);
}

void interrupt_enable(interrupt_index index) { IRQ_Enable((irq_index)index); }

void interrupt_disable(interrupt_index index) { IRQ_Disable((irq_index)index); }
