#include "ws2812.pio.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "config.h"
// Função para inicializar o PIO para controlar os LEDs WS2812
void ws2812_init(PIO pio, uint sm, uint offset, uint pin) {
    pio_sm_config c = ws2812_program_get_default_config(offset);
    sm_config_set_sideset_pins(&c, pin);
    sm_config_set_out_shift(&c, false, true, 24);
    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

// Função para enviar dados para os LEDs WS2812
void ws2812_put_pixel(PIO pio, uint sm, uint32_t *rgb) {
    for (int i = 0; i < NUM_LEDS; i++) {
        pio_sm_put_blocking(pio, sm, rgb[i] << 8u);
    }
}

// Função para obter a configuração padrão do programa WS2812
pio_sm_config ws2812_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_clkdiv(&c, 1.0f);
    sm_config_set_sideset_pins(&c, 1);
    sm_config_set_out_shift(&c, false, true, 24);
    return c;
}

const pio_program_t ws2812_program = {
    .instructions = (uint16_t[]) {
        0x6221, //  0: out    x, 1
        0x1123, //  1: jmp    !osre, 3
        0xa042, //  2: nop          [2]
        0xa040, //  3: nop
    },
    .length = 4,
    .origin = -1,
};
