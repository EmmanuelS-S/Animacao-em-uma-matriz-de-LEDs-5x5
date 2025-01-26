// ws2812.pio.h

#include "hardware/pio.h"

// Funções para inicializar e controlar os LEDs WS2812
void ws2812_init(PIO pio, uint sm, uint offset, uint pin);
void ws2812_put_pixel(PIO pio, uint sm, uint32_t *rgb);
pio_sm_config ws2812_program_get_default_config(uint offset);

extern const pio_program_t ws2812_program;
