#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "pico/bootrom.h"

// Carrega o programa .pio
#include "ws2812.pio.h"

// Definição do pino de dados e do total de LEDs
#define LED_PIN     7
#define NUM_LEDS    25

// (FPS) da animação, se quiser usar para temporização
#define ANIMATION_FPS 10

// Define o Buzzer
#define BUZZER      21

// Teclado matricial 4×4
#define ROWS 4
#define COL  4

const uint ROW_PIN[ROWS] = {2, 3, 4, 5};
const uint COL_PIN[COL]  = {6, 8, 9, 10};

const char keys[ROWS][COL] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

// Variáveis globais para PIO
static PIO  pio = pio0;
static uint sm  = 0;

// Inicializa o teclado: linhas em saída, colunas em entrada pull-down
void keypad_init(void) {
    for (int i = 0; i < ROWS; i++) {
        gpio_init(ROW_PIN[i]);
        gpio_set_dir(ROW_PIN[i], GPIO_OUT);
        gpio_put(ROW_PIN[i], false); 
    }
    for (int i = 0; i < COL; i++) {
        gpio_init(COL_PIN[i]);
        gpio_set_dir(COL_PIN[i], GPIO_IN);
        gpio_pull_down(COL_PIN[i]);
    }
}

// Lê o teclado matricial 4×4
char read_keypad(void) {
    for (int r = 0; r < ROWS; r++) {
        // Ativa a linha
        gpio_put(ROW_PIN[r], true);
        sleep_us(1000);

        // Verifica colunas
        for (int c = 0; c < COL; c++) {
            if (gpio_get(COL_PIN[c])) {
                gpio_put(ROW_PIN[r], false); 
                return keys[r][c];
            }
        }
        // Desativa a linha
        gpio_put(ROW_PIN[r], false);
    }
    return 0;
}

// Configura GPIOs do teclado e buzzer
static void config_gpio(void) {
    keypad_init();
    // Buzzer
    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);
}

// Função para controle do buzzer (ligar = 1, desligar = 0)
void control_buzzer(uint8_t buzzer) {
    gpio_put(BUZZER, buzzer);
}

uint32_t matrix_rgb(double r, double g, double b) {
    unsigned char R = (unsigned char)(r * 255.0);
    unsigned char G = (unsigned char)(g * 255.0);
    unsigned char B = (unsigned char)(b * 255.0);

    return ( (uint32_t)B << 24 ) | // Blue
           ( (uint32_t)G << 16 ) | // Green
           ( (uint32_t)R <<  8 );  // Red
}
// Limpa (desliga) todos os LEDs enviando cor "0"
static void clear_all_leds(void) {
    for (int16_t i = 0; i < NUM_LEDS; i++) {
        pio_sm_put_blocking(pio, sm, 0);
    }
}

// Envia UM quadro (25 pixels)
static void draw_frame(const uint32_t frame[NUM_LEDS]) {
    for (int i = 0; i < NUM_LEDS; i++) {
        uint32_t rgb = frame[i];
        pio_sm_put_blocking(pio, sm, rgb);
    }
}

// Roda uma animação de `num_frames` quadros; cada quadro tem 25 LEDs.
static void run_animation(const uint32_t frames[][NUM_LEDS], int num_frames, int delay_ms) {
    for (int i = 0; i < num_frames; i++) {
        // Apaga tudo antes de desenhar
        clear_all_leds();
        sleep_us(100);

        // Desenha o quadro i
        draw_frame(frames[i]);

        sleep_ms(delay_ms);
    }
}

// Animação 0: "Deck Suits" com 5 frames
static void animation_deck_suits(void) {
    #define OUROS   matrix_rgb(1.0, 0, 0)    // Vermelho (RGB: 255, 0, 0)
    #define COPAS   matrix_rgb(0.8, 0, 0)    // Vermelho (~80%)
    #define ESPADAS matrix_rgb(0, 0, 1.0)    // Azul (RGB: 0, 0, 255)
    #define PAUS    matrix_rgb(0, 0, 0.8)    // Azul (RGB: 0, 0, 255) (~80%)
    #define JOKER   matrix_rgb(1.0, 1.0, 1.0) // Branco (100%)

    static const uint32_t frames[5][NUM_LEDS] = {
        // Frame 1 (Ouros)
        {
          0,0,OUROS,0,0,
          0,OUROS,OUROS,OUROS,0,
          OUROS,OUROS,OUROS,OUROS,OUROS,
          0,OUROS,OUROS,OUROS,0,
          0,0,OUROS,0,0
        },
        // Frame 2 (Copas)
        {
          0,0,COPAS,0,0,
          0,COPAS,COPAS,COPAS,0,
          COPAS,COPAS,COPAS,COPAS,COPAS,
          0,COPAS,COPAS,COPAS,0,
          0,0,COPAS,0,0
        },
        // Frame 3 (Espadas)
        {
          0,0,ESPADAS,0,0,
          ESPADAS,ESPADAS,ESPADAS,ESPADAS,ESPADAS,
          ESPADAS,ESPADAS,ESPADAS,ESPADAS,ESPADAS,
          0,ESPADAS,ESPADAS,ESPADAS,0,
          0,0,ESPADAS,0,0
        },
        // Frame 4 (Paus)
        {
          0,PAUS,PAUS,PAUS,0,
          PAUS,PAUS,PAUS,PAUS,PAUS,
          PAUS,PAUS,PAUS,PAUS,PAUS,
          0,PAUS,PAUS,PAUS,0,
          0,0,PAUS,0,0
        },
        // Frame 5 (Joker)
        {
          0,0,JOKER,JOKER,JOKER,
          0,JOKER,JOKER,JOKER,0,
          JOKER,JOKER,JOKER,JOKER,JOKER,
          JOKER,JOKER,JOKER,JOKER,0,
          0,0,JOKER,JOKER,JOKER
        }
    };

    run_animation(frames, 5, 500);
}

