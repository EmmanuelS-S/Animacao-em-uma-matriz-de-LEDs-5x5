#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "blink.pio.h" // Biblioteca para manipular o arquivo blink.pio

// Definindo Linhas e Colunas do teclado matricial
#define ROWS 4
#define COL 4

// Mapas de pinos para o teclado matricial
const uint ROW_PIN[ROWS] = {8, 7, 6, 5};
const uint COL_PIN[COL] = {4, 3, 2, 28};

// Definições da Matriz de LEDs 5x5
#define NUM_LEDS 25
//define o array NUM_LEDS como sendo de 25 posicoes (0 a 24)
#define LED_PIN 22 
// Pino de saída para WS2812
#define ANIMATION_FPS 10 
// Quadros por segundo

// Definições dos LEDs RGB
#define LED_GREEN 11
#define LED_BLUE 12
#define LED_RED 13

// Definição do buzzer
#define BUZZER 21

// Mapas de teclas do teclado matricial
const char keys[ROWS][COL] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Função para ler o teclado matricial 4x4
char read_keypad() {
    for (int r = 0; r < ROWS; r++) {
        gpio_put(ROW_PIN[r], true); // Ativação da linha
        sleep_us(50); 

        for (int c = 0; c < COL; c++) {
            if (gpio_get(COL_PIN[c])) { // Verificação de sinal na coluna
                gpio_put(ROW_PIN[r], false); // Desativa a linha
                return keys[r][c]; // Retorna a tecla pressionada
            }
        }
        gpio_put(ROW_PIN[r], false); // Desativa a linha
    }

    return 0; 
}

uint32_t led_colors[NUM_LEDS]; /* Array para armazenar cores dos LEDs, cada posição corresponde a um LED da matriz de LEDs 5x5 (totalizando 25) sendo cada posicao um número de 32 bits, 
cores representadas por 24 bits (vermelho (8), verde(8) e azul(8)), e os 8 bits restantes são geralmente usados para controle de brilho ou deixados como zero.*/

// Funções auxiliares para WS2812
// Função para limpar os LEDs
void clean_leds() {
    for (int i = 0; i < NUM_LEDS; i++) {
        set_led(i, 0); // Limpando todos os LEDs (definindo a cor como 0)
    }
}

//Função para exibir um quadro de LEDs/ um frame na matriz de LEDs 5x5
void show_frame(uint32_t frame[5][5]) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int led_index = i * 5 + j; // Convertendo as coordenadas 2D em um índice 1D
            set_led(led_index, frame[i][j]); // Definindo a cor do LED
        }
    }
}
// Função para exibir a animação composta por vários frames
void run_animation(uint32_t frames[5][5][5], int num_frames) {
    for (int frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        clean_leds();  // Limpar os LEDs antes de exibir o próximo quadro
        show_frame(frames[frame_idx]);  // Exibir o quadro atual
        delay(500);  // Atraso para a animação (500 ms por exemplo)
    }
}

