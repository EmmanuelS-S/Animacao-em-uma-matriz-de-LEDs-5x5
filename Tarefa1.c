#include <pico/stdlib.h>
#include <stdio.h>

// Definindo linhas e colunas do teclado matricial
// Definindo linhas e colunas do teclado matricial
#define linha1 8  
#define linha2 7  
#define linha3 6  
#define linha4 5  
#define coluna1 4 
#define coluna2 3  
#define coluna3 2  
#define coluna4 28  

// Definições dos LEDs RGB
#define led_pin_green 11
#define led_pin_blue 12
#define led_pin_red 13

// Definição do buzzer
#define buzzer 21

void config_gpio() {
    // Incializando e configurando GPIOs como saidas
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