// Animação 1: Snake (7 frames)
static void animation_snake(void) {
    #define SNAKE_COLOR matrix_rgb(0, 0.5, 0) // Verde (~50%)
    static const uint32_t frames[7][NUM_LEDS] = {
        {
            SNAKE_COLOR,0,SNAKE_COLOR,0,0,
            0,0,0,0,0,
            0,0,SNAKE_COLOR,0,0,
            0,0,0,0,0,
            0,0,0,0,SNAKE_COLOR
        },
        {
            0,SNAKE_COLOR,SNAKE_COLOR,0,0,
            0,0,0,0,0,
            0,0,SNAKE_COLOR,0,0,
            0,0,0,0,0,
            0,0,0,0,SNAKE_COLOR
        },
        {
            0,0,SNAKE_COLOR,0,0,
            0,SNAKE_COLOR,0,0,0,
            0,0,SNAKE_COLOR,0,0,
            0,0,0,0,0,
            0,0,0,0,SNAKE_COLOR
        },
        {
            0,0,0,0,0,
            0,SNAKE_COLOR,SNAKE_COLOR,0,0,
            0,0,SNAKE_COLOR,0,0,
            0,0,0,0,0,
            0,0,0,0,SNAKE_COLOR
        },
        {
            0,0,0,0,0,
            0,0,SNAKE_COLOR,0,0,
            0,SNAKE_COLOR,SNAKE_COLOR,0,0,
            0,0,0,0,0,
            0,0,0,0,SNAKE_COLOR
        },
        {
            0,0,0,0,0,
            0,0,SNAKE_COLOR,0,0,
            0,SNAKE_COLOR,SNAKE_COLOR,0,0,
            0,0,0,SNAKE_COLOR,0,
            0,0,0,0,0
        },
        {
            0,0,0,0,0,
            0,0,0,SNAKE_COLOR,0,
            0,SNAKE_COLOR,SNAKE_COLOR,0,0,
            0,0,0,SNAKE_COLOR,0,
            0,0,0,0,0
        }
    };

    run_animation(frames, 7, 500);
}

int main() {
    stdio_init_all();
    config_gpio();

    // Ajusta clock para 128 MHz (se desejar)
    bool ok = set_sys_clock_khz(128000, false);
    if (ok) {
        printf("Clock ajustado para %ld Hz\n", clock_get_hz(clk_sys));
    }

    // Inicializa o PIO (ws2812_program)
    uint offset = pio_add_program(pio, &ws2812_program);
    sm = pio_claim_unused_sm(pio, true);
    ws2812_program_init(pio, sm, offset, LED_PIN);

    while (true) {
        char key = read_keypad();
        if (key != 0) {
            printf("Tecla pressionada: %c\n", key);

            switch (key) {
                case '0': {
                    animation_deck_suits();
                    control_buzzer(1); 
                    sleep_ms(500);
                    control_buzzer(0);
                    break;
                }

                case '1': {
                    animation_snake();
                    break;
                }
                case '2' : { //Animação que Gabriel Marcone Fará.
                    printf("Nada\n");
                    break;
                }
                case '3' : {
                    printf("Nada\n");
                    break;
                }
                case '4' : {
                    printf("Nada\n");
                    break;
                }
                case '5' : {
                    printf("Nada\n");
                    break;
                }
                case '6' : {
                    printf("Nada\n");
                    break;
                }
                case 'A': { // Desliga todos os LEDs
                    clear_all_leds();

                    control_buzzer(1);
                    sleep_ms(1000);        //Aciona o Buzzer por 1 segundo após apagar os leds
                    control_buzzer(0);
                    break;
                }

                case 'B': { // Azul 100%
                    uint32_t color = matrix_rgb(0, 0, 1.0); // Azul em RGB
                    for (int i = 0; i < NUM_LEDS; i++) {
                        pio_sm_put_blocking(pio, sm, color);
                    }
                } break;

                case 'C': { // Vermelho 80%
                    uint32_t color = matrix_rgb(0.8, 0, 0); // Vermelho em RGB
                    for (int i = 0; i < NUM_LEDS; i++) {
                        pio_sm_put_blocking(pio, sm, color);
                    }
                } break;

                case 'D': { // Verde 50%
                    uint32_t color = matrix_rgb(0, 0.5, 0); // Verde em RGB
                    for (int i = 0; i < NUM_LEDS; i++) {
                        pio_sm_put_blocking(pio, sm, color);
                    }
                } break;

                case '#': { // Branco 20%
                    uint32_t color = matrix_rgb(0.2, 0.2, 0.2); // Branco em RGB
                    for (int i = 0; i < NUM_LEDS; i++) {
                        pio_sm_put_blocking(pio, sm, color);
                    }
                } break;

                case '*': {
                    printf("Saindo do modo exec e habilitando modo de gravacao...\n");
                    reset_usb_boot(0, 0);
                    break;
                }

                default: {
                    clear_all_leds();
                    break;
                }
            }
        }
        sleep_ms(100);
    }
    return 0;
}