// Funcao para Inicializar as animações definindo os frames de cada uma, e um exemplo de Animaçao
/*void init_animation(uint32_t animation[7][5][5][5]) {
    // Animação 1 (Exemplo: LEDs piscando alternadamente)
    uint32_t red = 0xFF0000, black = 0x000000;
    animation[0][0][0][0] = red;
    animation[0][0][0][1] = black;
}*/
//Animacao 0, com 5 frames, sequencia de naipes de baralho, ouros, copas, espadas e paus e o joker
void animation_deck_suits() {
    // Definindo as cores para os naipes
    //uint32_t ouros_color = 0xFFFF00;    // Ouros (Amarelo)
    uint32_t ouros_color = 0xFF0000;    // Ouros (vermelho)
    uint32_t copas_color = 0xFF0000;   // Copas (Vermelho)
    uint32_t espadas_color = 0x000000; // Espadas (Preto)
    uint32_t paus_color = 0x000000;    // Paus (Preto)
    uint32_t joker_color = 0xFFFFFF;   // Joker (Branco)

    // Definindo os frames da animação (5 frames)
    /*Posicoes na matriz: Ouros:(0,2), (1,1), (1,2), (1,3), (2,0), (2,1) (2,2), (2,3), (2,4), (3,1), (3,2), (3,3), (4,2), 
    copas: (1,1), (1,3), (2,0), (2,1), (2,2), (2,3), (2,4), (3,1), (3,2), (3,3), (4,2), 
    espadas: (0,2), (1,0), (1,1), (1,2), (1,3), (1,4), (2,0), (2,1), (2,2), (2,3), (2,4), (3,2), (4,1), (4,2), (4,3), 
    paus: (0,1), (0,2), (0,3), (1,0), (1,1), (1,2), (1,3), (1,4), (2,0), (2,1), (2,2), (2,3), (2,4), (3,2), (4,1), (4,2), (4,3), 
    joker: (0,2), (0,3), (0,4), (1,3), (2,3), (3,0), (3,1), (3,3), (4,0), (4,1), (4,2), (4,3)*/
    uint32_t frames[5][5][5] = {
        // Frame 1 (Ouros)
        {
            {0, 0, ouros_color, 0, 0},
            {0, ouros_color, ouros_color, ouros_color, 0},
            {ouros_color, ouros_color, ouros_color, ouros_color, ouros_color},
            {0, ouros_color, ouros_color, ouros_color, 0},
            {0, 0, ouros_color, 0, 0}
        },
        // Frame 2 (Copas)
        {
            {0, 0, copas_color, 0, 0},
            {0, copas_color, copas_color, copas_color, 0},
            {copas_color, copas_color, copas_color, copas_color, copas_color},
            {0, copas_color, copas_color, copas_color, 0},
            {0, 0, copas_color, 0, 0}
        },
        // Frame 3 (Espadas)
        {
            {0, 0, espadas_color, 0, 0},
            {espadas_color, espadas_color, espadas_color, espadas_color, espadas_color},
            {espadas_color, espadas_color, espadas_color, espadas_color, espadas_color},
            {0, espadas_color, espadas_color, espadas_color, 0},
            {0, 0, espadas_color, 0, 0}
        },
        // Frame 4 (Paus)
        {
            {0, paus_color, paus_color, paus_color, 0},
            {paus_color, paus_color, paus_color, paus_color, paus_color},
            {paus_color, paus_color, paus_color, paus_color, paus_color},
            {0, paus_color, paus_color, paus_color, 0},
            {0, 0, paus_color, 0, 0}
        },
        // Frame 5 (Joker)
        {
            {0, 0, joker_color, joker_color, joker_color},
            {0, joker_color, joker_color, joker_color, 0},
            {joker_color, joker_color, joker_color, joker_color, joker_color},
            {joker_color, joker_color, joker_color, joker_color, 0},
            {0, 0, joker_color, joker_color, joker_color}
        }
    };

    // Executando a animação dos naipes
    run_animation(frames, 5); // A animação tem 5 frames
}
//Animacao 1, jogo da cobrinha com 7 frames
void animation_snake() {
    // Definindo a cor preta para a cobrinha
    uint32_t snake_color = 0x000000; // Preto

    // Definindo os frames da animação (7 frames)
    /*Frame 1: (0,0), (0,2), (2,3), (4,2), frame 2: (0,1), (0,2), (2,3), (4,2), 
    frame 3: (0,2), (1,2), (2,3), (4,2), frame 4: (1,2), (2,2), (2,3), (4,2), 
    frame 5: (2,2), (2,3), (3,3), (4,2), frame 6: (2,3), (3,3),(4,3), (4,2), frame 7: (3,3),(4,3),(4,2),(4,1)*/
    uint32_t snake_frames[7][5][5] = {
        // Frame 1
        {
            {snake_color, 0, snake_color, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, snake_color, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, snake_color}
        },
        // Frame 2
        {
            {0, snake_color, snake_color, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, snake_color, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, snake_color}
        },
        // Frame 3
        {
            {0, 0, snake_color, 0, 0},
            {0, snake_color, 0, 0, 0},
            {0, 0, snake_color, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, snake_color}
        },
        // Frame 4
        {
            {0, 0, 0, 0, 0},
            {0, snake_color, snake_color, 0, 0},
            {0, 0, snake_color, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, snake_color}
        },
        // Frame 5
        {
            {0, 0, 0, 0, 0},
            {0, 0, snake_color, 0, 0},
            {0, snake_color, snake_color, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, snake_color}
        },
        // Frame 6
        {
            {0, 0, 0, 0, 0},
            {0, 0, snake_color, 0, 0},
            {0, snake_color, snake_color, 0, 0},
            {0, 0, 0, snake_color, 0},
            {0, 0, 0, 0, 0}
        },
        // Frame 7
        {
            {0, 0, 0, 0, 0},
            {0, 0, 0, snake_color, 0},
            {0, snake_color, snake_color, 0, 0},
            {0, 0, 0, snake_color, 0},
            {0, 0, 0, 0, 0}
        }
    };

    // Executando a animação do jogo da cobrinha
    run_animation(snake_frames, 7); // A animação tem 7 frames
}
void config_gpio() {
    // Configurando as linhas do teclado como saídas
    for (int i = 0; i < ROWS; i++) {
        gpio_init(ROW_PIN[i]);
        gpio_set_dir(ROW_PIN[i], GPIO_OUT);
    }

    // Configurando as colunas do teclado como entradas com pull-down
    for (int i = 0; i < COL; i++) {
        gpio_init(COL_PIN[i]);
        gpio_set_dir(COL_PIN[i], GPIO_IN);
        gpio_pull_down(COL_PIN[i]);
    }
    // Incializando e configurando GPIOs dos LEDS e BUZZER como saidas
    gpio_init(LED_GREEN); gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_init(LED_BLUE); gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_init(LED_RED); gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_init(BUZZER); gpio_set_dir(BUZZER, GPIO_OUT);
}
// Função para acionar os pinos
void control_gpios(uint8_t red, uint8_t blue, uint8_t green, uint8_t buzzer) {
    gpio_put(LED_RED, red);
    gpio_put(LED_BLUE, blue);
    gpio_put(LED_GREEN, green);
    gpio_put(BUZZER, buzzer);
}

