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

//uint32_t led_colors[NUM_LEDS]; // Array para armazenar cores dos LEDs


/*void clear_leds() {
    for (int i = 0; i < NUM_LEDS; i++) {
        led_colors[i] = 0x000000; // Apaga todos os LEDs
    }
    put_pixel(led_colors);
}

void set_all_leds(uint32_t color) {
    for (int i = 0; i < NUM_LEDS; i++) {
        led_colors[i] = color;
    }
    put_pixel(led_colors);
}*/

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

     return 0;
    
}
