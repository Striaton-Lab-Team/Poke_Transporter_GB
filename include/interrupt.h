#ifndef INTERRUPT_H
#define INTERRUPT_H

typedef enum
{
    INTR_VBLANK = 0,
    INTR_HBLANK = 1,
    INTR_VCOUNT = 2,
    INTR_TIMER0 = 3,
    INTR_TIMER1 = 4,
    INTR_TIMER2 = 5,
    INTR_TIMER3 = 6,
    INTR_SERIAL = 7,
    INTR_DMA0 = 8,
    INTR_DMA1 = 9,
    INTR_DMA2 = 10,
    INTR_DMA3 = 11,
    INTR_KEYPAD = 12,
    INTR_GAMEPAK = 13,
    INTR_NUMBER
} interrupt_index;

typedef void (*interrupt_vector)(void);

void interrupt_init(void);
void interrupt_set_handler(interrupt_index index, interrupt_vector function);
void interrupt_enable(interrupt_index index);
void interrupt_disable(interrupt_index index);
void interrupt_set_reference_vcount(unsigned long y);

#endif // INTERRUPT_H