int main() {
    stdio_init_all();
    config_gpio();

    while (true) {
        char key = read_keypad(); // Leitura do teclado matricial 4x4

        if (key != 0) { // Se alguma tecla for pressionada
            printf("Tecla pressionada: %c\n", key);

            switch (key) {
                /*->As teclas de 0 a 6 do teclado matricial, caso acionadas, o sistema deve executar algum tipo de animação na matriz de LEDs 5x5. Os tipos de desenho, cores (RGB) e 
                luminosidades associados a estas imagens/animações ficam a critério dos membros da equipe de desenvolvimento, Cada animação deve possuir, no mínimo, 5 frames 
                (imagens diferentes) e o FPS (quantidade de quadros por segundo) também deve ser definido pela equipe de desenvolvimento.*/
                case '0': 
                    animation_deck_suits(); // Chama a animação dos naipes
                    control_gpios(0, 0, 0, 1); // Especificação opcional: gerar sinal sonoro para uma das animações usando o componente buzzer.  
                    break;
                case '1':
                    animation_snake(); // Chama a animação da cobrinha
                    break;
                case '2':
                    break;
                case '3':
                    break;
                case '4':
                    break;
                case '5':
                    break;
                case '6':
                    break;
                case '7':
                    break;
                case 'A': // Liga todos os LEDs
                    control_gpios(1, 1, 1, 0);
                    break;
                case 'C': // Liga o LED vermelho ->  todos os LEDs deverão ser ligados na cor vermelha, no nível de intensidade de 80% da luminosidade máxima. 
                    control_gpios(1, 0, 0, 0);
                    break;
                case 'B': // Liga o LED azul -> todos os LEDs deverão ser ligados na cor azul, no nível de intensidade de 100%
                    control_gpios(0, 1, 0, 0);
                    break;
                case 'D': // Liga o LED verde -> todos os LEDs deverão ser ligados na cor verde, no nível de intensidade de 50% da luminosidade máxima. 
                    control_gpios(0, 0, 1, 0);
                    break;
                case '#': // Liga todos os LEDs -> todos os LEDs deverão ser ligados na cor branca, no nível de intensidade de 20% da luminosidade máxima.
                    control_gpios(1, 1, 1, 0);
                    break;
                case '*': // o Raspberry Pi Pico W deve sair do modo de execução e habilitar o modo de gravação via software (reboot).
                    break;
                default: // Desliga os LEDs
                    control_gpios(0, 0, 0, 0);
                    break;
            }
        }
        sleep_ms(100); // delay para evitar leituras repetidas
    }  
    return 0;   
}
