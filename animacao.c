#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "blink.pio.h" // Biblioteca para manipular o aqrquivo blink.pio

// Definindo Linhas e Colunas do teclado matricial
#define ROWS 4
#define COL 4

// Mapas de pinos para o teclado matricial
const uint ROW_PIN[ROWS] = {8, 7, 6, 5};
const uint COL_PIN[COL] = {4, 3, 2, 28};

// Definições da Matriz de LEDs 5x5
#define NUM_LEDS 25
#define LED_PIN 22 // Pino de saída para WS2812
#define ANIMATION_FPS 10 // Quadros por segundo

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

uint32_t led_colors[NUM_LEDS]; // Array para armazenar cores dos LEDs

// Funções auxiliares para WS2812
void clean_leds() {
    for (int i = 0; i < NUM_LEDS; i++) {
        led_colors[i] = 0x000000; // Apaga todos os LEDs
    }
    put_pixel(led_colors);
}

//Função para exibir um único frame na matriz de LEDs 5x5
void show_frame(uint32_t frame[5][5]) {
    int index = 0;
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            led_colors[index++] = frame[row][col];
        }
    }
    put_pixel(led_colors);
}
//Executar uma animação composta por vários frames.
void run_animation(uint32_t animation[][5][5], int num_frames) {
    for (int frame = 0; frame < num_frames; frame++) {
        show_frame(animation[frame]);
        sleep_ms(1000 / ANIMATION_FPS); // Controla o FPS
    }
    clean_leds(); // Apaga os LEDs após a animação
}

// Funcao para Inicializar as animações definindo os frames de cada uma, e um exemplo de Animaçao
void init_animation(uint32_t animation[7][5][5][5]) {
    // Animação 1 (Exemplo: LEDs piscando alternadamente)
    uint32_t red = 0xFF0000, black = 0x000000;
    animation[0][0][0][0] = red;
    animation[0][0][0][1] = black;
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
void control_gpio(uint8_t red, uint8_t blue, uint8_t green, uint8_t buzzer) {
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
                case '0': // Aciona o buzzer->gerar sinal sonoro para uma das animações usando o componente buzzer. 
                    control_buzzer(1);
                    break;
                case 'A': // Liga todos os LEDs
                    control_leds(1, 1, 1);
                    break;
                case 'C': // Liga o LED vermelho ->  todos os LEDs deverão ser ligados na cor vermelha, no nível de intensidade de 80% da luminosidade máxima. 
                    control_leds(1, 0, 0);
                    break;
                case 'B': // Liga o LED azul -> todos os LEDs deverão ser ligados na cor azul, no nível de intensidade de 100%
                    control_leds(0, 1, 0);
                    break;
                case 'D': // Liga o LED verde -> todos os LEDs deverão ser ligados na cor verde, no nível de intensidade de 50% da luminosidade máxima. 
                    control_leds(0, 0, 1);
                    break;
                case '#': // Liga todos os LEDs -> todos os LEDs deverão ser ligados na cor branca, no nível de intensidade de 20% da luminosidade máxima.
                    control_leds(1, 1, 1);
                    break;
                case '*': // Desliga o buzzer -> o Raspberry Pi Pico W deve sair do modo de execução e habilitar o modo de gravação via software (reboot).
                    control_buzzer(0);
                    break;
                default: // Desliga os LEDs
                    control_leds(0, 0, 0);
                    break;
            }
        }
        sleep_ms(100); // delay para evitar leituras repetidas
    }  
    return 0;   
}