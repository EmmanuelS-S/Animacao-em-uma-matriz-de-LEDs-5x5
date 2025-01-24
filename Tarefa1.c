#include <pico/stdlib.h>
#include <stdio.h>
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "ws2812.pio.h"  // Biblioteca para WS2812

// Definindo linhas e colunas do teclado matricial
#define linhas 4
#define colunas 4

// Mapas de pinos para o teclado matricial
const uint pinosLinhas[linhas] = {8, 7, 6, 5};
const uint pinosColunas[colunas] = {4, 3, 2, 28};

// Definições da Matriz de LEDs 5x5
#define NUM_LEDS 25
#define LED_PIN 2 // Pino de saída para WS2812
#define ANIMATION_FPS 10 // Quadros por segundo

// Definições dos LEDs RGB
#define led_pin_green 11
#define led_pin_blue 12
#define led_pin_red 13

// Definição do buzzer
#define buzzer 21

// Mapas de teclas do teclado matricial
const char esquema_Teclado[linhas][colunas] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Funções auxiliares:
// Função para ler o teclado matricial 4x4
char ler_teclado() {
    for (int l = 0; l < LINHAS; l++) {
        gpio_put(pinosLinhas[l], true); // ativação da linha
        sleep_us(50); 

        for (int c = 0; c < COLUNAS; c++) {
            if (gpio_get(pinosColunas[c])) { // Verificação de sinal na coluna
                gpio_put(pinosLinhas[l], false); // Desativa a linha
                return esquema_Teclado[l][c]; // Retorna a tecla pressionada
            }
        }
        gpio_put(pinosLinhas[l], false); // Desativa a linha
    }

    return 0; 
}

uint32_t led_cores[NUM_LEDS]; // Array para armazenar cores dos LEDs

// Funções auxiliares para WS2812
void clear_leds() {
    for (int i = 0; i < NUM_LEDS; i++) {
        led_cores[i] = 0x000000; // Apaga todos os LEDs
    }
    put_pixel(led_cores);
}

//funcao para exibir um único frame na matriz de LEDs 5x5
void exibir_frame(uint32_t frame[5][5]) {
    int index = 0;
    for (int linha = 0; linha < 5; linha++) {
        for (int col = 0; col < 5; col++) {
            led_cores[index++] = frame[linha][col];
        }
    }
    put_pixel(led_cores);
}

void config_gpio() {
    // Configurando linhas do teclado como saídas
    for (int i = 0; i < LINHAS; i++) {
        gpio_init(pinosLinhas[i]);
        gpio_set_dir(pinosLinhas[i], GPIO_OUT);
    }

    // Configurando colunas do teclado como entradas com pull-down
    for (int i = 0; i < COLUNAS; i++) {
        gpio_init(pinosColunas[i]);
        gpio_set_dir(pinosColunas[i], GPIO_IN);
        gpio_pull_down(pinosColunas[i]);
    }
    // Incializando e configurando GPIOs dos LEDS e buzzer como saidas
    gpio_init(led_pin_green); gpio_set_dir(led_pin_green, GPIO_OUT);
    gpio_init(led_pin_blue); gpio_set_dir(led_pin_blue, GPIO_OUT);
    gpio_init(led_pin_red); gpio_set_dir(led_pin_red, GPIO_OUT);
    gpio_init(buzzer); gpio_set_dir(buzzer, GPIO_OUT);
}
// Função para acionar os pinos
void control_gpio(uint8_t red, uint8_t blue, uint8_t green, uint8_t buz) {
    gpio_put(led_pin_red, red);
    gpio_put(led_pin_blue, blue);
    gpio_put(led_pin_green, green);
    gpio_put(buzzer, buz);
}

int main() {
    stdio_init_all();
    config_gpio();
    while (true) {
        char teclado = ler_teclado(); // Leitura do teclado matricial 4x4

        if (teclado != 0) { // Se alguma tecla for pressionada
            printf("Tecla pressionada: %c\n", key);

            switch (teclado) {
                case '0':
                /*->As teclas de 0 a 6 do teclado matricial, caso acionadas, o sistema deve executar algum tipo de animação na matriz de LEDs 5x5. Os tipos de desenho, cores (RGB) e 
                luminosidades associados a estas imagens/animações ficam a critério dos membros da equipe de desenvolvimento, Cada animação deve possuir, no mínimo, 5 frames 
                (imagens diferentes) e o FPS (quantidade de quadros por segundo) também deve ser definido pela equipe de desenvolvimento.*/
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
                case '0': // Aciona o buzzer->gerar sinal sonoro para uma das animações usando o componente buzzer. 
                    control_buzzer(1);
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
