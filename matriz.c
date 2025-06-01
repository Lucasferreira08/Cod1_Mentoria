#include "matriz.h"

// Número de LEDs na matriz (5x5)
#define NUM_PIXELS 25
#define OUT_PIN 7

int num = 1;

// Luz vermelha (parte superior do semáforo)
static double desenho_alerta1[25] = {0.0, 0.0, 1.0, 0.0, 0.0,
                                    0.0, 0.0, 1.0, 0.0, 0.0, 
                                    0.0, 0.0, 1.0, 0.0, 0.0,
                                    0.0, 0.0, 1.0, 0.0, 0.0,
                                    0.0, 0.0, 1.0, 0.0, 0.0};

static double desenho_alerta2[25] = {0.0, 0.0, 0.0, 0.0, 1.0,
                                    0.0, 0.0, 0.0, 1.0, 0.0, 
                                    0.0, 0.0, 1.0, 0.0, 0.0,
                                    0.0, 1.0, 0.0, 0.0, 0.0,
                                    1.0, 0.0, 0.0, 0.0, 0.0};

static double desenho_alerta3[25] = {0.0, 0.0, 0.0, 0.0, 0.0,
                                    0.0, 0.0, 0.0, 0.0, 0.0, 
                                    1.0, 1.0, 1.0, 1.0, 1.0,
                                    0.0, 0.0, 0.0, 0.0, 0.0,
                                    0.0, 0.0, 0.0, 0.0, 0.0};


static double desenho_alerta4[25] = {1.0, 0.0, 0.0, 0.0, 0.0,
                                    0.0, 1.0, 0.0, 0.0, 0.0, 
                                    0.0, 0.0, 1.0, 0.0, 0.0,
                                    0.0, 0.0, 0.0, 1.0, 0.0,
                                    0.0, 0.0, 0.0, 0.0, 1.0};

// // Luz amarela (parte central do semáforo)
// static double desenho_amarelo[25] = {0.0, 0.0, 0.0, 0.0, 0.0,
//                              0.0, 0.0, 0.0, 0.0, 0.0, 
//                              0.0, 1.0, 1.0, 1.0, 0.0,
//                              0.0, 0.0, 0.0, 0.0, 0.0,
//                              0.0, 0.0, 0.0, 0.0, 0.0};

// // Luz verde (parte inferior do semáforo)
// static double desenho_verde[25] =   {0.0, 0.0, 0.0, 0.0, 0.0,
//                              0.0, 0.0, 0.0, 0.0, 0.0, 
//                              0.0, 0.0, 0.0, 0.0, 0.0,
//                              0.0, 1.0, 1.0, 1.0, 0.0,
//                              0.0, 0.0, 0.0, 0.0, 0.0};

// Estrutura do semáforo completo (para visualização)
static double apagar[25] = {0.0, 0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0, 0.0, 
                              0.0, 0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0, 0.0};

// Rotina para definir a intensidade das cores do LED (RGB)
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255; // Converte a intensidade de vermelho para 8 bits
  G = g * 255; // Converte a intensidade de verde para 8 bits
  B = b * 255; // Converte a intensidade de azul para 8 bits
  return (G << 24) | (R << 16) | (B << 8); // Retorna o valor RGB no formato 32 bits
}

void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        // Define a cor do LED com base no vetor de desenho
        if (desenho[24-i]>0.000) 
        {
            valor_led = matrix_rgb(b, r, g);
        }
        else
        {
            valor_led = matrix_rgb(0.0, 0.0, 0.0);
        }
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para a matriz de LEDs via PIO
    }
}

// Função para exibir o semáforo na matriz de LEDs
void apagar_matriz(PIO pio, uint sm)
{
    uint32_t valor_led;

    desenho_pio(apagar, valor_led, pio, sm, 0.0, 0.0, 0.0); // Vermelho (r=1.0)
}

// Função para exibir o semáforo na matriz de LEDs
void desenhar_alerta(PIO pio, uint sm)
{
    uint32_t valor_led;

    if (num==1)
    {
        desenho_pio(desenho_alerta1, valor_led, pio, sm, 0.1, 0.0, 0.0); // Vermelho (r=1.0)
        apagar_matriz( pio,  sm);
    }
    else if (num==2)
    {
        desenho_pio(desenho_alerta2, valor_led, pio, sm, 0.1, 0.0, 0.0); // Vermelho (r=1.0)
        apagar_matriz( pio,  sm);
    }
    else if (num==3)
    {
        desenho_pio(desenho_alerta3, valor_led, pio, sm, 0.1, 0.0, 0.0); // Vermelho (r=1.0)
        apagar_matriz( pio,  sm);
    }
    else if (num==4)
    {
        desenho_pio(desenho_alerta4, valor_led, pio, sm, 0.1, 0.0, 0.0); // Vermelho (r=1.0)
        apagar_matriz( pio,  sm);
        num=0;
    }

    num++;
}

uint pio_init(PIO pio)
{
    // Ajusta o clock do RP2040 para 128 MHz (128000 kHz)
    set_sys_clock_khz(128000, false);

    // Carrega o programa PIO na memória do PIO e retorna o offset onde ele foi colocado
    uint offset = pio_add_program(pio, &pio_matrix_program);

    // Reserva um state machine livre (bloqueante) e retorna seu índice
    uint sm = pio_claim_unused_sm(pio, true);

    // Inicializa o state machine com o programa carregado, definindo pino de saída
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    return sm;
}

