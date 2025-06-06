#ifndef MATRIZ_H  // Se INIT_CONFIG_H não estiver definido
#define MATRIZ_H  // Defina INIT_CONFIG_H

#include "pio_matriz.pio.h"

// Initializes the keypad
uint32_t matrix_rgb(double b, double r, double g);

// Checks if a specific key is pressed
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);

// Função para exibir o semáforo na matriz de LEDs
void apagar_matriz(PIO pio, uint sm);

uint pio_init(PIO pio);

// Novas funções para a animação da ventoinha
void animacao_ventoinha_desenhar_proximo_frame(PIO pio, uint sm, double r, double g, double b);

#endif